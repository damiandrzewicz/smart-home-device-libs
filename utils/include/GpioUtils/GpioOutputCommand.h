#pragma once

class GpioOutputCommand
{
public:
    virtual void on() = 0;

    virtual void off() = 0;

protected:
    virtual void init() = 0;
};