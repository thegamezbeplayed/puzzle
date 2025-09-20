cmake_minimum_required(VERSION 3.21)
project(reflect)

include(FetchContent)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONC REQUIRED json-c)

set(CMAKE_C_STANDARD 99)

add_compile_definitions(RAYLIB_CUSTOM_CONFIG)
# Raylib requires OpenGL and other system libs

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)
#add_subdirectory(vendor/raylib)

include_directories(include)         # Your config.h
#include_directories(vendor/raylib/src)
#include_directories(vendor/raylib/src/external)
add_executable(${PROJECT_NAME})
add_subdirectory(src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/resources)
add_subdirectory(resources)
target_link_libraries(${PROJECT_NAME} raylib
  ${JSONC_LIBRARIES}
)
# Let CMake inherit the include dirs from raylib
target_include_directories(${PROJECT_NAME}
    PRIVATE
    includes
        ${JSONC_INCLUDE_DIRS}
)
# Libraries & link dirs

target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g -O0)
target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)

if("${PLATFORM}" MATCHES "WEB")
    add_custom_command(
        TARGET ${PROJECT_NAME} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/../resources
    )
    #DEPENDS ${PROJECT_NAME}
else()
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
    )
  endif()
# Web Configurations
if ("${PLATFORM}" MATCHES "WEB")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PUBLIC -sUSE_GLFW=3 PUBLIC --preload-file resources)
endif()

# macOS/Linux extra libs (raylib needs these)
if(APPLE)
  target_link_libraries(${PROJECT_NAME} m dl pthread GL X11 Xrandr Xinerama Xcursor Xi)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(OpenGL_LIBRARY OpenGL REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(CoreVideo_LIBRARY CoreVideo REQUIRED)

    target_link_libraries(game ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${CoreVideo_LIBRARY})
endif()
