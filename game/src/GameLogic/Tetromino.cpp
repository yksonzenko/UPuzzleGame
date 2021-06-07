#include "Tetromino.hpp"

unsigned int tetrominoInfo[7][4] = {
    {4, 5, 6, 7}, // 'I' 0
    {2, 4, 5, 6}, // 'L' 1
    {0, 4, 5, 6}, // 'J' 2
    {1, 2, 5, 6}, // 'O' 3
    {1, 4, 5, 6}, // 'T' 4
    {0, 1, 5, 6}, // 'Z' 5
    {1, 2, 4, 5} // 'S' 6
};

Tetromino::Tetromino(int id)
    : m_BlockShapeID(id), m_TetrominoPos({ -1.5f, 9.5f }), m_CurrentRotation(0), m_TetrominoShape(),
    m_BlockTexture(::MiniKit::Graphics::Rect2D<uint32_t>{ ((uint32_t)id % 7) * 32, 0, 32, 32 })
{
	m_BlockShapeID = m_BlockShapeID % 7; // not to get out of bounds
	m_TetrominoShape.resize(4);
	for (auto i = 0; i < 4; ++i) {
		auto& s = m_TetrominoShape[i];
		s.x = tetrominoInfo[m_BlockShapeID][i] % 4;
		s.y = tetrominoInfo[m_BlockShapeID][i] / 4;
	}
}

::std::vector<TetrominoBoxes> Tetromino::getTetrominoPos() const
{
    ::std::vector<TetrominoBoxes> blockPositions(4);
    for (auto i = 0; i < 4; ++i) {
        blockPositions[i].position = { m_TetrominoShape[i].x + m_TetrominoPos.x, m_TetrominoPos.y - m_TetrominoShape[i].y};
    }
    return blockPositions;
}

::std::vector<TetrominoBoxes> Tetromino::getFutureTetrominoPos(Directions direct) const
{
    ::std::vector<TetrominoBoxes> blockPositions(4);
    ::MiniKit::Graphics::float2 tempPosition{ m_TetrominoPos };
    if (direct == Directions::Left)
    {
        tempPosition.x -= 1.0f;
    }
    if (direct == Directions::Right)
    {
        tempPosition.x += 1.0f;
    }
    else
    {
        tempPosition.y -= 1.0f;
    }
    for (auto i = 0; i < 4; ++i)
    {
        blockPositions[i].position = { m_TetrominoShape[i].x + tempPosition.x, tempPosition.y - m_TetrominoShape[i].y };
    }

    return blockPositions;
}

::std::vector<float2> Tetromino::getTetrominoShape() const
{
    return m_TetrominoShape;
}

void Tetromino::getToDrawTetromino(::MiniKit::Graphics::CommandBuffer& commandBuf)
{
    const auto& pos = getTetrominoPos();
    m_Tetromino.resize(4);
    ::MiniKit::Graphics::DrawInfo drawTetromino{};
    for (auto i = 0; i < 4; ++i)
    {
        auto& entity = m_Tetromino[i];
        entity.position.x = (pos[i].position.x * 32);
        entity.position.y = (pos[i].position.y * 32);
        entity.scale = { 0.143f, 1.0f };
    }

    for (const auto& tet : m_Tetromino)
    {
        drawTetromino.scale = tet.scale;
        drawTetromino.position = tet.position;
        commandBuf.Draw(drawTetromino, m_BlockTexture);
    }
}

void Tetromino::revertState()
{
    m_TetrominoShape = m_OldTetrominoShape;
}

int Tetromino::getShapeID() const
{
    return m_BlockShapeID;
}

void Tetromino::setPosition(const ::MiniKit::Graphics::float2& position)
{
    m_TetrominoPos = position;
}

void Tetromino::inputHandler(Directions direct)
{
    if (direct == Directions::Left)
    {
        m_TetrominoPos.x -= 1.0f;
    }

    if (direct == Directions::Right)
    {
        m_TetrominoPos.x += 1.0f;
    }

	else
    {
        m_TetrominoPos.y -= 1.0f;
    }
}

void Tetromino::rotate()
{
    //store state of Block in case rotation turns out to be invalid
    m_OldTetrominoShape = m_TetrominoShape;
    m_CurrentRotation++;

    if (m_BlockShapeID == 3) { //square: no need for rotation
        return;
    }
    if (m_BlockShapeID == 0) { // rotation of I
        for (auto i = 0; i < 4; ++i) {
            float2 oldPoint = m_TetrominoShape[i];    //pivot
            float2 localVector{ 0.0f, 0.0f };
            float2 nextPoint{};
            if (m_CurrentRotation % 4 == 1)
            {
                localVector = { oldPoint.x - 2, oldPoint.y - 1 };
                nextPoint = { (0 * localVector.x) + (1 * localVector.y),
                    (-1 * localVector.x) + (0 * localVector.y) };
                m_TetrominoShape[i] = { nextPoint.x + 2, nextPoint.y + 1 };
            }
            else if (m_CurrentRotation % 4 == 2)
            {
                localVector = { oldPoint.x - 2, oldPoint.y - 2 };
                nextPoint = { (0 * localVector.x) + (1 * localVector.y),
                    (-1 * localVector.x) + (0 * localVector.y) };
                m_TetrominoShape[i] = { nextPoint.x + 2, nextPoint.y + 2 };
            }
            else if (m_CurrentRotation % 4 == 3)
            {
                localVector = { oldPoint.x - 1, oldPoint.y - 2 };
                nextPoint = { (0 * localVector.x) + (1 * localVector.y),
                    (-1 * localVector.x) + (0 * localVector.y) };
                m_TetrominoShape[i] = { nextPoint.x + 1, nextPoint.y + 2 };
            }
            else if (m_CurrentRotation % 4 == 0)
            {
                localVector = { oldPoint.x - 1, oldPoint.y - 1 };
                nextPoint = { (0 * localVector.x) + (1 * localVector.y),
                    (-1 * localVector.x) + (0 * localVector.y) };
                m_TetrominoShape[i] = { nextPoint.x + 1, nextPoint.y + 1 };
            }
            offsetRotation();
        }
        return;
    }
    for (auto i = 0; i < 4; ++i) {
        float2 oldPoint = m_TetrominoShape[i];    //pivot
        float2 localVector = { oldPoint.x - 1, oldPoint.y - 1 };

        float2 nextPoint{ (0 * localVector.x) + (-1 * localVector.y),
                        (1 * localVector.x) + (0 * localVector.y) };

        m_TetrominoShape[i] = { nextPoint.x + 1, nextPoint.y + 1 };
    }
    offsetRotation();
}

void Tetromino::offsetRotation()
{
	if (m_BlockShapeID == 0)
	{
        if (m_TetrominoPos.x == -5.5f)
        {
            m_TetrominoPos.x += 1.0f;
        }
        if (m_TetrominoPos.x == -6.5f)
        {
            m_TetrominoPos.x += 2.0f;
        }
        if (m_TetrominoPos.x == 2.5f)
        {
            m_TetrominoPos.x -= 1.0f;
        }
        if (m_TetrominoPos.x == 3.5f)
        {
            m_TetrominoPos.x -= 2;
        }
        if (m_TetrominoPos.y == -7.5f)
        {
            m_TetrominoPos.y += 1.0f;
        }
        if (m_TetrominoPos.y == -8.5f)
        {
            m_TetrominoPos.y += 2.0f;
        }
	}
    else
    {
        if (m_TetrominoPos.x < -4.5)
        {
            m_TetrominoPos.x += 1;
        }
        if (m_TetrominoPos.x > 2.5)
        {
            m_TetrominoPos.x -= 1;
        }
    }
}
