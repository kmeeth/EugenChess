#include "../../h/uci/UCIUtility.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace eugenchess::uci::implementation;
using namespace eugenchess::uci;
using namespace eugenchess::engine;

namespace
{
    class TestEngineAdapter : public Engine
    {
    public:
        void setPosition(std::string_view FEN) override
        {
            myFEN = FEN;
        }
        [[nodiscard]] std::string getFEN() const override
        {
            return myFEN;
        }
        void setClock(Clock clock) override
        {
            myClock = clock;
        }
        void playMove(Move move) override
        {
            // TODO
        }
        void setProtocol(std::string_view name) override
        {
        }
        void debugMode(bool b) override
        {
            debug = b;
        }
        bool getDebugMode() const override
        {
            return debug;
        }
        EngineOptions& options() override
        {
            return myOptions;
        }
        ~TestEngineAdapter() override = default;
        std::vector<std::string> commands() override
        {
            return myCommands;
        }
        std::string getName() const override
        {
            return "Name";
        }
        std::string getAuthor() const override
        {
            return "Author";
        }
        std::string getVersion() const override
        {
            return "Version";
        }
    protected:
        std::string myFEN;
        Clock myClock;
        bool debug = false;
        EngineOptions myOptions;
        std::vector<std::string> myCommands;
    };
}