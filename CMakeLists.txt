cmake_minimum_required(VERSION 3.22)
project(TradeStream)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})

find_package(PkgConfig REQUIRED)
set(ZMQ_LIB_DIR "/opt/homebrew/Cellar/zeromq/4.3.4/lib")
pkg_check_modules(ZMQ REQUIRED "${ZMQ_LIB_DIR}/pkgconfig/libzmq.pc")

add_executable(TradeStream main.cpp Agg/Agg.hpp AggStreamer/AggStreamer.hpp DataStore/DataStore.hpp FileHandle/FileHandle.hpp Serializer/Serializer.hpp TradeStreamer/TradeStreamer.hpp ZMQHandle/ZMQHandle.hpp FileHandle/CSVFileHandle.hpp Serializer/ChunkLenSerializer.hpp ZMQHandle/TCPZMQHandle.hpp ZMQHandleFactory/ZMQHandleFactory.hpp)

target_include_directories(TradeStream PRIVATE ${Boost_INCLUDE_DIR} ${ZMQ_INCLUDE_DIR})
target_link_libraries(TradeStream PRIVATE ${GTEST_LIBRARIES} ${Boost_LIBRARIES} "${ZMQ_LIB_DIR}/libzmq.dylib")
