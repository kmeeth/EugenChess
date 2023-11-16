#include "../../h/uci/UCIUtility.h"
#include <sstream>
#include <functional>

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

// Lines 73 - 82 in the spec.
void UCIUtility::uciokHandler(Engine& engine, std::istringstream& ss, std::ostream& out)
{
    engine.ping();
    out << "readyok" << std::endl;
}

// Lines 66 - 71 in the spec.
void UCIUtility::debugHandler(Engine& engine, std::istringstream& ss, std::ostream& out)
{
    std::string token;
    ss >> token;
    if (token == "on")
        engine.debugMode(true);
    else if (token == "off")
        engine.debugMode(false);
}

// Determines the type of the option and thus sets the value in a type-safe manner.
static void setOptionFromString(Engine::EngineOption& option, std::string_view value)
{
    bool isRangeConstrained = option.getConstraint().has_value() and option.getConstraint().value().index() == 0;
    if (isRangeConstrained)
        option.set(std::stoi(value.data()));
    else
        option.set(value.data());
}

// Lines 84-96 in thespec.
void UCIUtility::setoptionHandler(Engine& engine, std::istringstream& ss, std::ostream& out)
{
    auto& options = engine.options();
    std::string token, value, name;
    ss >> token; // "name" token.
    while (ss >> token and token != "value") // Names can have spaces.
        name += (name.empty() ? "" : " ") + token;
    if (options.count(name)) // It is a proper EngineOption.
    {
        while (ss >> token) // Values can have spaces.
            value += (value.empty() ? "" : " ") + token;
        setOptionFromString(options[name], value);
    }
    else // It is a command (i.e. UCI button type option).
        engine.performCommand(name);
}

void UCIUtility::mainLoop(Engine& engine, std::istream& in, std::ostream& out)
{
    while (true)
    {
        std::string line, token;
        if (!std::getline(in, line))
            line = "quit";
        std::istringstream ss(line);
        ss >> std::skipws;
    retry:
        ss >> token;
        if (token == "quit")
            break;
        using UCICommandHandler = std::function<void(Engine&, std::istringstream&, std::ostream&)>;
        const std::unordered_map<std::string, UCICommandHandler> handlers =
            {
                { "uciok",     UCIUtility::uciokHandler },
                { "debug",     UCIUtility::debugHandler },
                { "setoption", UCIUtility::setoptionHandler }
            };
        if (handlers.find(token) != handlers.end())
            handlers.at(token)(engine, ss, out);
        else if (ss)
            goto retry; // An unknown token has been encountered and this reads a new one from the stream.
        else
            out << "Unknown UCI command." << std::endl;
    }
}
