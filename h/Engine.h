#ifndef EUGENCHESS_ENGINE_H
#define EUGENCHESS_ENGINE_H
#include <string_view>
#include <optional>
#include <any>
#include <variant>
#include <vector>
#include <unordered_map>
/*
 * This class is an abstraction for a chess engine. It offers an interface of basic engine functionalities.
 */
namespace eugenchess::engine
{
    class Engine
    {
    public:
        struct Move
        {
            struct Tile // Coordinates in [0, 7].
            {
                int x, y;
            };
            Tile from, to;
            Move() = default;
            explicit Move(std::string_view algebraic); // Assumes standard algebraic form.
            using MaybeMove = std::optional<Move>; // Support for nullmoves.
        };
        virtual void setPosition(std::string_view FEN) = 0;
        [[nodiscard]] virtual std::string getFEN() const = 0;
        virtual void playMove(Move move) = 0;
        // Some Engines might want to implement some protocol-specific options. This function makes knowledge of the
        // protocol possible to the Engine.
        virtual void setProtocol(std::string_view name) = 0;
        // Engine options are represented as pseudo-JSON objects. Since this is the only part of the Engine that might
        // need knowledge of the protocol, certain protocols might have requirements for their contents. These are to
        // be documented for Communicator specializations, if present. Strings are used as a base type as they are the
        // most general one.
        using OptionEntry = std::variant<std::string, std::vector<std::string>>;
        using EngineOption = std::unordered_map<std::string, OptionEntry>;
        using EngineOptions = std::unordered_map<std::string, EngineOption>;
        [[nodiscard]] virtual EngineOptions& options() const = 0;
        virtual ~Engine() = default;
    };
}
// Prints algebraic form.
std::ostream& operator<<(std::ostream&, const eugenchess::engine::Engine::Move&);
// Printing a MaybeMove to provide support for nullmoves.
std::ostream& operator<<(std::ostream&, const eugenchess::engine::Engine::Move::MaybeMove&);

#endif //EUGENCHESS_ENGINE_H
