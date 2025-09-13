cmake_minimum_required(VERSION 3.31)
project(SoftRenderPipeline)

set(CMAKE_CXX_STANDARD 20)

# 第三方库
add_subdirectory(thirdparty/glm)
add_subdirectory(source)