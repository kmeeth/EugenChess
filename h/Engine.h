#ifndef EUGENCHESS_ENGINE_H
#define EUGENCHESS_ENGINE_H

#include <any>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
/*
 * This class is an abstraction for a chess engine. It offers an interface of basic engine functionalities.
 */
namespace eugenchess::engine
{
    class Engine
    {
    public:
        struct Move {
            struct Tile// Coordinates in [0, 7].
            {
                int x, y;
            };
            Tile from, to;
            Move() = default;
            explicit Move(std::string_view algebraic);// Assumes standard algebraic form.
            using MaybeMove = std::optional<Move>;    // Support for nullmoves.
        };
        virtual void setPosition(std::string_view FEN) = 0;
        [[nodiscard]] virtual std::string getFEN() const = 0;
        struct Clock {
            int whiteTime, blackTime;          // In milliseconds.
            int whiteIncrement, blackIncrement;// In milliseconds.
            int turnsToTimeControl;
        };
        virtual void setClock(Clock clock) = 0;
        virtual void playMove(Move move) = 0;
        virtual Move calculateBestMove() = 0;
        virtual void ponderMove(Move move) = 0;
        // The pondered move has been played and the engine should continue the search for the best move as regular.
        // ponderHit() returns the best move.
        virtual Move ponderHit() = 0;
        // Some Engines might want to implement some protocol-specific options. This function makes knowledge of the
        // protocol possible to the Engine.
        virtual void setProtocol(std::string_view name) = 0;
        // Engine options are represented as either integers or strings. These can be constrained or unconstrained.
        // Constrained strings belong to a set of strings, while constrained integers fall within a range.
        class EngineOption
        {
        public:
            using RangeConstraint = std::pair<int, int>;
            using EnumConstraint = std::vector<std::string>;
            using Constraint = std::variant<RangeConstraint, EnumConstraint>;
            using MaybeConstraint = std::optional<Constraint>;
            using OptionValue = std::variant<int, std::string>;
            EngineOption() = default;
            explicit EngineOption(Constraint c);
            [[nodiscard]] MaybeConstraint getConstraint() const;
            static bool isWithinConstraint(const OptionValue&, const MaybeConstraint&);
            // The get() and set() methods may be overridden, e.g. in order to ensure thread-safety.
            [[nodiscard]] virtual OptionValue get() const;
            virtual void set(const OptionValue&);
            virtual ~EngineOption() = default;
        protected:
            // This protected member exists because it serves the default implementations of the methods. It
            // can be used freely in alternative implementations.
            OptionValue value;
        private:
            // Must have the same underlying type as the value.
            const MaybeConstraint maybeConstraint;
        };
        // Maps the name of the engine option to the option itself.
        using EngineOptions = std::unordered_map<std::string, EngineOption>;
        [[nodiscard]] virtual EngineOptions& options() = 0;
        virtual ~Engine() = default;
        // Commands are (optional) custom actions that can be called upon the engine by name. It is recommended that no
        // commands and options share no names.
        virtual void performCommand(std::string_view name) = 0;
        [[nodiscard]] virtual std::vector<std::string> commands() = 0;
        // Identification.
        [[nodiscard]] virtual std::string getName() const;
        [[nodiscard]] virtual std::string getAuthor() const;
        [[nodiscard]] virtual std::string getVersion() const;
    };
}
// Prints algebraic form.
std::ostream& operator<<(std::ostream&, const eugenchess::engine::Engine::Move&);
// Printing a MaybeMove to provide support for nullmoves.
std::ostream& operator<<(std::ostream&, const eugenchess::engine::Engine::Move::MaybeMove&);

#endif//EUGENCHESS_ENGINE_H
