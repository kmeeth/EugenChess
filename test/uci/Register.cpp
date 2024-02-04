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
        explicit TestEngine(std::string username, std::string code)
            : username(std::move(username)), code(std::move(code))
        {
        }
        void setPosition(std::string_view FEN) override
        {
        }
        [[nodiscard]] std::string getFEN() const override
        {
            throw std::exception();
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
            throw std::exception();
        }
        ~TestEngine() override = default;
        void performCommand(std::string_view name) override
        {
        }
        std::vector<std::string> commands() override
        {
            throw std::exception();
        }
        [[nodiscard]] bool requiresRegistration() const override
        {
            return true;
        }
        bool registrationCheck(const MaybeCredential& u, const MaybeCredential& c) override
        {
            if(!username.empty())
                if(username != u)
                    return false;
            if(!code.empty())
                if(code != c)
                    return false;
            return true;
        }
    private:
        std::string username, code;
    };
}

TEST(RegisterTests, NameRequiredSuccess)
{
    auto engine = TestEngine("kmeeth", "");
    std::string line;
    std::istringstream ss("name kmeeth");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, NameRequiredFail)
{
    auto engine = TestEngine("kmeeth", "");
    std::string line;
    std::istringstream ss("name notkmeeth");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}

TEST(RegisterTests, NameRequiredSpacesSuccess)
{
    auto engine = TestEngine("kmeeth plus", "");
    std::string line;
    std::istringstream ss("name kmeeth plus");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, NameRequiredSpacesFail)
{
    auto engine = TestEngine("kmeeth plus", "");
    std::string line;
    std::istringstream ss("name kmeeth minus");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}

TEST(RegisterTests, CodeRequiredSuccess)
{
    auto engine = TestEngine("", "12345");
    std::string line;
    std::istringstream ss("code 12345");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, CodeRequiredFail)
{
    auto engine = TestEngine("", "12345");
    std::string line;
    std::istringstream ss("code 123456");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}

TEST(RegisterTests, CodeRequiredSpacesSuccess)
{
    auto engine = TestEngine("", "12345 67");
    std::string line;
    std::istringstream ss("code 12345 67");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, CodeRequiredSpacesFail)
{
    auto engine = TestEngine("", "12345 67");
    std::string line;
    std::istringstream ss("code 12345vcb");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}

TEST(RegisterTests, LaterFail)
{
    auto engine = TestEngine("kmeeth", "1905");
    std::string line;
    std::istringstream ss("later");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}

TEST(RegisterTests, LaterSuccess)
{
    auto engine = TestEngine("", "");
    std::string line;
    std::istringstream ss("later");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, BothSuccess)
{
    auto engine = TestEngine("kmeeth plus", "1905");
    std::string line;
    std::istringstream ss("name kmeeth plus code 1905");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, BothSuccessSwapped)
{
    auto engine = TestEngine("kmeeth plus", "1905");
    std::string line;
    std::istringstream ss("code 1905 name kmeeth plus");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration ok");
}

TEST(RegisterTests, BothFailSwapped)
{
    auto engine = TestEngine("kmeeth plus", "1905");
    std::string line;
    std::istringstream ss("code kmeeth plus name 1905");
    std::ostringstream out;
    UCIUtility::registerHandler(engine, ss, out);
    std::stringstream check(out.str());
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration checking");
    EXPECT_TRUE(std::getline(check, line));
    EXPECT_EQ(line, "registration error");
}
