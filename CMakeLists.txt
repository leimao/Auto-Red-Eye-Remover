cmake_minimum_required( VERSION 3.0.0 )
set ( CMAKE_CXX_STANDARD 11 )

project( AutoRedEyeRemover VERSION 1.0.0 )

set(MODEL_FACE_FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/models/haarcascade_frontalface_alt.xml")
set(MODEL_EYE_FILEPATH "${CMAKE_CURRENT_SOURCE_DIR}/models/haarcascade_eye_tree_eyeglasses.xml")

add_subdirectory(lib)
add_subdirectory(main)

