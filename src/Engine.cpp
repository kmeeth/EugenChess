#include "../h/Engine.h"
#include <iostream>

using namespace eugenchess::engine;

Engine::Move::Move(std::string_view algebraic)
    :from(), to()
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

std::ostream &operator<<(std::ostream& stream, const Engine::Move::MaybeMove& maybeMove)
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
