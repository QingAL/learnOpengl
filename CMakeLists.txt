cmake_minimum_required (VERSION 3.8)

project ("learnOpengl")

set(CMAKE_CXX_STANDARD 17)

# 添加依赖
include_directories("D:/OpenGL/glfw-3.3.7.bin.WIN64/include")
include_directories("D:/OpenGL/glad/include")
include_directories("D:/vs_studio/learnOpengl/include")

link_directories("D:/OpenGL/glfw-3.3.7.bin.WIN64/lib-vc2019")

# load libs
add_library(myLib "src/lib/stb_image.cpp" "src/lib/Shader.cpp" "src/lib/glad.c" )

# 编译
add_executable (learn "src/introduction/helloCamera.cpp")

# 链接
target_link_libraries(learn "glfw3.lib" myLib)