#include "../../h/uci/UCIUtility.h"
#include <sstream>

using namespace eugenchess::uci::implementation;
using namespace eugenchess::uci;
using namespace eugenchess::engine;

// Lines 186-192 in the spec.
void UCIUtility::identificationPhase(Engine& engine, std::istream& in, std::ostream& out)
{
    out << "id name " << engine.getName() + " " + engine.getVersion() << std::endl;
    out << "id author " << engine.getAuthor() << std::endl;
}

static std::unordered_map<std::string, std::string> analyseOption(const Engine::EngineOption& option)
{
    auto data = std::unordered_map<std::string, std::string>();
    auto constraint = option.getConstraint();
    if (!constraint.has_value())
        data["type"] = "string";
    else
        data["type"] = (constraint->index() == 0 ? "spin" : "combo");
    if (option.get().index() == 0)
        data["default"] = std::to_string(std::get<int>(option.get()));
    else
        data["default"] = std::get<std::string>(option.get());
    if (data["type"] == "spin")
    {
        data["min"] = std::to_string(std::get<Engine::EngineOption::RangeConstraint>(constraint.value()).first);
        data["max"] = std::to_string(std::get<Engine::EngineOption::RangeConstraint>(constraint.value()).second);
    }
    else if (data["type"] == "combo")
    {
        data["var"] = "";
        for (auto& x: std::get<Engine::EngineOption::EnumConstraint>(constraint.value()))
        {
            data["var"] += " var " + x;
        }
    }
    return data;
}

// Lines 314-426 in the spec.
void UCIUtility::optionsListingPhase(Engine& engine, std::istream& in, std::ostream& out)
{
    auto& options = engine.options();
    for (auto& [name, option]: options)
    {
        auto data = analyseOption(option);
        out << "option name " << name;
        out << " type " << data["type"];
        out << " default " << data["default"];
        if (data["type"] == "combo")
            out << data["var"];
        else if (data["type"] == "spin")
            out << " min " << data["min"] << " max " << data["max"];
        out << std::endl;
    }
    auto commands = engine.commands();
    for (auto& command: commands)
        out << "option name " << command << " type button" << std::endl;
}

void UCIUtility::mainLoop(Engine& engine, std::istream& in, std::ostream& out)
{
    while (true)
    {
        std::string line, token;
        if (!std::getline(in, line))
            line = "quit";
        std::istringstream ss(line);
        ss >> std::skipws >> token;
        if (token == "quit")
            break;
    }
}
