cmake_minimum_required(VERSION 2.8)
project(BianFu)

find_package(LLVM REQUIRED CONFIG PATHS thirdparty/llvm/lib/cmake/llvm)
# To run this project, you should build antlr4 for c++ with cmake and then find the dist folder and get the .a file
# You should also find the includes for antlr and put thim in antlr/include
set(CMAKE_CXX_STANDARD 14)
include_directories(thirdparty/antlr/include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(gen)

file(GLOB srcFiles src/error/**.cpp src/scope/**.cpp src/scope/primitives/**.cpp src/scope/special/**.cpp src/visitors/**.cpp)
file(GLOB generated gen/**.cpp)

SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
message(STATUS ${generated})

add_executable(BianFu src/main.cpp ${generated} ${srcFiles} src/error/BianFuLog.cpp src/error/BianFuLog.h src/error/BianFuError.cpp src/error/BianFuError.h src/scope/primitives/BFFloatPrimitive.cpp src/scope/primitives/BFFloatPrimitive.h)
llvm_map_components_to_libnames(llvm_libs support core irreader executionengine codegen)

target_link_libraries(BianFu ${llvm_libs})
target_link_libraries(BianFu "${CMAKE_SOURCE_DIR}/thirdparty/antlr/libantlr4-runtime.a")