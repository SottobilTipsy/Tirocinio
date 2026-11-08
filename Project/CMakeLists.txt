cmake_minimum_required(VERSION 3.15)
project(knotangle)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_executable(knotangle
        main.cpp
        CoreModel/grid.cpp
        CoreModel/Gridmaker.cpp
        Graphics/temp.cpp
        Graphics/Nbox.cpp
        Graphics/Button.cpp
        Graphics/Checkbox.cpp
        Graphics/knotbox.cpp
    )

target_include_directories(knotangle PRIVATE
    CoreModel
    Graphics
)

target_link_libraries(knotangle PRIVATE raylib)

file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/Resources SYMBOLIC)

if (APPLE)
    target_link_libraries(knotangle PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL" "-framework CoreVideo")
elseif(WIN32)
    target_link_libraries(knotangle PRIVATE winmm)
endif()