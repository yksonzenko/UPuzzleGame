// Tetromino

#pragma once

#include <vector>
#include <MiniKit/MiniKit.hpp>
#include <random>
#include <iostream>
#include "Directions.hpp"

using namespace ::MiniKit::Graphics;

struct alignas(16) TetrominoBoxes
{
    ::MiniKit::Graphics::Color color{ 1.0f, 1.0f, 1.0f, 1.0f };
    ::MiniKit::Graphics::float2 position{ 0.0f, 0.0f };
    ::MiniKit::Graphics::float2 scale{ 1.0f, 1.0f };
};

class Tetromino
{
public:
    Tetromino(int id);
    ~Tetromino() = default;
     
    void rotate();
    void setPosition(const ::MiniKit::Graphics::float2& position);
    void revertState();
    int getShapeID() const;
    void inputHandler(Directions direct);
    ::std::vector<float2> getTetrominoShape() const;
    ::std::vector<TetrominoBoxes> getTetrominoPos() const;
    ::std::vector<TetrominoBoxes> getFutureTetrominoPos(Directions direct) const;
    void getToDrawTetromino(::MiniKit::Graphics::CommandBuffer& commandBuf);
    void offsetRotation();

private:
    int m_BlockShapeID;
    int m_CurrentRotation;

    ::std::vector<TetrominoBoxes> m_Tetromino;
    ::std::unique_ptr<::MiniKit::Graphics::Image> m_TetrominoTexture{ nullptr };

    ::MiniKit::Graphics::Rect2D<uint32_t> m_BlockTexture;

    ::std::vector<float2> m_TetrominoShape;
    ::std::vector<float2> m_OldTetrominoShape;
    float2 m_TetrominoPos{ 0.0f, 0.0f };
};
