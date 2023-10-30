#include "../h/Engine.h"
#include <gtest/gtest.h>

using namespace eugenchess::engine;
using Move = Engine::Move;

// Tests whether the Engine::Move constructor parses the algebraic notation correctly.
TEST(EngineTests, MoveConstructionFromAlgebraic)
{
    {
        Move move("a1a1");
        EXPECT_TRUE(move.from.x == 0);
        EXPECT_TRUE(move.from.y == 0);
        EXPECT_TRUE(move.to.x == 0);
        EXPECT_TRUE(move.to.y == 0);
    }
    {
        Move move("h8h8");
        EXPECT_TRUE(move.from.x == 7);
        EXPECT_TRUE(move.from.y == 7);
        EXPECT_TRUE(move.to.x == 7);
        EXPECT_TRUE(move.to.y == 7);
    }
    {
        Move move("e2e4");
        EXPECT_TRUE(move.from.x == 4);
        EXPECT_TRUE(move.from.y == 1);
        EXPECT_TRUE(move.to.x == 4);
        EXPECT_TRUE(move.to.y == 3);
    }
    {
        Move move("d5g2");
        EXPECT_TRUE(move.from.x == 3);
        EXPECT_TRUE(move.from.y == 4);
        EXPECT_TRUE(move.to.x == 6);
        EXPECT_TRUE(move.to.y == 1);
    }
}

// Tests whether printing Engine::Move yields correct algebraic notation.
#include <sstream>
TEST(EngineTests, MovePrint)
{
    {
        Move move("d2d4");
        std::ostringstream stream;
        stream << move;
        EXPECT_TRUE(stream.str() == "d2d4");
    }
    {
        Move move("h1a4");
        std::ostringstream stream;
        stream << move;
        EXPECT_TRUE(stream.str() == "h1a4");
    }
}

// Tests whether printing Engine::MaybeMove yields correct algebraic notation.
using MaybeMove = Engine::Move::MaybeMove;
TEST(EngineTests, MaybeMovePrint)
{
    {
        MaybeMove maybeMove("a4a6");
        std::ostringstream stream;
        stream << maybeMove;
        EXPECT_TRUE(stream.str() == "a4a6");
    }
    {
        MaybeMove maybeMove;
        std::ostringstream stream;
        stream << maybeMove;
        EXPECT_TRUE(stream.str() == "0000");
    }
}


