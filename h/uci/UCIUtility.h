#ifndef EUGENCHESS_UCIUTILITY_H
#define EUGENCHESS_UCIUTILITY_H

#include "../Engine.h"
#include "UCICommunicator.h"
#include <thread>
#include <unordered_set>

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
        static void copyprotectionHandler(engine::Engine& engine, std::ostream& out);
        static void registerHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void ucinewgameHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void isreadyHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void positionHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void goHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void ponderhitHandler(engine::Engine& engine, std::istringstream& ss, std::ostream& out);
        static void waitForAllCalculations();
        static std::unique_ptr<std::thread> activeCalculationThread;
    };
}

#endif //EUGENCHESS_UCIUTILITY_H
