cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(ndvr-cpp)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)

set(CMAKE_BUILD_TYPE Debug)  

list(APPEND CMAKE_PREFIX_PATH "/usr/include/libtorch;/usr/include/opencv4")
list(APPEND debug_modules opencv_highgui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OPENCV_CXX_FLAGS} -DFFMPEG=OFF -DWITH_OPENCL=1 -pthread -Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Torch REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-rtsp-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-sdp-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-net-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-video-1.0>=1.4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-pbutils-1.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-pbutils-1.0>=1.2)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-app-1.0>=1.4) 

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
)

link_directories(
    ${GLIB_LIBRARY_DIRS}
    ${GSTREAMER_LIBRARY_DIRS}
)


FILE(GLOB_RECURSE sourcefiles "src/main.cpp")
FILE(GLOB_RECURSE headerfiles "src/*.h")

set(CMAKE_CXX_STANDARD 17)

add_executable(main ${sourcefiles} ${headerfiles})

target_link_libraries(main
    ${OpenCV_LIBS}
    ${GSTREAMER_LIBRARIES}
    ${TORCH_LIBRARIES}
) 

set_property(TARGET main PROPERTY CXX_STANDARD 17)


