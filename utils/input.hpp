#pragma once

#include <functional>
#include <optional>
#include <unordered_map>

namespace flsvrwin
{

template <typename Command> class InputHandler
{
  private:
    typedef std::function<void(int)> CommandFunc; // void(mods)

    struct CommandElement
    {
        std::optional<int> key  = std::nullopt;
        CommandFunc        func = {};
    };

    std::unordered_map<Command, CommandElement> commandList = {};

  public:
    InputHandler()  = default;
    ~InputHandler() = default;

    void callback(int key, int mods)
    {
        for (const auto& command : commandList)
        {
            auto& commandKey  = command.second.key;
            auto& commandFunc = command.second.func;

            if (!commandKey.has_value())
                continue;

            if (key == commandKey.value())
                // dont break here because there may be multiple commands with the same key
                commandFunc(mods);
        }
    }

    void sendCommand(Command cmd, int mods = 0x00)
    {
        if (commandList.contains(cmd))
            commandList[cmd].func(mods);
    }

    void addCommand(CommandFunc func, Command cmd, std::optional<int> key = std::nullopt)
    {
        commandList[cmd] = CommandElement{.key = key, .func = func};
    }
};

} // namespace flsvrwin
