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

set(APP_SOURCES
    src/app/application.cpp
)

set(PLATFORM_SOURCES
    src/platform/window.cpp
)

set(GRAPHICS_SOURCES
    src/graphics/block_vertex_array.cpp
    src/graphics/shader.cpp
    src/graphics/texture.cpp
)

set(GAMEPLAY_SOURCES
    src/gameplay/camera.cpp
)

set(WORLD_SOURCES
    src/world/chunk.cpp
    src/world/map.cpp
)

set(SOURCE_FILES
    ${APP_SOURCES}
    ${PLATFORM_SOURCES}
    ${GRAPHICS_SOURCES}
    ${GAMEPLAY_SOURCES}
    ${WORLD_SOURCES}
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm glad glfw FastNoiseLite stb_image)
