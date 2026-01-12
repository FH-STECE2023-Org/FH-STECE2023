#pragma once

#include "eventloop.h"

class GracefulTerminator : public InputHandler
{
public:
    GracefulTerminator();
    ~GracefulTerminator() override;

    void hookup(Eventloop&);
    EventAction ready(int fd) override;

private:
    int _fd;
};