cmake_minimum_required(VERSION 2.8)
PROJECT (protoDemo)

# Add main src
aux_source_directory(. DIR_SRCS)

# Find required protobuf package
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()
 
include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/AddressBook.proto)
 
ADD_EXECUTABLE(protoDemo ${DIR_SRCS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protoDemo ${PROTOBUF_LIBRARIES})
