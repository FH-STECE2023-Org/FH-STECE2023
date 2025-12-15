#pragma once

#include <map>

enum class EventAction 
{
    Continue,
    Quit,
};

class InputHandler
{
public:
    virtual ~InputHandler() = default;
    [[nodiscard]] virtual EventAction ready(int fd) = 0;
};

class PriorityHandler
{
public:
    virtual ~PriorityHandler() = default;
    [[nodiscard]] virtual EventAction ready(int fd) = 0;
};

class Eventloop
{
public:
    void register_input(int fd, InputHandler*);
    void register_priority(int fd, PriorityHandler*);
    void run();
    EventAction run_one();

private:
    std::map<int, InputHandler*> _input_handlers;
    std::map<int, PriorityHandler*> _priority_handlers;
};
