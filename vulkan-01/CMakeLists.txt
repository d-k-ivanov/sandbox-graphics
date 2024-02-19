cmake_minimum_required(VERSION 3.22)

option(VERBOSE_OUTPUT OFF)

set(PROJECT_NAME VukanApp)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

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

# Resource file
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Resources/App.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/App.rc")

set(SHADER_FILES
    Resources/Shaders/SimpleShader.vert
    Resources/Shaders/SimpleShader.frag
    Resources/Shaders/SimpleShader2.vert
    Resources/Shaders/SimpleShader2.frag
)

set(SOURCE_FILES
    Source/Main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/App.rc

    # Sources
    Source/ApplicationOne.cpp
    Source/ApplicationOne.h
    Source/Device.cpp
    Source/Device.h
    Source/Filesystem.cpp
    Source/Filesystem.h
    Source/Model.cpp
    Source/Model.h
    Source/Pipeline.cpp
    Source/Pipeline.h
    Source/SwapChain.cpp
    Source/SwapChain.h
    Source/Window.cpp
    Source/Window.h

    # Shaders
    ${SHADER_FILES}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})


# Find glslc executable
find_program(GLSLC_EXECUTABLE NAMES glslc)

if(NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please make sure Vulkan SDK is installed and available in your system's PATH.")
endif()


get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(isMultiConfig)
    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses\" exists, remove it!")
    endif()

    add_custom_target(
        always_run_resources_target ALL
        DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses" # fake! ensure we run!
    )

    add_custom_command(
        OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyResourses" # fake! ensure we run!
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resources"
    )

    foreach(SHADER_FILE ${SHADER_FILES})
        # Dummy target to ensure that resources are up-to-date
        if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Dummy${SHADER_FILE_NAME}")
            message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Dummy${SHADER_FILE_NAME}\" exists, remove it!")
        endif()

        get_filename_component(SHADER_FILE_NAME ${SHADER_FILE} NAME)
        set(SHADER_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Resources/Shaders/${SHADER_FILE_NAME}.spv")
        add_custom_command(
            OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Dummy${SHADER_FILE_NAME}" # fake! ensure we run!
            "${SHADER_OUTPUT_FILE}"
            COMMAND ${GLSLC_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}" -o "${SHADER_OUTPUT_FILE}"
            DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}"
        )
        list(APPEND SHADER_OUTPUT_FILES "${SHADER_OUTPUT_FILE}")
    endforeach()

    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyShaders")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyShaders\" exists, remove it!")
    endif()

    add_custom_target(
        always_run_shaders_target ALL
        DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/DummyShaders" # fake! ensure we run!
        ${SHADER_OUTPUT_FILES}
    )
else()
    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/DummyResourses")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/DummyResourses\" exists, remove it!")
    endif()

    add_custom_target(
        always_run_resources_target ALL
        DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/DummyResourses" # fake! ensure we run!
    )

    add_custom_command(
        OUTPUT
        "${CMAKE_CURRENT_BINARY_DIR}/DummyResourses" # fake! ensure we run!
        "${CMAKE_CURRENT_BINARY_DIR}/Resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        "${CMAKE_CURRENT_BINARY_DIR}/Resources"
    )

    foreach(SHADER_FILE ${SHADER_FILES})
        # Dummy target to ensure that resources are up-to-date
        if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Dummy${SHADER_FILE_NAME}")
            message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/Dummy${SHADER_FILE_NAME}\" exists, remove it!")
        endif()

        get_filename_component(SHADER_FILE_NAME ${SHADER_FILE} NAME)
        set(SHADER_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/Resources/Shaders/${SHADER_FILE_NAME}.spv")
        add_custom_command(
            OUTPUT
            "${CMAKE_CURRENT_BINARY_DIR}/Dummy${SHADER_FILE_NAME}" # fake! ensure we run!
            "${SHADER_OUTPUT_FILE}"
            COMMAND ${GLSLC_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}" -o "${SHADER_OUTPUT_FILE}"
            DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER_FILE}"
        )
        list(APPEND SHADER_OUTPUT_FILES "${SHADER_OUTPUT_FILE}")
    endforeach()

    # Dummy target to ensure that resources are up-to-date
    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/DummyShaders")
        message(FATAL_ERROR "\"${CMAKE_CURRENT_BINARY_DIR}/DummyShaders\" exists, remove it!")
    endif()

    add_custom_target(
        always_run_shaders_target ALL
        DEPENDS
        "${CMAKE_CURRENT_BINARY_DIR}/DummyShaders" # fake! ensure we run!
        ${SHADER_OUTPUT_FILES}
    )
endif()

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} ${EXE_PLATFORM_KEYWORD} ${SOURCE_FILES})
add_dependencies(${PROJECT_NAME} always_run_resources_target)
add_dependencies(${PROJECT_NAME} always_run_shaders_target)

# Use WinMain as a default endpoint on Windows
if(WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE "$<IF:$<CONFIG:Release>,TRUE,${CMAKE_WIN32_EXECUTABLE}>")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

# Enable PDB files in the Release configuration and a couple of useful flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
    target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Release>:/Zi>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/DEBUG:FASTLINK>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:REF>")
    target_link_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:Release>:/OPT:ICF>")
    target_compile_options(${PROJECT_NAME} PUBLIC "/MP")
endif()

target_link_libraries(
    ${PROJECT_NAME}
    glfw
    glm::glm-header-only
    Vulkan::Vulkan
)

if(VERBOSE_OUTPUT)
    cmake_list_all_variables()
endif()
