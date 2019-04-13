cmake_minimum_required( VERSION 3.0.0 )

project( AutoRedEyeRemoverApp VERSION 1.0.0 )

find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS filesystem)

# message(STATUS "============================")
# message(STATUS "${Boost_INCLUDE_DIRS}")
# message(STATUS "${Boost_LIBRARY_DIRS}")
# Boost library will only be non-empty if COMPONENTS are provided.
# https://cmake.org/cmake/help/v3.14/module/FindBoost.html
# message(STATUS "${Boost_LIBRARIES}")
# message(STATUS "${Boost_FILESYSTEM_LIBRARY}")
# message(STATUS "${OpenCV_INCLUDE_DIRS}")
# message(STATUS "${OpenCV_LIBRARIES}")
# message(STATUS "============================")

include_directories( ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )
add_executable( ${PROJECT_NAME} main.cpp )
target_link_libraries( ${PROJECT_NAME} PUBLIC ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE utils)