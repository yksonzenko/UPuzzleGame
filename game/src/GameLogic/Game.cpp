// Game

#include "Game.hpp"
#include "Directions.hpp"
#include <SDL.h>
#include <iomanip>

using namespace std::chrono_literals;

std::default_random_engine engine{ static_cast<unsigned int>(
    std::chrono::system_clock::now().time_since_epoch().count()) };

int Game::getRandBlockID(int min, int max)
{
    std::uniform_int_distribution<int> int_distribution(min, max);
    return int_distribution(engine);
}

Game::Game() : m_CurState(GameState::Started), m_GameTimer(), m_GameScore(), m_Grid(), m_Tetromino(), m_GameHUD(), m_DelayTime(0.5f), m_TShapeID(getRandBlockID(0, 4)) // first possible shapes 'I' 'L' 'J' 'O' 'T'
{
    m_GameTimer.reset(new PauseResume);
    m_GameScore.reset(new Score);
    m_GameHUD.reset(new GameHUD);
    m_Grid = ::std::make_unique<Grid>(::MiniKit::Graphics::int2{ 10, 20 });
    createTetromino();
    m_Grid->SetDelegate(this);
}


::std::error_code Game::Start(::MiniKit::Engine::Context& context) noexcept
{
    auto& window = context.GetWindow();
    window.AddResponder(*this);
    m_pOpen = true;

    if (m_LoadTexture(context) == false)
    {
        return ::MiniKit::MakeErrorCode(::MiniKit::Error::InitializationFailed);
    }
    return {};
};

::std::error_code Game::Shutdown(::MiniKit::Engine::Context& context) noexcept
{
    auto& window = context.GetWindow();
    window.RemoveResponder(*this);
    m_pOpen = false;

    return {};
};

void Game::Tick(::MiniKit::Engine::Context& context) noexcept
{
    if (m_CurState == GameState::Started)
    {
        SDL_Delay(120);
        const auto deltaTime = context.GetFrameDelta();
        m_DeltaTime = context.GetFrameDelta();
        m_GameTimer->start(deltaTime);
        m_ElapsedTime += deltaTime;

        auto& graphicsDevice = context.GetGraphicsDevice();
        Device& graphicsDevice2 = context.GetGraphicsDevice();

        if (m_ElapsedTime > m_DelayTime && m_CurState == GameState::Started)
        {
            m_ElapsedTime = 0;
            m_InputController(Directions::Down);
        }
        m_Update(m_DeltaTime);

        // Rendering
        auto& commandBuffer = graphicsDevice.BeginFrame(0.14f, 0.14f, 0.14f, 0.14f); // 95% Gray Color #111111
        {
            commandBuffer.SetImage(*m_GridTexture);
            m_Grid->drawGrid(commandBuffer);
            m_Grid->drawLockedTetromino(commandBuffer);

            if (m_Tetromino && m_CurState == GameState::Started)
            {
                commandBuffer.SetImage(*m_TetrominoTexture);
                m_Tetromino->getToDrawTetromino(commandBuffer);
            }
            if (m_CurState == GameState::GameOver)
            {
                SDL_Delay(700);
                m_DrawGameOverScreen(commandBuffer);
            }
            if (m_GameTimer->isPaused() && m_CurState == GameState::Started)
            {
                m_CurState = GameState::Paused;
                m_DrawPausedScreen(commandBuffer);
            }
        }
        m_GameHUD->drawGameInfoWindow(&m_pOpen);

        graphicsDevice.EndFrame(commandBuffer);
    }

}

void Game::KeyDown(::MiniKit::Platform::Window& window, const ::MiniKit::Platform::KeyEvent& event) noexcept
{
    if (event.keycode == ::MiniKit::Platform::Keycode::KeyEscape)
    {
        window.GetContext().Terminate();
    }
    if (event.keycode == ::MiniKit::Platform::Keycode::KeyP)
    {
        m_GameTimer->paused();
    }

    if (event.keycode == ::MiniKit::Platform::Keycode::KeyR)
    {
        m_CurState = GameState::Started;
        m_GameTimer->resumed();
    }

    if (event.keycode == ::MiniKit::Platform::Keycode::KeyN)
    {
        m_NewGame();
    }

    if (m_CurState == GameState::Started)
    {
        if (event.keycode == ::MiniKit::Platform::Keycode::KeyLeft)
        {
            m_InputController(Directions::Left);
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyRight)
        {
            m_InputController(Directions::Right);
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyUp)
        {
            m_Rotate();
        }

        if (event.keycode == ::MiniKit::Platform::Keycode::KeyDown)
        {
            m_InputController(Directions::StepDown);
        }
    }
}

void Game::KeyUp(::MiniKit::Platform::Window& window, const ::MiniKit::Platform::KeyEvent& event) noexcept
{
    if (event.keycode == ::MiniKit::Platform::Keycode::KeyLeft)
    {
        m_x -= 0.0f;
    }

    if (event.keycode == ::MiniKit::Platform::Keycode::KeyRight)
    {
        m_x += 0.0f;
    }

    if (event.keycode == ::MiniKit::Platform::Keycode::KeyUp)
    {
        m_y += 0.0f;
    }

    if (event.keycode == ::MiniKit::Platform::Keycode::KeyDown)
    {
        m_y -= 0.0f;
    }
}

bool Game::m_LoadTexture(::MiniKit::Engine::Context& context)
{
    auto& graphicsDevice = context.GetGraphicsDevice();

    m_GridTexture = graphicsDevice.CreateImage("assets/images/grid.png");

    if (!m_GridTexture)
    {
        return false;
    }

    m_TetrominoTexture = graphicsDevice.CreateImage("assets/images/spritesheet_224x32px.png");

    if (!m_TetrominoTexture)
    {
        return false;
    }

    m_GameOverBackground = graphicsDevice.CreateImage("assets/images/game_over.png");

    if (!m_GameOverBackground)
    {
        return false;
    }

    m_PausedTexture = graphicsDevice.CreateImage("assets/images/paused.png");

    if (!m_PausedTexture)
    {
        return false;
    }

    if (!m_Grid->setTetrominoTexture(graphicsDevice)) return false;

    return true;
}

void Game::onClearLine(Grid& grid)
{
    m_GameScore->addClearedLinesScore(grid.getClearedLinesAmount());
    if (m_GameScore->getLines() % 10 == 0)
    {
        m_DelayTime -= 0.05f;
    }
}

void Game::onGameOver(Grid& grid)
{
    m_GameTimer->stop();
    m_CurState = GameState::GameOver;
}

void Game::m_DrawGameOverScreen(::MiniKit::Graphics::CommandBuffer& commandBuf)
{
    commandBuf.SetImage(*m_GameOverBackground);
    ::MiniKit::Graphics::DrawInfo drawGameOverScreen{};
    drawGameOverScreen.scale = { 0.7f, 0.7f };
    commandBuf.Draw(drawGameOverScreen);
}

void Game::m_DrawPausedScreen(::MiniKit::Graphics::CommandBuffer& commandBuf)
{
    commandBuf.SetImage(*m_PausedTexture);
    ::MiniKit::Graphics::DrawInfo drawPausedScreen{};
    drawPausedScreen.scale = { 0.5f, 0.5f };
    commandBuf.Draw(drawPausedScreen);
}

void Game::m_NewGame()
{
    m_CurState = GameState::Started;
    m_GameTimer->stop();
    m_Grid->clearGrid();
    m_GameScore->resetScore();
    m_DelayTime = 0.5f;
    m_TShapeID = getRandBlockID(0, 4);
    createTetromino();
    m_GameTimer->resetTimer();
}

void Game::createTetromino() {
    m_Tetromino.reset(new Tetromino{ m_TShapeID });
    if (m_Grid->checkIfOccupiedBlock(m_Tetromino->getTetrominoPos())) {
        m_Grid->gameOver();
    }
    m_TShapeID = getRandBlockID(0, 6);
}

void Game::m_InputController(Directions direct)
{
    if (!m_Tetromino) return;
    if (m_CheckNoCollision(m_Tetromino->getFutureTetrominoPos(direct)))
    {
        m_Tetromino->inputHandler(direct);
        if (direct == Directions::StepDown)
        {
            m_GameScore->addBonusScore(1);
        }
    }
    else
    {
        if (direct == Directions::Down || direct == Directions::StepDown)
        {
            m_Timer += m_DeltaTime;
            if (m_Timer > 0.3f)
            {
                m_Timer = 0;
                m_Grid->lockBlock(m_Tetromino->getShapeID(), m_Tetromino->getTetrominoPos());
                m_Tetromino.reset(nullptr);
            }
        }
    }
}

bool Game::m_CheckNoCollision(const ::std::vector<TetrominoBoxes>& block) {
    for (int i = 0; i < 4; ++i) {
        if (block[i].position.x < -4.5f || block[i].position.x > 4.5f || block[i].position.y < -9.5f)
        {
            return false;
        }
    }
    if (m_Grid->checkIfOccupiedBlock(block))
    {
        return false;
    }
    return true;
}

void Game::m_Rotate()
{
    if (!m_Tetromino) return;

    m_Tetromino->rotate();
    if (!m_CheckNoCollision(m_Tetromino->getTetrominoPos())) {
        m_Tetromino->revertState();
    }
}

void Game::m_Update(const float& dTimer) {
    m_Grid->update(dTimer);

    m_GameHUD->setGameInfo(m_GameTimer->getTimer(), m_GameScore->getScore(), m_GameScore->getLevel(), m_GameScore->getLines());

    if (!m_Tetromino)
    {
        if (m_Grid->isLineToClear()) {
            return;
        }
        createTetromino();
    }
}
