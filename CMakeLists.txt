cmake_minimum_required(VERSION 3.24)

project(th07-vulkan VERSION 0.1.0 LANGUAGES CXX)

option(TH07_BUILD_APP "Build the SDL3/Vulkan application shell" ON)
option(TH07_BUILD_TESTS "Build C++ tests" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(th07_game
    src/BorderSystem.cpp
    src/CherrySystem.cpp
    src/ClearBonusSystem.cpp
    src/Th07EclFile.cpp
    src/EclOpcodeTables.cpp
    src/PlayerBombTables.cpp
    src/ReferencePaths.cpp
    src/Rng.cpp
    src/ShtFile.cpp
    src/Th07AnmFile.cpp
    src/Th07ArchiveReader.cpp
    src/Th07EffectTables.cpp
    src/Th07FileSystemRuntime.cpp
    src/Th07Constants.cpp
    src/Th07MsgFile.cpp
    src/Th07ResourceFormatTables.cpp
    src/Th07ResourceManifest.cpp
    src/Th07ScoreTables.cpp
    src/Th07StdFile.cpp
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
        src/Th07App.cpp
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

    add_executable(th07-sht-tests
        tests/test_sht_parser.cpp
    )

    target_link_libraries(th07-sht-tests PRIVATE th07_game)

    add_test(NAME th07-sht-parser COMMAND th07-sht-tests)

    add_executable(th07-player-layout-tests
        tests/test_player_layout.cpp
    )

    target_link_libraries(th07-player-layout-tests PRIVATE th07_game)

    add_test(NAME th07-player-layout COMMAND th07-player-layout-tests)

    add_executable(th07-ecl-opcode-tests
        tests/test_ecl_opcode_tables.cpp
    )

    target_link_libraries(th07-ecl-opcode-tests PRIVATE th07_game)

    add_test(NAME th07-ecl-opcodes COMMAND th07-ecl-opcode-tests)

    add_executable(th07-ecl-parser-tests
        tests/test_ecl_parser.cpp
    )

    target_link_libraries(th07-ecl-parser-tests PRIVATE th07_game)

    add_test(NAME th07-ecl-parser COMMAND th07-ecl-parser-tests)

    add_executable(th07-ecl-manager-contract-tests
        tests/test_ecl_manager_contract.cpp
    )

    target_link_libraries(th07-ecl-manager-contract-tests PRIVATE th07_game)

    add_test(NAME th07-ecl-manager-contract COMMAND th07-ecl-manager-contract-tests)

    add_executable(th07-resource-format-tests
        tests/test_resource_format_tables.cpp
    )

    target_link_libraries(th07-resource-format-tests PRIVATE th07_game)

    add_test(NAME th07-resource-formats COMMAND th07-resource-format-tests)

    add_executable(th07-rng-tests
        tests/test_rng.cpp
    )

    target_link_libraries(th07-rng-tests PRIVATE th07_game)

    add_test(NAME th07-rng COMMAND th07-rng-tests)

    add_executable(th07-timer-tests
        tests/test_timer.cpp
    )

    target_link_libraries(th07-timer-tests PRIVATE th07_game)

    add_test(NAME th07-timer COMMAND th07-timer-tests)

    add_executable(th07-score-tests
        tests/test_score_tables.cpp
    )

    target_link_libraries(th07-score-tests PRIVATE th07_game)

    add_test(NAME th07-score COMMAND th07-score-tests)

    add_executable(th07-clear-bonus-tests
        tests/test_clear_bonus.cpp
    )

    target_link_libraries(th07-clear-bonus-tests PRIVATE th07_game)

    add_test(NAME th07-clear-bonus COMMAND th07-clear-bonus-tests)

    add_executable(th07-cherry-border-tests
        tests/test_cherry_border.cpp
    )

    target_link_libraries(th07-cherry-border-tests PRIVATE th07_game)

    add_test(NAME th07-cherry-border COMMAND th07-cherry-border-tests)

    add_executable(th07-gui-layout-tests
        tests/test_gui_layout.cpp
    )

    target_link_libraries(th07-gui-layout-tests PRIVATE th07_game)

    add_test(NAME th07-gui-layout COMMAND th07-gui-layout-tests)

    add_executable(th07-gui-msg-loader-contract-tests
        tests/test_gui_msg_loader_contract.cpp
    )

    target_link_libraries(th07-gui-msg-loader-contract-tests PRIVATE th07_game)

    add_test(NAME th07-gui-msg-loader-contract COMMAND th07-gui-msg-loader-contract-tests)

    add_executable(th07-file-system-contract-tests
        tests/test_file_system_contract.cpp
    )

    target_link_libraries(th07-file-system-contract-tests PRIVATE th07_game)

    add_test(NAME th07-file-system-contract COMMAND th07-file-system-contract-tests)

    add_executable(th07-std-parser-tests
        tests/test_std_parser.cpp
    )

    target_link_libraries(th07-std-parser-tests PRIVATE th07_game)

    add_test(NAME th07-std-parser COMMAND th07-std-parser-tests)

    add_executable(th07-stage-loader-contract-tests
        tests/test_stage_loader_contract.cpp
    )

    target_link_libraries(th07-stage-loader-contract-tests PRIVATE th07_game)

    add_test(NAME th07-stage-loader-contract COMMAND th07-stage-loader-contract-tests)

    add_executable(th07-msg-parser-tests
        tests/test_msg_parser.cpp
    )

    target_link_libraries(th07-msg-parser-tests PRIVATE th07_game)

    add_test(NAME th07-msg-parser COMMAND th07-msg-parser-tests)

    add_executable(th07-anm-parser-tests
        tests/test_anm_parser.cpp
    )

    target_link_libraries(th07-anm-parser-tests PRIVATE th07_game)

    add_test(NAME th07-anm-parser COMMAND th07-anm-parser-tests)

    add_executable(th07-anm-manager-load-contract-tests
        tests/test_anm_manager_load_contract.cpp
    )

    target_link_libraries(th07-anm-manager-load-contract-tests PRIVATE th07_game)

    add_test(NAME th07-anm-manager-load-contract COMMAND th07-anm-manager-load-contract-tests)
endif()
