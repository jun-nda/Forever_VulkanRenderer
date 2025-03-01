# https://blog.csdn.net/iuu77/article/details/129229361
cmake_minimum_required (VERSION 3.14)
 
project (learn_cmake)

set(CMAKE_CXX_STANDARD 17)
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

add_executable(main ${editorSource} ${editorHeaders} ${engineSource} ${engineHeaders})

source_group(TREE "${PROJECT_SOURCE_DIR}/source/engine" PREFIX "engine" FILES ${engineSource} ${engineHeaders}) 
source_group(TREE "${PROJECT_SOURCE_DIR}/source/editor" PREFIX "editor" FILES ${editorSource} ${editorHeaders}) 

target_link_libraries(main PUBLIC glfw assimp)
target_include_directories(main PUBLIC 
    "${PROJECT_SOURCE_DIR}/external/include" 
    "${PROJECT_SOURCE_DIR}/source/engine" 
)