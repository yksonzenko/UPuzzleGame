// Grid

#pragma once

#include <MiniKit/MiniKit.hpp>
#include <vector>
#include <unordered_map>
#include "Tetromino.hpp"

class Game;

class GridDelegate;

struct alignas(16) GridBoxes
{
	GridBoxes& operator=(const GridBoxes& field);
	::MiniKit::Graphics::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
	::MiniKit::Graphics::float2 position{ 0.0f, 0.0f };
	::MiniKit::Graphics::float2 scale{ 1.0f, 1.0f };
	bool isOccupied{ false };
};

class Grid
{
public:
	Grid(::MiniKit::Graphics::int2 size);
	~Grid() = default;

	void drawGrid(::MiniKit::Graphics::CommandBuffer& commandBuf);
	void drawLockedTetromino(::MiniKit::Graphics::CommandBuffer& commandBuf);
	bool setTetrominoTexture(::MiniKit::Graphics::Device& graphics);
	void SetDelegate(GridDelegate* delegate);
	void update(const float& dTimer);
	void clearGrid();
	void gameOver();

	void clearLines();
	::std::vector<GridBoxes> getGrid() const;
	void lockBlock(int id, const ::std::vector<TetrominoBoxes>& block);

	bool checkIfOccupiedBlock(const ::std::vector<TetrominoBoxes>& block);
	GridBoxes* getOccupiedField(int x, int y);
	int getClearedLinesAmount() const;
	inline bool isLineToClear() const { return m_LinesMatched; }

private:
	void m_SetGridPositions();
	void m_CheckCompleteLines();
	void m_MovePosYDown();
	::MiniKit::Graphics::int2 m_ConvertCartesianToLocal(float x, float y);

	GridDelegate* m_Delegate{ nullptr };

	::std::unique_ptr<::MiniKit::Graphics::Image> m_TetrominoTexture{ nullptr };
	::std::vector<GridBoxes> m_GridRect;
	::std::vector<GridBoxes> m_CurLockedBlock;

	::std::unordered_map<int, ::std::unique_ptr<GridBoxes>> m_OccupiedFieldBlocks;

	bool m_LinesMatched{ false };
	int m_TShapeID{ 0 };
	int m_ClearedLinesCounter{ 0 };
	::MiniKit::Graphics::int2 m_GridSize;
	::std::vector<int> m_LinesToClear{ 0 };
};


class GridDelegate
{
public:
	virtual void onClearLine(Grid& grid) = 0;
	virtual void onGameOver(Grid& grid) = 0;
};
