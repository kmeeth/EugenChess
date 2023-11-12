#include "../../h/uci/UCICommunicator.h"
#include "../../h/Engine.h"

using namespace eugenchess::uci;
using namespace eugenchess::engine;

UCICommunicator::UCICommunicator(std::istream& in, std::ostream& out, Engine& engine)
    : in(in), out(out), engine(engine)
{
}

void UCICommunicator::run()
{
    engine.setProtocol("uci");
    identificationPhase(in, out, engine);
    optionsListingPhase(in, out, engine);
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


