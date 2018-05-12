cmake_minimum_required(VERSION 3.6)
project(piledstrip-server)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(rpi-many-ws2812b/lib)

find_package(Boost COMPONENTS program_options system filesystem REQUIRED)

include_directories(${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
link_directories(${LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})

add_executable(
	piledstrip-server
	EspInterpreter.cpp
	EspInterpreter.h
	Interpreter.h
	LedInterface.cpp
	LedInterface.h
	main.cpp
	DemoInterface.cpp
	DemoInterface.h
	OpcInterpreter.cpp
	OpcInterpreter.h
	RpiManyInterface.cpp
	RpiManyInterface.h
	Server.h
	StripConfig.h
	TcpServer.cpp
	TcpServer.h
	UdpServer.cpp
	UdpServer.h
)

target_link_libraries(
	piledstrip-server
	boost_system
	pthread
	rpi-many-ws2812b
	${Boost_LIBRARIES}
)
