#ifndef EUGENCHESS_UCIUTILITY_H
#define EUGENCHESS_UCIUTILITY_H

#include "UCICommunicator.h"
#include "../Engine.h"

namespace eugenchess::uci::implementation
{
    class UCIUtility
    {
    public:
        static void identificationPhase(engine::Engine& engine, std::istream& in, std::ostream& out);
        static engine::Engine::EngineOptions&
        optionsListingPhase(engine::Engine& engine, std::istream& in, std::ostream& out);
    };
}

#endif //EUGENCHESS_UCIUTILITY_H