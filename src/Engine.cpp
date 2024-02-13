#include "../h/Engine.h"
#include <algorithm>
#include <iostream>

using namespace eugenchess::engine;

Engine::Move::Move(std::string_view algebraic)
    : from(), to()
{
    from.x = algebraic[0] - 'a';
    from.y = algebraic[1] - '1';
    to.x = algebraic[2] - 'a';
    to.y = algebraic[3] - '1';
    if(algebraic.length() > 4)
        switch(algebraic[4])
        {
        case 'q':
        {
            promotionPiece = Piece::Queen;
            break;
        }
        case 'n':
        {
            promotionPiece = Piece::Knight;
            break;
        }
        case 'r':
        {
            promotionPiece = Piece::Rook;
            break;
        }
        case 'b':
        {
            promotionPiece = Piece::Bishop;
            break;
        }
        }
}

std::ostream& operator<<(std::ostream& stream, const Engine::Move& move)
{
    using namespace eugenchess::engine;
    stream << (char)(move.from.x + 'a');
    stream << (char)(move.from.y + '1');
    stream << (char)(move.to.x + 'a');
    stream << (char)(move.to.y + '1');
    if(move.promotionPiece.has_value())
    {
        char c = ' ';
        switch(move.promotionPiece.value())
        {
        case Engine::Move::Piece::Queen:
        {
            c = 'q';
            break;
        }
        case Engine::Move::Piece::Knight:
        {
            c = 'n';
            break;
        }
        case Engine::Move::Piece::Rook:
        {
            c = 'r';
            break;
        }
        case Engine::Move::Piece::Bishop:
        {
            c = 'b';
            break;
        }
        default:
            break;
        }
        stream << c;
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Engine::Move::MaybeMove& maybeMove)
{
    if(maybeMove.has_value())
        return (stream << maybeMove.value());
    return stream << "0000";
}

Engine::EngineOption::MaybeConstraint Engine::EngineOption::getConstraint() const
{
    return maybeConstraint;
}

Engine::EngineOption::OptionValue Engine::EngineOption::get() const
{
    return value;
}

void Engine::EngineOption::set(const Engine::EngineOption::OptionValue& v)
{
    value = v;
}

bool Engine::EngineOption::isWithinConstraint(const Engine::EngineOption::OptionValue& optionValue,
                                              const Engine::EngineOption::MaybeConstraint& constraint)
{
    if(!constraint.has_value())
        return true;
    const int valueIndex = static_cast<int>(optionValue.index());
    const int constraintIndex = static_cast<int>(constraint.value().index());
    // Assuming the two are equal.
    if(valueIndex == 0 and constraintIndex == 0) // int with range.
    {
        auto& value = std::get<int>(optionValue);
        auto& rangeConstraint = std::get<RangeConstraint>(constraint.value());
        return rangeConstraint.first <= value and rangeConstraint.second >= value;
    }
    else if(valueIndex == 1 and constraintIndex == 1) // std::string with enum.
    {
        auto& value = std::get<std::string>(optionValue);
        auto& enumConstraint = std::get<EnumConstraint>(constraint.value());
        return std::any_of(enumConstraint.begin(), enumConstraint.end(), [=](const std::string& x) { return x == value; });
    }
    return false;
}

Engine::EngineOption::EngineOption(Engine::EngineOption::Constraint c)
    : maybeConstraint(c)
{
}

std::string Engine::getName() const
{
    return "Unknown Engine";
}

std::string Engine::getAuthor() const
{
    return "Unknown Author";
}

std::string Engine::getVersion() const
{
    return "Unknown Version";
}
