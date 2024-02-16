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
            piece = Engine::Move::Piece::Queen;
            break;
        case 'n':
            piece = Engine::Move::Piece::Knight;
            break;
        case 'r':
            piece = Engine::Move::Piece::Rook;
            break;
        case 'b':
            piece = Engine::Move::Piece::Bishop;
            break;
        case 'k':
            piece = Engine::Move::Piece::King;
            break;
        case 'p':
            piece = Engine::Move::Piece::Pawn;
            break;
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
        enPassantTile.value().y = part[1] - '1';
    }

    // Phase 5: halfmove count.
    ss >> part;
    int halfmoveCount = std::stoi(part);

    // Phase 6: fullmove count.
    ss >> part;
    int fullmoveCount = std::stoi(part);

    // Updating the board.
    // Phase 1: move the piece.
    Engine::Move::Tile from = move.from, to = move.to;
    boardState[to.x][to.y] = boardState[from.x][from.y];
    boardState[from.x][from.y].reset();

    // Phase 2: check if it is a promotion.
    if(to.y == 0 or to.y == 7)
        boardState[to.x][to.y] = {move.promotionPiece.value(), activeColor};

    // Phase 3: check if it is an en passant.
    if(boardState[to.x][to.y].value().piece == Engine::Move::Piece::Pawn                                     // Did a pawn move?
       and enPassantTile.has_value() and enPassantTile.value().x == to.x and enPassantTile.value().y == to.y // Was it to the en passant tile?
       and to.x != from.x)                                                                                   // Was it diagonal?
        boardState[to.x][from.y].reset();

    // Phase 4: check if it was a king move and if it was a castle.
    if(boardState[to.x][to.y].value().piece == Engine::Move::Piece::King)
    {
        for(auto& a: castlingRights[static_cast<int>(activeColor)])
            a = false;                   // Removing both castling rights.
        if(std::abs(to.x - from.x) == 2) // Was it a castle?
        {
            boardState[(to.x + from.x) / 2][to.y] = {Engine::Move::Piece::Rook, activeColor}; // Move the rook to the new place.
            boardState[(to.x > from.x ? boardSize - 1 : 0)][to.y].reset();                    // Remove the rook from its old spot.
        }
    }

    // Phase 5: check if it was a move from a corner, because of castling rights.
    if((from.x == 0 or from.x == boardSize - 1) and from.y == (activeColor == Color::Black ? boardSize - 1 : 0))
        for(auto& a: castlingRights[static_cast<int>(activeColor)])
            a = false; // Removing both castling rights.

    // Phase 6: check if it was a double pawn move, because of en passant.
    if(boardState[to.x][to.y].value().piece == Engine::Move::Piece::Pawn and std::abs(from.y - to.y) == 2)
        enPassantTile = {to.x, (to.y + from.y) / 2};
    else
        enPassantTile.reset();

    // Phase 7: increment the counters.
    halfmoveCount++;
    if(activeColor == Color::Black)
        fullmoveCount++;

    // Phase 8: swap the active color.
    activeColor = (activeColor == Color::Black ? Color::White : Color::Black);

    // Generating the new FEN.
    // Phase 1: pieces on the board.
    auto getCharacterFromColoredPiece = [](const ColoredPiece& p) -> char {
        char c = 0;
        switch(p.piece)
        {
        case Engine::Move::Piece::Pawn:
            c = 'p';
            break;
        case Engine::Move::Piece::Knight:
            c = 'n';
            break;
        case Engine::Move::Piece::Bishop:
            c = 'b';
            break;
        case Engine::Move::Piece::Rook:
            c = 'r';
            break;
        case Engine::Move::Piece::Queen:
            c = 'q';
            break;
        case Engine::Move::Piece::King:
            c = 'k';
            break;
        default:
            break;
        }
        if(p.color == Color::White)
            c = static_cast<char>(std::toupper(c));
        return c;
    };
    std::string newFEN;
    for(y = 0; y < boardSize; y++)
    {
        int emptyCounter = 0;
        for(x = 0; x < boardSize; x++)
        {
            if(!boardState[x][y].has_value())
                emptyCounter++;
            else
            {
                newFEN += (emptyCounter == 0 ? "" : std::to_string(emptyCounter));
                newFEN += getCharacterFromColoredPiece(boardState[x][y].value());
                emptyCounter = 0;
            }
        }
        newFEN += (y == boardSize - 1 ? ' ' : '/');
    }

    // Phase 2: active color.
    newFEN += (activeColor == Color::White ? "w " : "b ");

    // Phase 3: castling rights.
    std::string addition;
    if(castlingRights[static_cast<int>(Color::White)][kingSide]) addition += 'K';
    if(castlingRights[static_cast<int>(Color::White)][kingSide]) addition += 'Q';
    if(castlingRights[static_cast<int>(Color::Black)][kingSide]) addition += 'k';
    if(castlingRights[static_cast<int>(Color::Black)][kingSide]) addition += 'q';
    if(addition.empty())
        addition = "-";
    addition += " ";
    newFEN += addition;

    // Phase 4: en passant square.
    if(enPassantTile.has_value())
    {
        newFEN += static_cast<char>(enPassantTile.value().x + 'a');
        newFEN += static_cast<char>(enPassantTile.value().y + '1');
    }
    else
        newFEN += '-';
    newFEN += ' ';

    // Phase 5: halfmove count.
    newFEN += std::to_string(halfmoveCount) + " ";

    // Phase 6: fullmove count.
    newFEN += std::to_string(fullmoveCount);

    return newFEN;
}
