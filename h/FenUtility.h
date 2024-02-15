#ifndef EUGENCHESS_FENUTILITY_H
#define EUGENCHESS_FENUTILITY_H
#include <string>
#include "Engine.h"

namespace eugenchess::implementation
{
    // Returns an updated FEN after a move has been made.
    std::string makeMoveDirectlyOnFEN(std::string_view FEN, const eugenchess::engine::Engine::Move& move);
}

#endif //EUGENCHESS_FENUTILITY_H
