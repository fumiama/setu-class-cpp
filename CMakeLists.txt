cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(setu-class-cpp)
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_INSTALL_PREFIX /usr/local/)

set(OpenCV_DIR /usr/local/share/opencv4/)
set(OpenCV_LIBRARY_DIRS /usr/local/lib/)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
if (OpenCV_FOUND)
	MESSAGE (STATUS "OpenCV_DEFINITIONS: ${OpenCV_DEFINITIONS}")
	MESSAGE (STATUS "OpenCV_INCLUDEDIRS: ${OpenCV_INCLUDE_DIRS}")
	MESSAGE (STATUS "OpenCV_LIBRARYDIRS: ${OpenCV_LIBRARY_DIRS}")

	# For testing
	# add_executable(setu setu.cpp)
	# target_link_libraries(setu ${TORCH_LIBRARIES} ${OpenCV_LIBS})

	# For building lib
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
else()
	MESSAGE (STATUS "OpenCV: not found")
endif(OpenCV_FOUND)

