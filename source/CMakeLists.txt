# 递归添加源文件
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# 定义可执行文件
add_executable(SoftRenderPipeline ${SRC_FILES})

# 包含头文件
target_include_directories(SoftRenderPipeline PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/glm)
target_include_directories(SoftRenderPipeline PRIVATE ${CMAKE_SOURCE_DIR}/source/include)

# glm宏
target_compile_definitions(SoftRenderPipeline PUBLIC GLM_FORCE_DEPTH_ZERO_TO_ONE GLM_FORCE_RADIANS GLM_FORCE_LEFT_HANDED)
