#include "../../h/uci/UCIUtility.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace eugenchess::uci::implementation;
using namespace eugenchess::uci;
using namespace eugenchess::engine;

namespace
{
    class TestEngine : public Engine
    {
    public:
        void setPosition(std::string_view FEN) override
        {
            fen = FEN;
        }
        [[nodiscard]] std::string getFEN() const override
        {
            return fen;
        }
        void setClock(Clock clock) override
        {
        }
        void playMove(Move move) override
        {
            playedMoves.push_back(move);
        }
        Move calculateBestMove() override
        {
            return {};
        }
        void ponderMove(Move move) override
        {
        }
        Move ponderHit() override
        {
            return {};
        }
        void setProtocol(std::string_view name) override
        {
        }
        EngineOptions& options() override
        {
            throw std::exception();
        }
        void performCommand(std::string_view name) override
        {
        }
        std::vector<std::string> commands() override
        {
            return {};
        }
        std::vector<Move> playedMoves;
        std::string fen;
    };
}

TEST(PositionsTests, StartPos)
{
    auto engine = TestEngine();
    std::string line;
    std::istringstream ss("startpos");
    std::ostringstream out;
    UCIUtility::positionHandler(engine, ss, out);
    EXPECT_EQ(engine.fen, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST(PositionsTests, SomePos)
{
    auto engine = TestEngine();
    std::string line;
    std::istringstream ss("fen rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    std::ostringstream out;
    UCIUtility::positionHandler(engine, ss, out);
    EXPECT_EQ(engine.fen, "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}

TEST(PositionsTests, SomePosWithMoves)
{
    auto engine = TestEngine();
    std::string line;
    std::istringstream ss("fen rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1 moves a1b2 c4b6 a2g5");
    std::ostringstream out;
    UCIUtility::positionHandler(engine, ss, out);
    EXPECT_EQ(engine.fen, "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    // clang-format off
    std::vector<Engine::Move> expected =
        {
            Engine::Move("a1b2"), Engine::Move("c4b6"), Engine::Move("a2g5")
        };
    // clang-format on
    ASSERT_EQ(expected.size(), engine.playedMoves.size());
    for(int i = 0; i < expected.size(); i++)
        EXPECT_EQ(expected[i], engine.playedMoves[i]);
}
