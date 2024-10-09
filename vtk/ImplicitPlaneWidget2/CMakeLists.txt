cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

option(VERBOSE_OUTPUT OFF)

set(PROJECT_NAME ImplicitPlaneWidget2)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/../../shared/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../shared/cmake")

include(InitVCPKG)
include(Functions)
include(ToolchainUpdate)
include(GitVersion)

# Git Version implementation for CMake
get_version_info("APP")

# Initialize Date and Time variables
init_time_and_date_variables()

project(${PROJECT_NAME} VERSION ${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}.${APP_VERSION_TWEAK} LANGUAGES CXX)

set(EXE_PLATFORM_KEYWORD "")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(EXE_PLATFORM_KEYWORD "")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(EXE_PLATFORM_KEYWORD "MACOSX_BUNDLE")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-changes-meaning)
endif()

project(ImplicitPlaneWidget2)

find_package(VTK COMPONENTS
    CommonColor
    CommonCore
    CommonDataModel
    FiltersCore
    FiltersSources
    IOXML
    InteractionStyle
    InteractionWidgets
    RenderingContextOpenGL2
    RenderingCore
    RenderingFreeType
    RenderingGL2PSOpenGL2
    RenderingOpenGL2
)

if(NOT VTK_FOUND)
    message(FATAL_ERROR "ImplicitPlaneWidget2: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${PROJECT_NAME}.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

if(VERBOSE_OUTPUT)
    cmake_list_all_variables()
endif()