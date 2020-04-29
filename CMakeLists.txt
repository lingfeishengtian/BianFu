cmake_minimum_required(VERSION 2.8)
project(BianFu)

# To run this project, you should build antlr4 for c++ with cmake and then find the dist folder and get the .a file
# You should also find the includes for antlr and put thim in antlr/antlr4-runtime
set(CMAKE_CXX_STANDARD 14)
include_directories(antlr/antlr4-runtime/)
include_directories(gen)

file(GLOB srcFiles src/error/**.cpp src/scope/**.cpp src/scope/primitives/**.cpp src/visitors/**.cpp)
file(GLOB generated gen/**.cpp)

message(STATUS ${generated})

add_executable(BianFu src/main.cpp ${generated} ${srcFiles} src/error/BianFuLog.cpp src/error/BianFuLog.h src/error/BianFuError.cpp src/error/BianFuError.h src/scope/primitives/BFFloatPrimitive.cpp src/scope/primitives/BFFloatPrimitive.h)
target_link_libraries(BianFu "${CMAKE_SOURCE_DIR}/antlr/libantlr4-runtime.a")