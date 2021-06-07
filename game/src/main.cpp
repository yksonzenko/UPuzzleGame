#include <MiniKit/MiniKit.hpp>
#include "GameLogic/Game.hpp"

int main(int argc, char** argv)
{
    Game example;

    ::MiniKit::Engine::StartupInfo startupInfo{};

    startupInfo.window.title = "Tetris";
    startupInfo.window.width = 992;
    startupInfo.window.height = 768;

    return ::MiniKit::ApplicationMain(startupInfo, example);
}
