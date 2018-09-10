cmake_minimum_required ( VERSION 3.1 )
set (BT_PROJECT_NAME BeastServer)

project(${BT_PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} /MT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_GENERATOR_PLATFORM x64)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.66.0 COMPONENTS thread REQUIRED)

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/beastserver)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADERS
	${HEADER_DIR}/ClientManager.h
    ${HEADER_DIR}/ConditionVariable.h
    ${HEADER_DIR}/EventManager.h
    ${HEADER_DIR}/HttpClient.h
    ${HEADER_DIR}/Server.h
    ${HEADER_DIR}/TCPAcceptor.h
    ${HEADER_DIR}/TimedRunner.h
    ${HEADER_DIR}/Typedef.h)

set(SOURCES
	${SOURCE_DIR}/ClientManager.cpp
    ${SOURCE_DIR}/EventManager.cpp
    ${SOURCE_DIR}/HttpClient.cpp
    ${SOURCE_DIR}/Server.cpp
    ${SOURCE_DIR}/Source.cpp
    ${SOURCE_DIR}/TCPAcceptor.cpp
    ${SOURCE_DIR}/TimedRunner.cpp)

include_directories(${HEADER_DIR})

include_directories(${Boost_INCLUDE_DIRS})

add_library(${BT_PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${BT_PROJECT_NAME} ${Boost_LIBRARIES})

set_property(TARGET ${BT_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${BT_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)