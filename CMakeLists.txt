cmake_minimum_required(VERSION 3.31)
project(SoftRenderPipeline)

# 全局配置
set(CMAKE_CXX_STANDARD 20) # 设置C++标准为20
set(CMAKE_BUILD_TYPE Release) # 设置构建类型为Release

# 第三方库
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/rapidobj)
add_subdirectory(source)