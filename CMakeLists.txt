cmake_minimum_required(VERSION 3.31...4.0)

include(FetchContent)

project(kewin_craft LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(PRODUCTION OFF CACHE BOOL "Make this a production build" FORCE)

if(PRODUCTION)
        set(ASSETS_PATH "\"./assets/\"")
else()
        set(ASSETS_PATH "\"${PROJECT_SOURCE_DIR}/assets/\"")
endif()

configure_file(config.h.in config.h)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)

FetchContent_MakeAvailable(glm glfw)

add_subdirectory(external/glad)
add_subdirectory(external/FastNoiseLite)
add_subdirectory(external/stb_image)

set(SOURCE_FILES
    src/app.cpp
    src/block_vertex_array.cpp
    src/camera.cpp
    src/chunk.cpp
    src/cursor.cpp
    src/main.cpp
    src/map.cpp
    src/shader.cpp
    src/texture.cpp
    src/window.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glad glfw FastNoiseLite stb_image)
