set(SOURCE_FILES main.cpp)
add_executable(bbb ${SOURCE_FILES})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

add_library(OwnStuff Matrix.cpp Point.cpp Color.cpp Triangle.cpp)

target_link_libraries(bbb ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} OwnStuff)
