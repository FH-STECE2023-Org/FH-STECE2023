#pragma once


class Motor
{
public:
    enum class Direction
    {
        IDLE,
        FORWARD,
        BACKWARD,
    };

    virtual ~Motor() = default;

    virtual void forward() = 0;
    virtual void backward() = 0;
    virtual void stop() = 0;

    // for tests only? (Door uses for a sanity check)
    virtual Direction get_direction() const = 0;
};

