cmake_minimum_required(VERSION 3.16)
project(finger_tracker CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui videoio)

# libmediapipe.so (MediaPipe Tasks C API) ships with the mediapipe pip package.
set(MEDIAPIPE_LIB
    "${CMAKE_CURRENT_SOURCE_DIR}/cam_venv/lib/python3.11/site-packages/mediapipe/tasks/c/libmediapipe.so"
    CACHE FILEPATH "Path to libmediapipe.so")
if(NOT EXISTS "${MEDIAPIPE_LIB}")
  message(FATAL_ERROR "libmediapipe.so not found at ${MEDIAPIPE_LIB}; set -DMEDIAPIPE_LIB=...")
endif()
get_filename_component(MEDIAPIPE_LIB_DIR "${MEDIAPIPE_LIB}" DIRECTORY)

add_executable(finger_tracker finger_tracker.cpp)
target_include_directories(finger_tracker PRIVATE ${OpenCV_INCLUDE_DIRS})
target_compile_definitions(finger_tracker PRIVATE MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(finger_tracker PRIVATE ${OpenCV_LIBS} "${MEDIAPIPE_LIB}")
set_target_properties(finger_tracker PROPERTIES BUILD_RPATH "${MEDIAPIPE_LIB_DIR}")
