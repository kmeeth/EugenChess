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
        }
        [[nodiscard]] std::string getFEN() const override
        {
            return "";
        }
        void setClock(Clock clock) override
        {
            myClock = clock;
        }
        void playMove(Move move) override
        {
        }
        Move calculateBestMove() override
        {
            return Move("a1a1");
        }
        void setProtocol(std::string_view name) override
        {
        }
        EngineOptions& options() override
        {
            return myOptions;
        }
        void performCommand(std::string_view name) override
        {
        }
        std::vector<std::string> commands() override
        {
            return {};
        }
        Clock myClock = Clock();
        EngineOptions myOptions =
            {
                {"infinite", EngineOption(Engine::EngineOption::Constraint(std::pair<int, int>(0, 1)))},
                {"depth", EngineOption(Engine::EngineOption::Constraint(std::pair<int, int>(0, std::numeric_limits<int>::max())))},
                {"nodes", EngineOption(Engine::EngineOption::Constraint(std::pair<int, int>(0, std::numeric_limits<int>::max())))},
                {"mate", EngineOption(Engine::EngineOption::Constraint(std::pair<int, int>(0, std::numeric_limits<int>::max())))},
                {"movetime", EngineOption(Engine::EngineOption::Constraint(std::pair<int, int>(0, std::numeric_limits<int>::max())))},
            };
    };
}

TEST(GoTests, DefaultValues)
{
    auto engine = TestEngine();
    std::istringstream ss("go");
    std::ostringstream out;
    UCIUtility::goHandler(engine, ss, out);
    const std::string keys[] = {"infinite", "depth", "nodes", "mate", "movetime"};
    for(auto& key: keys)
    {
        EXPECT_TRUE(engine.options().count(key));
        auto optionValue = engine.options().at(key).get();
        EXPECT_EQ(optionValue.index(), 0);
        const int expectedDefault = (key == "infinite" or key == "mate" ? 0 : std::numeric_limits<int>::max());
        EXPECT_EQ(std::get<int>(optionValue), expectedDefault);
    }
    UCIUtility::waitForAllCalculations();
}

TEST(GoTests, DefaultValuesClock)
{
    auto engine = TestEngine();
    std::istringstream ss("go");
    std::ostringstream out;
    UCIUtility::goHandler(engine, ss, out);
    EXPECT_EQ(engine.myClock.turnsToTimeControl, std::numeric_limits<int>::max());
    EXPECT_EQ(engine.myClock.blackTime, std::numeric_limits<int>::max());
    EXPECT_EQ(engine.myClock.whiteTime, std::numeric_limits<int>::max());
    EXPECT_EQ(engine.myClock.whiteIncrement, 0);
    EXPECT_EQ(engine.myClock.blackIncrement, 0);
    UCIUtility::waitForAllCalculations();
}

TEST(GoTests, CustomValues)
{
    auto engine = TestEngine();
    std::istringstream ss("go infinite mate 5 depth 7 nodes 10000 movetime 100000");
    std::ostringstream out;
    UCIUtility::goHandler(engine, ss, out);
    const std::string keys[] = {"infinite", "depth", "nodes", "mate", "movetime"};
    const int expectedValues[] = {1, 7, 10000, 5, 100000};
    for(int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
    {
        const std::string key = keys[i];
        const int expectedValue = expectedValues[i];
        EXPECT_TRUE(engine.options().count(key));
        auto optionValue = engine.options().at(key).get();
        EXPECT_EQ(optionValue.index(), 0);
        EXPECT_EQ(std::get<int>(optionValue), expectedValue);
    }
    UCIUtility::waitForAllCalculations();
}

TEST(GoTests, CustomValuesClock)
{
    auto engine = TestEngine();
    std::istringstream ss("go wtime 1 btime 2 winc 3 binc 4 movestogo 5");
    std::ostringstream out;
    UCIUtility::goHandler(engine, ss, out);
    EXPECT_EQ(engine.myClock.turnsToTimeControl, 5);
    EXPECT_EQ(engine.myClock.blackTime, 2);
    EXPECT_EQ(engine.myClock.whiteTime, 1);
    EXPECT_EQ(engine.myClock.whiteIncrement, 3);
    EXPECT_EQ(engine.myClock.blackIncrement, 4);
    UCIUtility::waitForAllCalculations();
}

TEST(GoTests, Ponder)
{
    auto engine = TestEngine();
    std::istringstream ss("go ponder");
    std::ostringstream out;
    UCIUtility::goHandler(engine, ss, out);
    UCIUtility::waitForAllCalculations();
}
