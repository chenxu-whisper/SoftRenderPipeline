# 递归添加源文件
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# 定义可执行文件
add_executable(SoftRenderPipeline ${SRC_FILES})

# 包含头文件
target_include_directories(SoftRenderPipeline PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/glm)
target_include_directories(SoftRenderPipeline PRIVATE ${CMAKE_SOURCE_DIR}/source/include)
