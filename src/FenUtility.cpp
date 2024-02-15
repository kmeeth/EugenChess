#include "../h/FenUtility.h"
#include <sstream>

using namespace eugenchess::engine;
using namespace eugenchess::implementation;

namespace
{
    enum class Color
    {
        Black,
        White,
        Count
    };
    struct ColoredPiece
    {
        Engine::Move::Piece piece;
        Color color;
    };
    using MaybeColoredPiece = std::optional<ColoredPiece>;
}

std::string makeMoveDirectlyOnFEN(std::string_view FEN, const Engine::Move& move)
{
    constexpr int boardSize = 8;
    MaybeColoredPiece boardState[boardSize][boardSize]; // x, y
    auto getColoredPiece = [](char c) -> ColoredPiece {
        Color color = ((c >= 'A' and c <= 'Z') ? Color::White : Color::Black);
        c = static_cast<char>(std::tolower(c));
        Engine::Move::Piece piece;
        switch(c)
        {
        case 'q':
        {
            piece = Engine::Move::Piece::Queen;
            break;
        }
        case 'n':
        {
            piece = Engine::Move::Piece::Knight;
            break;
        }
        case 'r':
        {
            piece = Engine::Move::Piece::Rook;
            break;
        }
        case 'b':
        {
            piece = Engine::Move::Piece::Bishop;
            break;
        }
        case 'k':
        {
            piece = Engine::Move::Piece::King;
            break;
        }
        case 'p':
        {
            piece = Engine::Move::Piece::Pawn;
            break;
        }
        default:
            piece = Engine::Move::Piece::Count;
        }
        return {piece, color};
    };
    std::istringstream ss(FEN.data()); // Split the data by space.
    std::string part;

    // Phase 1: pieces on the board.
    ss >> part;
    int x, y = 0;
    for(char c: part)
    {
        if(c == '/')
        {
            x = 0;
            y++;
        }
        else if(c >= '0' and c <= '9')
            x += c - '0';
        else
            boardState[x++][y] = getColoredPiece(c);
    }

    // Phase 2: active color.
    ss >> part;
    Color activeColor = (part[0] == 'w' ? Color::White : Color::Black);

    // Phase 3: castling rights.
    constexpr int queenSide = 0;
    constexpr int kingSide = 1;
    bool castlingRights[static_cast<int>(Color::Count)][2] = {};
    ss >> part;
    auto stringContains = [](const std::string& str, const char c) -> bool {
        return str.find(c) != std::string::npos;
    };
    castlingRights[static_cast<int>(Color::Black)][kingSide] = stringContains(part, 'k');
    castlingRights[static_cast<int>(Color::Black)][queenSide] = stringContains(part, 'q');
    castlingRights[static_cast<int>(Color::White)][kingSide] = stringContains(part, 'K');
    castlingRights[static_cast<int>(Color::White)][queenSide] = stringContains(part, 'Q');

    // Phase 4: en passant square.
    using MaybeTile = std::optional<Engine::Move::Tile>;
    MaybeTile enPassantTile;
    ss >> part;
    if(part != "-")
    {
        enPassantTile = Engine::Move::Tile();
        enPassantTile.value().x = part[0] - 'a';
        enPassantTile.value().y = part[1] - '0';
    }

    // Phase 5: halfmove count.
    ss >> part;
    int halfmoveCount = std::stoi(part);

    // Phase 6: fullmove count.
    ss >> part;
    int fullmoveCount = std::stoi(part);

    // TODO: update the FEN
    throw std::exception();
}
