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

// Tests the Engine::EngineOption constructors.
using EngineOption = Engine::EngineOption;
TEST(EngineTests, EngineOptionConstruction)
{
    {
        EngineOption engineOption;
        EXPECT_TRUE(!engineOption.getConstraint().has_value());
    }
    {
        EngineOption::RangeConstraint constraint(0, 100);
        EngineOption engineOption(constraint);
        EXPECT_TRUE(engineOption.getConstraint().has_value());
        EXPECT_TRUE(std::get<EngineOption::RangeConstraint>(engineOption.getConstraint().value()) == constraint);
    }
    {
        EngineOption::EnumConstraint constraint{ "Hello", "World", "!" };
        EngineOption engineOption(constraint);
        EXPECT_TRUE(engineOption.getConstraint().has_value());
        EXPECT_TRUE(std::get<EngineOption::EnumConstraint>(engineOption.getConstraint().value()) == constraint);
    }
}

// Tests isWithinConstraint() for Engine::EngineOption::RangeConstraint.
TEST(EngineTests, RangeConstraint)
{
    const EngineOption::RangeConstraint constraint(0, 100);
    EngineOption engineOption(constraint);
    const std::pair<int, bool> queries[] =
            {
                    {40, true}, {1, true}, {0, true}, {-1, false}, {-50, false},
                    {99, true}, {100, true}, {101, false}, {154, false}
            };
    for(const auto&[val, exp] : queries)
    {
        EXPECT_TRUE(EngineOption::isWithinConstraint(val, constraint) == exp);
    }
}

// Tests isWithinConstraint() for Engine::EngineOption::EnumConstraint.
TEST(EngineTests, EnumConstraint)
{
    const EngineOption::EnumConstraint constraint{"A", "B", "C", "IF"};
    EngineOption engineOption(constraint);
    const std::pair<std::string, bool> queries[] =
            {
                    {"A", true}, {"B", true}, {"C", true}, {"D", false}, {"AB", false}, {"I", false}, {"", false}
            };
    for(const auto&[val, exp] : queries)
    {
        EXPECT_TRUE(EngineOption::isWithinConstraint(val, constraint) == exp);
    }
}

// Tests isWithinConstraint() for no constraint.
TEST(EngineTests, NoConstraint)
{
    EngineOption engineOption;
    EXPECT_TRUE(EngineOption::isWithinConstraint(5, engineOption.getConstraint()));
    EXPECT_TRUE(EngineOption::isWithinConstraint("Tu manges une orange.", engineOption.getConstraint()));
}



