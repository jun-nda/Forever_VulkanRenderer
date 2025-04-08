# https://blog.csdn.net/iuu77/article/details/129229361
cmake_minimum_required (VERSION 3.14)
 
project (Forever_VulkanRenderer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${PROJECT_SOURCE_DIR}/external/assimp)

SET(GLFW_INSTALL OFF CACHE BOOL "" FORCE) 
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glfw)


file(GLOB_RECURSE engineHeaders CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/engine/*.h")
file(GLOB_RECURSE engineSource CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/engine/*.cpp")

file(GLOB_RECURSE editorHeaders CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/editor/*.h")
file(GLOB_RECURSE editorSource CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/editor/*.cpp")

# aux_source_directory(${PROJECT_SOURCE_DIR}/source/engine ENGINE_LIST)
# aux_source_directory(${PROJECT_SOURCE_DIR}/source/editor EDITOR_LIST)

# include_directories(${PROJECT_SOURCE_DIR}/source/engine)
# include_directories(${PROJECT_SOURCE_DIR}/source/editor)

include_directories()

add_executable(Forever_VulkanRenderer ${editorSource} ${editorHeaders} ${engineSource} ${engineHeaders})

source_group(TREE "${PROJECT_SOURCE_DIR}/source/engine" PREFIX "engine" FILES ${engineSource} ${engineHeaders}) 
source_group(TREE "${PROJECT_SOURCE_DIR}/source/editor" PREFIX "editor" FILES ${editorSource} ${editorHeaders}) 

find_package(Vulkan REQUIRED)

target_precompile_headers(Forever_VulkanRenderer PRIVATE 
    "${PROJECT_SOURCE_DIR}/source/engine/utils/utils.h" 
)

target_link_libraries(Forever_VulkanRenderer PUBLIC glfw assimp Vulkan::Vulkan)
target_include_directories(Forever_VulkanRenderer PUBLIC 
    "${PROJECT_SOURCE_DIR}/external/include" 
    "${PROJECT_SOURCE_DIR}/source/engine" 
)