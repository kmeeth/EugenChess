#ifndef EUGENCHESS_UCIUTILITY_H
#define EUGENCHESS_UCIUTILITY_H

#include "../Engine.h"
#include "UCICommunicator.h"

namespace eugenchess::uci::implementation
{
    class UCIUtility
    {
    public:
        static void identificationPhase(engine::Engine& engine, std::istream& in, std::ostream& out);
        static void optionsListingPhase(engine::Engine& engine, std::istream& in, std::ostream& out);
    };
}

#endif//EUGENCHESS_UCIUTILITY_H
