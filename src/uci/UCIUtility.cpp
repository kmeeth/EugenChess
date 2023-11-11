#include "../../h/uci/UCICommunicator.h"
#include "../../h/Engine.h"

using namespace eugenchess::uci;
using namespace eugenchess::engine;

// Lines 186-192 in the spec.
void UCICommunicator::identificationPhase()
{
    out << "id name" << engine.getName() + " " + engine.getVersion() << std::endl;
    out << "id author" << engine.getAuthor() << std::endl;
}