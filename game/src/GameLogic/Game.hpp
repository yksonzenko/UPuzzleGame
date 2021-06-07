// Game

#pragma once

#include <MiniKit/MiniKit.hpp>
#include <iostream>
#include <thread>
#include "Grid.hpp"
#include "Tetromino.hpp"
#include "Directions.hpp"
#include "PauseResumeState.hpp"
#include "Score.hpp"
#include "GameHUD.hpp"

class Game final : public ::MiniKit::Engine::Application, public ::MiniKit::Platform::Responder, public GridDelegate
{
	enum class GameState
	{
		Started,
		GameOver,
		Paused,
		Resumed
	};

public:
	Game();
	~Game() = default;

	::std::error_code Start(::MiniKit::Engine::Context& context) noexcept override;

	::std::error_code Shutdown(::MiniKit::Engine::Context& context) noexcept override;

	void Tick(::MiniKit::Engine::Context& context) noexcept override;

	void KeyDown(::MiniKit::Platform::Window& window, const ::MiniKit::Platform::KeyEvent& event) noexcept override;

	void KeyUp(::MiniKit::Platform::Window& window, const ::MiniKit::Platform::KeyEvent& event) noexcept override;

private:

	GameState m_CurState;

	bool m_LoadTexture(::MiniKit::Engine::Context& context);
	void m_Rotate();
	void m_Update(const float& dTimer);
	void m_NewGame();

	int getRandBlockID(int min, int max);
	void createTetromino();
	void m_InputController(Directions direct);
	bool m_CheckNoCollision(const ::std::vector<TetrominoBoxes>& block);

	void onClearLine(Grid& grid) override;
	void onGameOver(Grid& grid) override;
	void m_DrawGameOverScreen(::MiniKit::Graphics::CommandBuffer& commandBuf);
	void m_DrawPausedScreen(::MiniKit::Graphics::CommandBuffer& commandBuf);

	::std::unique_ptr<PauseResume> m_GameTimer{ nullptr };
	::std::unique_ptr<Score> m_GameScore{ nullptr };
	::std::unique_ptr<Grid> m_Grid{ nullptr };
	::std::unique_ptr<Tetromino> m_Tetromino{ nullptr };
	::std::unique_ptr<GameHUD> m_GameHUD{ nullptr };


	::std::unique_ptr<::MiniKit::Graphics::Image> m_GridTexture{ nullptr };
	::std::unique_ptr<::MiniKit::Graphics::Image> m_TetrominoTexture{ nullptr };
	::std::unique_ptr<::MiniKit::Graphics::Image> m_GameOverBackground{ nullptr };
	::std::unique_ptr<::MiniKit::Graphics::Image> m_PausedTexture{ nullptr };

	float m_x{ 0.0f };
	float m_y{ 0.0f };
	float m_ElapsedTime{ 0.0f };
	float m_DelayTime{ 0.0f };
	float m_Timer{ 0.0f };
	float m_DeltaTime{ 0.0f };
	int m_TShapeID{ 0 };
	bool m_pOpen{ false };
};
