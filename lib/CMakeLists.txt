cmake_minimum_required( VERSION 3.0.0 )

project( UtilsLibrary VERSION 1.0.0 )
find_package( OpenCV REQUIRED )

# Create a library
# https://cmake.org/cmake/help/v3.0/command/add_library.html1
add_library(
	utils STATIC
	src/utils.h
	src/utils.cpp)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${OpenCV_INCLUDE_DIRS})
target_link_libraries( utils PUBLIC ${OpenCV_LIBRARIES} )

# Add a preprocessor definitions
# Try PUBLIC/PRIVATE/INTERFACE to see what will happen
# target_compile_definitions(utils PUBLIC MODEL_FACE_FILEPATH="/home/leimao/Workspace/Auto_Red_Eye_Remover/models/haarcascade_frontalface_alt.xml")
# target_compile_definitions(utils PUBLIC MODEL_EYE_FILEPATH="/home/leimao/Workspace/Auto_Red_Eye_Remover/models/haarcascade_eye_tree_eyeglasses.xml")

target_compile_definitions(utils PUBLIC MODEL_FACE_FILEPATH="${MODEL_FACE_FILEPATH}")
target_compile_definitions(utils PUBLIC MODEL_EYE_FILEPATH="${MODEL_EYE_FILEPATH}")
