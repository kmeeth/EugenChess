#include "../h/FenUtility.h"
#include <gtest/gtest.h>

using namespace eugenchess::implementation;
using namespace eugenchess::engine;

// Tests a few general moves.
TEST(FenUtilityTests, CommonMoves)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"rnbqk1nr/1pp1bpp1/p2p3p/8/P1BNP3/8/1PP2PPP/RNBQK2R w KQkq - 1 7",
             Engine::Move("d1f3"),
             "rnbqk1nr/1pp1bpp1/p2p3p/8/P1BNP3/5Q2/1PP2PPP/RNB1K2R b KQkq - 2 7"},
            {"rnbqk1nr/1pp1bpp1/p2p3p/8/P1BNP3/5Q2/1PP2PPP/RNB1K2R b KQkq - 2 7",
             Engine::Move("g8f6"),
             "rnbqk2r/1pp1bpp1/p2p1n1p/8/P1BNP3/5Q2/1PP2PPP/RNB1K2R w KQkq - 3 8"},
            {"7r/3kbBp1/p2p3p/p1p2P2/1rP3P1/8/5P1P/1RR3K1 w - - 1 24",
             Engine::Move("f7e6"),
             "7r/3kb1p1/p2pB2p/p1p2P2/1rP3P1/8/5P1P/1RR3K1 b - - 2 24"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests a few normal captures.
TEST(FenUtilityTests, CommonCaptures)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"rnbqkbnr/ppp2pp1/3p3p/4p3/2BPP3/5N2/PPP2PPP/RNBQK2R b KQkq d3 0 4",
             Engine::Move("e5d4"),
             "rnbqkbnr/ppp2pp1/3p3p/8/2BpP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 5"},
            {"rnbqkbnr/ppp2pp1/3p3p/8/2BpP3/5N2/PPP2PPP/RNBQK2R w KQkq - 0 5",
             Engine::Move("f3d4"),
             "rnbqkbnr/ppp2pp1/3p3p/8/2BNP3/8/PPP2PPP/RNBQK2R b KQkq - 0 5"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests double pawn moves (because of en passant squares).
TEST(FenUtilityTests, DoublePawnMoves)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
             Engine::Move("e2e4"),
             "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"},
            {"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1",
             Engine::Move("e7e5"),
             "rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq e6 0 2"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests en passant captures.
TEST(FenUtilityTests, EnPassantCaptures)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"rnbqkbnr/ppp3pp/3p4/4ppP1/4P3/8/PPPP1P1P/RNBQKBNR w KQkq f6 0 4",
             Engine::Move("g5f6"),
             "rnbqkbnr/ppp3pp/3p1P2/4p3/4P3/8/PPPP1P1P/RNBQKBNR b KQkq - 0 4"},
            {"rnbqkbnr/ppp1pppp/8/8/2PpP3/7P/PP1P1PP1/RNBQKBNR b KQkq c3 0 3",
             Engine::Move("d4c3"),
             "rnbqkbnr/ppp1pppp/8/8/4P3/2p4P/PP1P1PP1/RNBQKBNR w KQkq - 0 4"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests promotions.
TEST(FenUtilityTests, Promotions)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"8/4P3/8/8/8/7k/8/7K w - - 0 1",
             Engine::Move("e7e8q"),
             "4Q3/8/8/8/8/7k/8/7K b - - 0 1"},
            {"8/4P3/8/8/8/7k/8/7K w - - 0 1",
             Engine::Move("e7e8n"),
             "4N3/8/8/8/8/7k/8/7K b - - 0 1"},
            {"8/4P3/8/8/8/7k/8/7K w - - 0 1",
             Engine::Move("e7e8b"),
             "4B3/8/8/8/8/7k/8/7K b - - 0 1"},
            {"8/4P3/8/8/8/7k/8/7K w - - 0 1",
             Engine::Move("e7e8r"),
             "4R3/8/8/8/8/7k/8/7K b - - 0 1"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests kingside castles.
TEST(FenUtilityTests, CastlingKingside)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4",
             Engine::Move("e1g1"),
             "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4"},
            {"rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 b kq - 5 4",
             Engine::Move("e8g8"),
             "rnbq1rk1/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQ1RK1 w - - 6 5"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Tests queenside castles.
TEST(FenUtilityTests, CastlingQueenside)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 6 5",
             Engine::Move("e1c1"),
             "r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/2KR1BNR b kq - 7 5"},
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/2KR1BNR b kq - 7 5",
             Engine::Move("e8c8"),
             "2kr1bnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/2KR1BNR w - - 8 6"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// King castling rights relinquishment.
TEST(FenUtilityTests, KingCastlingRelinquishment)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 6 5",
             Engine::Move("e1d1"),
             "r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R2K1BNR b kq - 7 5"},
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R2K1BNR b kq - 7 5",
             Engine::Move("e8d8"),
             "r2k1bnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R2K1BNR w - - 8 6"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}

// Rook castling rights relinquishment.
TEST(FenUtilityTests, RookCastlingRelinquishment)
{
    std::tuple<std::string, Engine::Move, std::string> expected[] =
        {
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/R3KBNR w KQkq - 6 5",
             Engine::Move("a1b1"),
             "r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/1R2KBNR b Kkq - 7 5"},
            {"r3kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/1R2KBNR b Kkq - 7 5",
             Engine::Move("a8b8"),
             "1r2kbnr/ppp1pppp/2nq4/3p1b2/3P1B2/2NQ4/PPP1PPPP/1R2KBNR w Kk - 8 6"},
            {"rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 4 4",
             Engine::Move("h1g1"),
             "rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK1R1 b Qkq - 5 4"},
            {"rnbqk2r/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK1R1 b Qkq - 5 4",
             Engine::Move("h8g8"),
             "rnbqk1r1/pppp1ppp/5n2/2b1p3/2B1P3/5N2/PPPP1PPP/RNBQK1R1 w Qq - 6 5"}};
    for(auto& [before, move, after]: expected)
        ASSERT_EQ(makeMoveDirectlyOnFEN(before, move), after);
}