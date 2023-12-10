cmake_minimum_required(VERSION 3.22)
option(VERBOSE_OUTPUT OFF)

set(PROJECT_NAME ray-traciing-weekone)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#include(InitVCPKG)
include(Functions)

project(${PROJECT_NAME})

set(EXE_PLATFORM_KEYWORD "")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(EXE_PLATFORM_KEYWORD "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(EXE_PLATFORM_KEYWORD "MACOSX_BUNDLE")
endif()

set(SOURCE_FILES
    ## Entrypoint
    src/main.cpp

    ## Sources
    src/color.h
    src/hittable_list.h
    src/hittable.h
    src/ray.h
    src/sphere.h
    src/utils.h
    src/vec3.h
)
source_group("Sources" SOURCE_FILES REGULAR_EXPRESSION ".*\\.(h|cpp)")
source_group("Shaders" SOURCE_FILES REGULAR_EXPRESSION ".*\\.glsl")

# find_package(assimp REQUIRED)
# find_package(glad REQUIRED)
# find_package(glfw3 REQUIRED)
# find_package(imgui REQUIRED)
# find_package(OpenGL REQUIRED)
# find_package(Stb REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# target_link_libraries(
#     ${PROJECT_NAME}
#         assimp::assimp
#         glad::glad
#         glfw
#         imgui::imgui
#         OpenGL::GL
# )

# target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "$<TARGET_PROPERTY:${PROJECT_NAME},SOURCE_DIR>/resources"
#         "$<TARGET_PROPERTY:${PROJECT_NAME},BINARY_DIR>/$<CONFIG>/resources"
# )

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if(VERBOSE_OUTPUT)
    cmake_list_all_variables()
endif()
