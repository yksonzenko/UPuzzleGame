#include "Grid.hpp"

GridBoxes& GridBoxes::operator=(const GridBoxes& field) {
    this->isOccupied = field.isOccupied;
    this->position.x = field.position.x;
    this->position.y = field.position.y;
    return *this;
}

Grid::Grid(::MiniKit::Graphics::int2 size)
    : m_GridSize(size), m_LinesToClear()
{
    m_SetGridPositions();
}

bool Grid::setTetrominoTexture(::MiniKit::Graphics::Device& graphics)
{
    return (m_TetrominoTexture = graphics.CreateImage("assets/images/spritesheet_224x32px.png")) ? true : false;
}

void Grid::m_SetGridPositions()
{
    const auto numTiles = m_GridSize.x * m_GridSize.y;
    const ::MiniKit::Graphics::float2 imageSize = { 32.0f, 32.0f };

    const auto imageScaleX = 1.0f;
    const auto imageScaleY = 1.0f;

    const auto anchorPositionX = -0.5f * imageScaleX * imageSize.width * (m_GridSize.x - 1u); // -144
    const auto anchorPositionY = -0.5f * imageScaleY * imageSize.height * (m_GridSize.y - 1u); // -304

    m_GridRect.resize(numTiles);
    m_OccupiedFieldBlocks.reserve(numTiles);

    for (auto x = 0u; x < m_GridSize.x; ++x)
    {
        for (auto y = 0u; y < m_GridSize.y; ++y) {
            auto& entity = m_GridRect[y * m_GridSize.x + x];
            entity.scale = { imageScaleX, imageScaleY };
            entity.position.x = anchorPositionX + imageScaleX * imageSize.width * x;
            entity.position.y = anchorPositionY + imageScaleY * imageSize.height * y;

            m_OccupiedFieldBlocks[y * m_GridSize.x + x] = ::std::make_unique<GridBoxes>();
        }
    }
};

void Grid::drawGrid(::MiniKit::Graphics::CommandBuffer& commandBuf)
{
    ::MiniKit::Graphics::DrawInfo drawGrid{};
    for (const auto& grid : m_GridRect)
    {
        drawGrid.scale = grid.scale;
        drawGrid.position = grid.position;
        commandBuf.Draw(drawGrid);
    }
}

void Grid::drawLockedTetromino(::MiniKit::Graphics::CommandBuffer& commandBuf)
{
    commandBuf.SetImage(*m_TetrominoTexture);
    ::MiniKit::Graphics::Rect2D<uint32_t> blockTexture(::MiniKit::Graphics::Rect2D<uint32_t>{ ((uint32_t)m_TShapeID % 7) * 32, 0, 32, 32 });
    for (auto x = 0; x < m_GridSize.x; ++x)
    {
        for (auto y = 0; y < m_GridSize.y; ++y)
        {
            auto field = getOccupiedField(x, y);
            if (field->isOccupied == true)
            {
                ::MiniKit::Graphics::DrawInfo drawOccupiedBlock{};
                drawOccupiedBlock.scale = { 0.143f, 1.0f };
                drawOccupiedBlock.position.x = field->position.x * 32;
                drawOccupiedBlock.position.y = field->position.y * 32;
                commandBuf.Draw(drawOccupiedBlock, blockTexture);
            }
        }
    }
}

::std::vector<GridBoxes> Grid::getGrid() const { return m_GridRect; }

GridBoxes* Grid::getOccupiedField(int x, int y) {
    return m_OccupiedFieldBlocks[y * m_GridSize.x + x].get();
}

::MiniKit::Graphics::int2 Grid::m_ConvertCartesianToLocal(float x, float y)
{
    int posX = static_cast<int>(x + 4.5f);
    int posY = static_cast<int>(y + 9.5f);
    return ::MiniKit::Graphics::int2{ posX, posY };
}

void Grid::SetDelegate(GridDelegate* delegate) { m_Delegate = delegate; }

void Grid::lockBlock(int id, const ::std::vector<TetrominoBoxes>& block) {
    m_CurLockedBlock.resize(4);
    for (int i = 0; i < 4; ++i) {
        int2 grid = m_ConvertCartesianToLocal(block[i].position.x, block[i].position.y);

        auto field = getOccupiedField(grid.x, grid.y);
        field->isOccupied = true;
        field->position.x = block[i].position.x;
        field->position.y = block[i].position.y;

        m_CurLockedBlock[i].position.x = block[i].position.x;
        m_CurLockedBlock[i].position.y = block[i].position.y;

    }
    m_TShapeID = id;
}

bool Grid::checkIfOccupiedBlock(const ::std::vector<TetrominoBoxes>& block)
{
    for (int i = 0; i < 4; ++i) {
        int2 grid = m_ConvertCartesianToLocal(block[i].position.x, block[i].position.y);
        auto field = getOccupiedField(grid.x, grid.y);
        if (field->isOccupied == true)
        {
            return true;
        }
    }
    return false;
}

void Grid::update(const float& dTimer)
{
    m_CheckCompleteLines();
    if (m_LinesMatched)
    {
        clearLines();
        if (m_Delegate)
        {
            m_Delegate->onClearLine(*this);
            m_ClearedLinesCounter = 0;
        }
    }
}

void Grid::gameOver()
{
    if (m_Delegate)
    {
        m_Delegate->onGameOver(*this);
    }
}

void Grid::clearGrid()
{
    for (int x = 0; x < m_GridSize.x; ++x) {
        for (int y = 0; y < m_GridSize.y; ++y) {
            auto field = getOccupiedField(x, y);
            field->isOccupied = false;
            field->position = { 0.0f, 0.0f };
        }
    }
}

void Grid::m_CheckCompleteLines()
{
    if (m_LinesMatched) return;
    for (auto y = 0; y < m_GridSize.y; ++y)
    {
        int countBlocks = 0;
        for (auto x = 0; x < m_GridSize.x; ++x)
        {
            auto* grid = getOccupiedField(x, y);
            if (grid->isOccupied)
            {
                countBlocks++;
            }
            if (countBlocks == 10)
            {
                m_LinesMatched = true;
                m_LinesToClear.push_back(y);
                ++m_ClearedLinesCounter;
            }
        }
    }
    // to ensure that we will clear lines from top to bottom 
    std::sort(m_LinesToClear.begin(), m_LinesToClear.end(), [](int a, int b) { return a < b; });
}

void Grid::m_MovePosYDown()
{
    for (auto row = m_LinesToClear[0]; row < m_GridSize.y; ++row)
    {
        for (auto col = 0; col < m_GridSize.x; ++col)
        {
            auto* gr = getOccupiedField(col, row);
            if (gr->isOccupied == true)
            {
                gr->position.y -= 1.0f;
            }
        }
    }
}

void Grid::clearLines() {
    if (m_LinesToClear.empty()) return;

    for (auto lines : m_LinesToClear)
    {
        for (auto y = m_LinesToClear[0]; y < m_GridSize.y; ++y)
        {
            for (auto x = 0; x < m_GridSize.x; ++x)
            {
                int overLine = y + 1;
                if (overLine < m_GridSize.y)
                {
                    *getOccupiedField(x, y) = *getOccupiedField(x, overLine);
                }
            }
        }
        m_MovePosYDown();
    }
    m_LinesToClear.clear();
    m_LinesMatched = false;
}

int Grid::getClearedLinesAmount() const { return m_ClearedLinesCounter; }
