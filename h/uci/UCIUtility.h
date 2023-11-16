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
        static void optionsListingPhase(engine::Engine& engine, std::istream& in, std::ostream& out);
        static void mainLoop(engine::Engine& engine, std::istream& in, std::ostream& out);
        static void uciokHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void debugHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void setoptionHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
    };
}

#endif //EUGENCHESS_UCIUTILITY_H
