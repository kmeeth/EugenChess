#ifndef EUGENCHESS_UCICOMMUNICATOR_H
#define EUGENCHESS_UCICOMMUNICATOR_H

#include "../Communicator.h"

namespace eugenchess::uci
{
    class UCICommunicator : public communication::Communicator
    {
    public:
        UCICommunicator(std::istream& in, std::ostream& out, engine::Engine& engine);
        void run() override;
    protected:
        [[nodiscard]] std::istream& getInput() const override;
        [[nodiscard]] std::ostream& getOutput() const override;
        [[nodiscard]] engine::Engine& getEngine() const override;
    private:
        std::istream& in;
        std::ostream& out;
        engine::Engine& engine;
    };
}

#endif//EUGENCHESS_UCICOMMUNICATOR_H
