#include "../h/Engine.h"
#include <iostream>
#include <algorithm>

using namespace eugenchess::engine;

Engine::Move::Move(std::string_view algebraic)
    : from(), to()
{
    from.x = algebraic[0] - 'a';
    from.y = algebraic[1] - '1';
    to.x = algebraic[2] - 'a';
    to.y = algebraic[3] - '1';
}

std::ostream& operator<<(std::ostream& stream, const Engine::Move& move)
{
    stream << (char)(move.from.x + 'a');
    stream << (char)(move.from.y + '1');
    stream << (char)(move.to.x + 'a');
    stream << (char)(move.to.y + '1');
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Engine::Move::MaybeMove& maybeMove)
{
    if (maybeMove.has_value())
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
    if (!constraint.has_value())
        return true;
    const int valueIndex = static_cast<int>(optionValue.index());
    const int constraintIndex = static_cast<int>(constraint.value().index());
    // Assuming the two are equal.
    if (valueIndex == 0 and constraintIndex == 0) // int with range.
    {
        auto& value = std::get<int>(optionValue);
        auto& rangeConstraint = std::get<RangeConstraint>(constraint.value());
        return rangeConstraint.first <= value and rangeConstraint.second >= value;
    }
    else if (valueIndex == 1 and constraintIndex == 1) // std::string with enum.
    {
        auto& value = std::get<std::string>(optionValue);
        auto& enumConstraint = std::get<EnumConstraint>(constraint.value());
        return std::any_of(enumConstraint.begin(), enumConstraint.end(), [=](const std::string& x)
        { return x == value; });
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

void Engine::ping()
{
    // Nothing needs to be done by default. Override if engine does some expensive setup.
}

void Engine::debugMode(bool mode)
{
    // Nothing needs to be done by default. Override if engine wants to implement any sort of debug mode.
}

bool Engine::getDebugMode() const
{
    return false;
}
