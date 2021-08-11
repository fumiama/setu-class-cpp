cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(setu-class-cpp)
SET(CMAKE_BUILD_TYPE "Release")

set(CMAKE_PREFIX_PATH
        /home/fumiama/libtorch_cpu
        /home/luxiangzhe/opencv_3.4.3/build)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

add_library(setu SHARED setu.cpp)
add_library(setus STATIC setu.cpp)
set_target_properties(setus PROPERTIES OUTPUT_NAME setu)
add_executable(t test.c)

target_link_libraries(setu ${TORCH_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(setus ${TORCH_LIBRARIES} ${OpenCV_LIBS})
target_link_libraries(t setu)

INSTALL(TARGETS setu LIBRARY DESTINATION lib)
INSTALL(TARGETS setus ARCHIVE DESTINATION lib)
INSTALL(FILES setu.h DESTINATION include)
