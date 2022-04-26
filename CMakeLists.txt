cmake_minimum_required (VERSION 3.8)

project ("learnOpengl")

set(CMAKE_CXX_STANDARD 17)

# 添加依赖
include_directories("D:/OpenGL/glfw-3.3.7.bin.WIN64/include")
include_directories("D:/OpenGL/glad/include")
include_directories("D:/vs_studio/learnOpengl/include")

link_directories("D:/OpenGL/glfw-3.3.7.bin.WIN64/lib-vc2019")

# 编译
add_executable (learn "src/glad.c"  "src/shader/Shader.cpp" 
					"src/introduction/helloShader2.cpp")

# 链接
target_link_libraries(learn "glfw3.lib")