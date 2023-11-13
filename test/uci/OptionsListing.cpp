#include "../../h/uci/UCIUtility.h"
#include <sstream>
#include <gtest/gtest.h>

using namespace eugenchess::engine;
using namespace eugenchess::uci;

namespace
{
    class TestEngine : public Engine
    {
    public:
        explicit TestEngine(int test)
        {
            if (test == 0)
            {
                myOptions.emplace("spin1", EngineOption::RangeConstraint{ 0, 100 });
                myOptions.emplace("spin2", EngineOption::RangeConstraint{ 50, 60 });
                myOptions["spin1"].set(69);
                myOptions["spin2"].set(52);
            }
            else if (test == 1)
            {
                myOptions.emplace("combo1", EngineOption::EnumConstraint{ "A", "B", "C" });
                myOptions.emplace("combo2", EngineOption::EnumConstraint{ "x", "xy", "abc", "lol" });
                myOptions["combo1"].set("A");
                myOptions["combo2"].set("abc");
            }
            else if (test == 2)
            {
                myOptions.emplace("string1", EngineOption());
                myOptions.emplace("string2", EngineOption());
                myOptions["string1"].set("A");
                myOptions["string2"].set("28.6.1389.");
            }
            else if (test == 3)
            {
                myCommands.emplace_back("button1");
                myCommands.emplace_back("button2");
            }
            else if (test == 4)
            {
                myOptions.emplace("spin1", EngineOption::RangeConstraint{ 0, 100 });
                myOptions.emplace("spin2", EngineOption::RangeConstraint{ 50, 60 });
                myOptions["spin1"].set(69);
                myOptions["spin2"].set(52);
                myOptions.emplace("combo1", EngineOption::EnumConstraint{ "A", "B", "C" });
                myOptions.emplace("combo2", EngineOption::EnumConstraint{ "x", "xy", "abc", "lol" });
                myOptions["combo1"].set("A");
                myOptions["combo2"].set("abc");
                myOptions.emplace("string1", EngineOption());
                myOptions.emplace("string2", EngineOption());
                myOptions["string1"].set("A");
                myOptions["string2"].set("28.6.1389.");
                myCommands.emplace_back("button1");
                myCommands.emplace_back("button2");
            }
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
            return myOptions;
        }
        ~TestEngine() override = default;
        void performCommand(std::string_view name) override
        {
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
    private:
        EngineOptions myOptions;
        std::vector<std::string> myCommands;
    };
    void checkOptions(int test, std::vector<std::string>& expected)
    {
        TestEngine engine(test);
        std::stringstream in, out;
        UCICommunicator communicator(in, out, engine);
        implementation::UCIUtility::optionsListingPhase(engine, in, out);
        while (out)
        {
            std::string line;
            std::getline(out, line);
            if (line.empty())
                continue;
            EXPECT_TRUE(std::any_of(expected.begin(), expected.end(), [=](auto& x)
            { return x == line; }));
        }
    }
}

TEST(OptionsListingTests, SpinOptions)
{
    std::vector<std::string> expected =
        {
            "option name spin1 type spin default 69 min 0 max 100",
            "option name spin2 type spin default 52 min 50 max 60"
        };
    checkOptions(0, expected);
}

TEST(OptionsListingTests, ComboOptions)
{
    std::vector<std::string> expected =
        {
            "option name combo1 type combo default A var A var B var C",
            "option name combo2 type combo default abc var x var xy var abc var lol"
        };
    checkOptions(1, expected);
}

TEST(OptionsListingTests, StringOptions)
{
    std::vector<std::string> expected =
        {
            "option name string1 type string default A",
            "option name string2 type string default 28.6.1389."
        };
    checkOptions(2, expected);
}

TEST(OptionsListingTests, ButtonOptions)
{
    std::vector<std::string> expected =
        {
            "option name button1 type button",
            "option name button2 type button"
        };
    checkOptions(3, expected);
}

TEST(OptionsListingTest, AllOptions)
{
    std::vector<std::string> expected =
        {
            "option name button1 type button",
            "option name button2 type button",
            "option name string1 type string default A",
            "option name string2 type string default 28.6.1389.",
            "option name combo1 type combo default A var A var B var C",
            "option name combo2 type combo default abc var x var xy var abc var lol",
            "option name spin1 type spin default 69 min 0 max 100",
            "option name spin2 type spin default 52 min 50 max 60"
        };
    checkOptions(4, expected);
}