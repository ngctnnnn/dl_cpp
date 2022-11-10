cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(dml)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_BUILD_TYPE Debug)  

list(APPEND CMAKE_PREFIX_PATH "/usr/include/libtorch")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} -DFFMPEG=OFF -DWITH_OPENCL=1 -pthread -Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(
    ${GLIB_INCLUDE_DIRS}
)

link_directories(
    ${GLIB_LIBRARY_DIRS}
)


FILE(GLOB_RECURSE sourcefiles "src/main.cpp")
FILE(GLOB_RECURSE headerfiles "src/*.h" "include/*.h" "include/*.cpp")

set(CMAKE_CXX_STANDARD 17)

add_executable(main ${sourcefiles} ${headerfiles})

target_link_libraries(main
    ${TORCH_LIBRARIES}
) 

set_property(TARGET main PROPERTY CXX_STANDARD 17)