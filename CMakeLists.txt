cmake_minimum_required(VERSION 3.24)

project(th07-vulkan VERSION 0.1.0 LANGUAGES CXX)

option(TH07_BUILD_APP "Build the SDL3/Vulkan application shell" ON)
option(TH07_BUILD_TESTS "Build C++ tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(th07_game
    src/game/ReferencePaths.cpp
    src/game/Th07Constants.cpp
)

target_include_directories(th07_game
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(th07_game PUBLIC cxx_std_20)

if(TH07_BUILD_APP)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)

    add_executable(th07-vulkan
        src/app/main.cpp
    )

    target_link_libraries(th07-vulkan
        PRIVATE
            th07_game
            PkgConfig::SDL3
    )
endif()

if(TH07_BUILD_TESTS)
    include(CTest)
    enable_testing()

    add_executable(th07-tests
        tests/test_reference_paths.cpp
    )

    target_link_libraries(th07-tests PRIVATE th07_game)

    add_test(NAME th07-reference-paths COMMAND th07-tests)
endif()
