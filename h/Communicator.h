#ifndef EUGENCHESS_COMMUNICATOR_H
#define EUGENCHESS_COMMUNICATOR_H

#include <iostream>
/*
 * This class is an abstraction for a communication protocol between a chess engine and a chess GUI. Running a
 * Communicator initiates a communication session, which will progress depending on the protocol used.
 */
namespace eugenchess
{
    namespace engine
    {
        class Engine;
    }
    namespace communication
    {
        class Communicator
        {
        public:
            virtual ~Communicator() = default;
            virtual void run() = 0;

        protected:
            [[nodiscard]] virtual std::istream& getInput() const = 0;
            [[nodiscard]] virtual std::ostream& getOutput() const = 0;
            [[nodiscard]] virtual engine::Engine& getEngine() const = 0;
        };
    }// namespace communication
}// namespace eugenchess

#endif//EUGENCHESS_COMMUNICATOR_H
