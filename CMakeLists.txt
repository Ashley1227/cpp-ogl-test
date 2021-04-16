cmake_minimum_required(VERSION 3.5)
project(Thingy VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(source_dir "${PROJECT_SOURCE_DIR}/src/")
file(GLOB source_files "${source_dir}/*.cpp")

add_executable(${PROJECT_NAME} ${source_files} src/rendering/shader/Shader.cpp src/rendering/shader/Shader.h src/rendering/renderer/Renderer.h src/rendering/renderer/Renderer3D.cpp src/rendering/renderer/Renderer3D.h src/rendering/renderer/GameRenderer.cpp src/rendering/renderer/GameRenderer.h src/rendering/renderer/Renderer.cpp src/rendering/camera/Camera.cpp src/rendering/camera/Camera.h src/global.h src/rendering/VertexArray.cpp src/rendering/VertexArray.h src/rendering/shader/Uniform.h src/world/chunk/Chunk.cpp src/world/chunk/Chunk.h src/world/block/blocks.h src/rendering/IndexArray.cpp src/rendering/IndexArray.h src/rendering/Mesh.cpp src/rendering/Mesh.h src/rendering/GLHelper.h src/math/noise.h src/math/noise.cpp src/rendering/renderer/FPSCounterRenderer.cpp src/rendering/renderer/FPSCounterRenderer.h src/world/World.cpp src/world/World.h src/global.cpp)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw-3.3.2/)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm/)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glew-2.1.0/build/cmake/)
include_directories(${PROJECT_NAME} PUBlIC ${PROJECT_SOURCE_DIR}/lib/glew-2.1.0/include/ PUBLIC ${PROJECT_SOURCE_DIR}/lib/stb/)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glew glm)