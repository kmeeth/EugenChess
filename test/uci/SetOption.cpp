#include "../../h/uci/UCIUtility.h"
#include <gtest/gtest.h>
#include <sstream>
#include <unordered_set>

using namespace eugenchess::engine;
using namespace eugenchess::uci;
using namespace eugenchess::uci::implementation;

namespace
{
    class TestEngine : public Engine
    {
    public:
        TestEngine()
        {
            myOptions.emplace("spin1", EngineOption::RangeConstraint{0, 100});
            myOptions.emplace("spin2", EngineOption::RangeConstraint{50, 60});
            myOptions["spin1"].set(69);
            myOptions["spin2"].set(52);
            myOptions.emplace("combo1", EngineOption::EnumConstraint{"A", "B", "C"});
            myOptions.emplace("combo2", EngineOption::EnumConstraint{"x", "xy", "abc", "lol"});
            myOptions["combo1"].set("A");
            myOptions["combo2"].set("abc");
            myOptions.emplace("string1", EngineOption());
            myOptions.emplace("string2", EngineOption());
            myOptions["string1"].set("A");
            myOptions["string2"].set("28.6.1389.");
            myCommands.emplace_back("button1");
            myCommands.emplace_back("button2");
        }
        void setPosition(std::string_view FEN) override
        {
        }
        [[nodiscard]] std::string getFEN() const override
        {
            return {};
        }
        void setClock(Clock clock) override
        {
        }
        void playMove(Move move) override
        {
        }
        Move calculateBestMove() override
        {
            return {};
        }
        void setProtocol(std::string_view name) override
        {
        }
        EngineOptions& options() override
        {
            return myOptions;
        }
        ~TestEngine() override = default;
        void performCommand(std::string_view name) override
        {
            performedCommands.emplace(name);
        }
        std::vector<std::string> commands() override
        {
            return myCommands;
        }
        std::string getName() const override
        {
            return Engine::getName();
        }
        std::string getAuthor() const override
        {
            return Engine::getAuthor();
        }
        std::string getVersion() const override
        {
            return Engine::getVersion();
        }
        std::unordered_set<std::string> performedCommands;
    private:
        EngineOptions myOptions;
        std::vector<std::string> myCommands;
    };
}

TEST(OptionsListingTests, SpinOptions)
{
    std::istringstream ss("name spin1 value 43");
    TestEngine engine;
    UCIUtility::setoptionHandler(engine, ss, std::cout);
    EXPECT_EQ(std::get<int>(engine.options()["spin1"].get()), 43);
}

TEST(OptionsListingTests, ComboOptions)
{
    std::istringstream ss("name combo1 value C");
    TestEngine engine;
    UCIUtility::setoptionHandler(engine, ss, std::cout);
    EXPECT_EQ(std::get<std::string>(engine.options()["combo1"].get()), "C");
}

TEST(OptionsListingTests, StringOptions)
{
    std::istringstream ss("name string1 value A quick brown fox jumps over the lazy dog.");
    TestEngine engine;
    UCIUtility::setoptionHandler(engine, ss, std::cout);
    EXPECT_EQ(std::get<std::string>(engine.options()["string1"].get()), "A quick brown fox jumps over the lazy dog.");
}

TEST(OptionsListingTests, ButtonOptions)
{
    std::istringstream ss("name button1");
    TestEngine engine;
    UCIUtility::setoptionHandler(engine, ss, std::cout);
    EXPECT_TRUE(engine.performedCommands.count("button1") > 0);
}

TEST(OptionsListingTest, AllOptions)
{
    TestEngine engine;
    {
        std::istringstream ss("name spin2 value 43");
        UCIUtility::setoptionHandler(engine, ss, std::cout);
        EXPECT_EQ(std::get<int>(engine.options()["spin2"].get()), 43);
    }
    {
        std::istringstream ss("name combo2 value lol");
        UCIUtility::setoptionHandler(engine, ss, std::cout);
        EXPECT_EQ(std::get<std::string>(engine.options()["combo2"].get()), "lol");
    }
    {
        std::istringstream ss("name string2 value I love cake.");
        UCIUtility::setoptionHandler(engine, ss, std::cout);
        EXPECT_EQ(std::get<std::string>(engine.options()["string2"].get()), "I love cake.");
    }
    {
        {
            std::istringstream ss("name button1");
            UCIUtility::setoptionHandler(engine, ss, std::cout);
        }
        {
            std::istringstream ss("name button2");
            UCIUtility::setoptionHandler(engine, ss, std::cout);
        }
        EXPECT_TRUE(engine.performedCommands.count("button2") > 0);
        EXPECT_TRUE(engine.performedCommands.count("button1") > 0);
    }
}