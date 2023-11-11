#include "../../h/uci/UCICommunicator.h"

using namespace eugenchess::uci;
using namespace eugenchess::engine;

UCICommunicator::UCICommunicator(std::istream& in, std::ostream& out, Engine& engine)
    : in(in), out(out), engine(engine)
{
}

void UCICommunicator::run()
{
    // TODO
}

std::istream& UCICommunicator::getInput() const
{
    return in;
}

std::ostream& UCICommunicator::getOutput() const
{
    return out;
}

Engine& UCICommunicator::getEngine() const
{
    return engine;
}


