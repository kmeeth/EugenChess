#include "../../h/uci/UCICommunicator.h"
#include "../../h/uci/UCIUtility.h"

using namespace eugenchess::uci;
using namespace eugenchess::uci::implementation;
using namespace eugenchess::engine;

UCICommunicator::UCICommunicator(std::istream& in, std::ostream& out, Engine& engine)
    : in(in), out(out), engine(engine)
{
}

void UCICommunicator::run()
{
    engine.setProtocol("uci");
    UCIUtility::identificationPhase(engine, in, out);
    if(engine.requiresRegistration())
        out << "registration error" << std::endl;
    UCIUtility::optionsListingPhase(engine, in, out);
    out << "uciok" << std::endl;
    UCIUtility::mainLoop(engine, in, out);
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
