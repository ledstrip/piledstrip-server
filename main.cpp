#include <cstdint>
#include <memory>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/asio/signal_set.hpp>

#include "LedInterface.h"
#include "OpcInterpreter.h"
#include "EspInterpreter.h"
#include "Server.h"
#include "TcpServer.h"
#include "UdpServer.h"
#include "RpiManyInterface.h"

namespace po = boost::program_options;
namespace pt = boost::property_tree;

const char* DEFAULT_CONFIG_FILE = "config.json";

void createDefaultConfigFile(pt::ptree& config)
{
	config.put("led_count", 300);
	pt::ptree servers;

	pt::ptree serverOpc;
	serverOpc.put("protocol", "tcp");
	serverOpc.put("port", 7777);
	serverOpc.put("interpreter", "opc");
	servers.push_back(std::make_pair("", serverOpc));

	pt::ptree serverEsp;
	serverEsp.put("protocol", "udp");
	serverEsp.put("port", 7777);
	serverEsp.put("interpreter", "esp");
	servers.push_back(std::make_pair("", serverEsp));

	config.add_child("servers", servers);

	pt::write_json(DEFAULT_CONFIG_FILE, config);
}

int main(int argc, char* argv[]) try
{
	// Parse arguments
	std::string configFile;

	po::options_description desc("Options");
	desc.add_options()
		("help", "print help message")
		("config", po::value<std::string>(&configFile)->default_value(DEFAULT_CONFIG_FILE),
			"JSON configuration file (if file does not exist a default configuration file is created automatically)")
		("verbose,v", "verbose logging");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
		return 0;
	}

	po::notify(vm);

	// Parse config file
	pt::ptree config;
	if (boost::filesystem::exists(configFile)) {
		try {
			pt::read_json(configFile, config);
		} catch (pt::json_parser::json_parser_error& e) {
			std::cerr << "Error parsing JSON config from \"" << configFile << "\": " << e.what() << std::endl;
		}
	} else {
		std::cout << "JSON config file \"" << configFile << "\" does not exist. Creating default JSON config." << std::endl;
		createDefaultConfigFile(config);
	}

	boost::asio::io_service io_service;
	boost::asio::signal_set signals(io_service, SIGINT, SIGTERM, SIGHUP);
	signals.async_wait([&io_service](const boost::system::error_code& /* error */, int /* signalNumber */) {
		io_service.stop();
	});

	std::vector<StripConfig> stripConfigs;

	pt::ptree children = config.get_child("strips");
	for (const auto& v : children) {
		const pt::ptree& serverTree = v.second;
		stripConfigs.push_back({
			serverTree.get<uint16_t>("led_count"),
			serverTree.get<bool>("flip")
		});
	}

	std::shared_ptr<LedInterface> ledInterface(new RpiManyInterface(stripConfigs));

	std::vector<std::unique_ptr<Server>> servers;
	std::vector<std::unique_ptr<Interpreter>> interpreters;

	children = config.get_child("servers");
	for (const auto& v : children) {
		const pt::ptree& serverTree = v.second;
		std::string internetProtocol = serverTree.get<std::string>("protocol");
		uint16_t port = serverTree.get<uint16_t>("port");
		std::string interpreterType = serverTree.get<std::string>("interpreter");

		std::unique_ptr<Interpreter> interpreter;
		if (interpreterType == "opc") {
			interpreter = std::make_unique<OpcInterpreter>(ledInterface);
		} else if (interpreterType == "esp") {
			interpreter = std::make_unique<EspInterpreter>(ledInterface);
		} else {
			std::cerr << "Unknown interpreter type \"" << interpreterType << "\"" << std::endl;
			continue;
		}

		std::unique_ptr<Server> server;
		if (internetProtocol == "tcp") {
			server = std::make_unique<TcpServer>(io_service, port, interpreter->getBufferSize());
		} else if (internetProtocol == "udp") {
			server = std::make_unique<UdpServer>(io_service, port);
		} else {
			std::cerr << "Unknown internet protocol \"" << internetProtocol << "\"" << std::endl;
			continue;
		}

		server->dataReceived.connect([interpreter = interpreter.get()](const std::string& source, uint8_t* data, size_t length) {
			std::cout << "Received " << length << " bytes from " << source << std::endl;
			interpreter->interpretData(data, length);
		});

		interpreters.push_back(std::move(interpreter));
		servers.push_back(std::move(server));
	}

	io_service.run();
} catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	return 1;
}
