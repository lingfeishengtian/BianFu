cmake_minimum_required(VERSION 2.8)
project(BianFu)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(LLVM REQUIRED CONFIG PATHS thirdparty/llvm/lib/cmake/llvm)

set(ANTLR_EXECUTABLE /usr/share/java/antlr-4.8-complete.jar)

if(UNIX AND NOT APPLE)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4_INCLUDE_DIRS})
add_definitions(-DANTLR4CPP_STATIC)
include_directories(${ANTLR4_INCLUDE_DIRS})

find_package(ANTLR REQUIRED)
ENDIF()

set(CMAKE_CXX_STANDARD 14)
include_directories(thirdparty/antlr/include)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
include_directories(gen)

link_directories(${CMAKE_SOURCE_DIR}/thirdparty/antlr/)

file(GLOB srcFiles src/error/**.cpp src/scope/**.cpp src/scope/primitives/**.cpp src/scope/special/**.cpp src/visitors/**.cpp)
file(GLOB generated gen/**.cpp)

SET(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
message(STATUS ${generated})

add_executable(BianFu src/main.cpp ${generated} ${srcFiles} src/error/BianFuLog.cpp src/error/BianFuLog.h src/error/BianFuError.cpp src/error/BianFuError.h src/scope/primitives/BFFloatPrimitive.cpp src/scope/primitives/BFFloatPrimitive.h src/visitors/CompileVisitor.cpp src/visitors/CompileVisitor.h src/scope/CompilerScope.cpp src/scope/CompilerScope.h)
llvm_map_components_to_libnames(llvm_libs support core irreader executionengine codegen)

target_link_libraries(BianFu ${llvm_libs})
target_link_libraries(BianFu libantlr4-runtime.a)