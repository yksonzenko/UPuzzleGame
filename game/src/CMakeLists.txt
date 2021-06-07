cmake_minimum_required(VERSION 3.17)

if(APPLE)
    set(GameSources
        GameLogic/Game.cpp
        GameLogic/Game.hpp
        GameLogic/Grid.cpp
        GameLogic/Grid.hpp
        GameLogic/Tetromino.cpp
        GameLogic/Tetromino.hpp
        GameLogic/Score.cpp
        GameLogic/Score.hpp
        GameLogic/PauseResumeState.cpp
        GameLogic/PauseResumeState.hpp
        GameLogic/GameHUD.cpp
        GameLogic/GameHUD.hpp
    )
    add_executable(puzzle main.cpp ${GameSources})

else()
    set(APP_ICON_RESOURCE "${CMAKE_SOURCE_DIR}/game/assets/images/tetris_icon.rc")

    set(GameSources
        GameLogic/Game.cpp
        GameLogic/Game.hpp
        GameLogic/Grid.cpp
        GameLogic/Grid.hpp
        GameLogic/Tetromino.cpp
        GameLogic/Tetromino.hpp
        GameLogic/Score.cpp
        GameLogic/Score.hpp
        GameLogic/PauseResumeState.cpp
        GameLogic/PauseResumeState.hpp
        GameLogic/GameHUD.cpp
        GameLogic/GameHUD.hpp
    )

    add_executable(puzzle main.cpp ${APP_ICON_RESOURCE} ${GameSources})
endif()

target_link_libraries(puzzle PRIVATE MiniKit::MiniKit)

add_custom_command(TARGET puzzle POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/game/assets $<TARGET_FILE_DIR:puzzle>/assets)


