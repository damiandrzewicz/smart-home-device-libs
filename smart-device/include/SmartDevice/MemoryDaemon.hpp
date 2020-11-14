#pragma once

#include <functional>

#include "RtosUtils/RoutineTask.hpp"

namespace SmartDevice
{
    class MemoryDaemon : public RoutineTask
    {
    public:
        MemoryDaemon();

        void setUpdateMemoryStateFunction(std::function<void(int, int)> fun);

    protected:
        virtual void task() override;

    private:

    private:
        unsigned short _maxHeapSize = 520; //KB

        std::function<void(int, int)> _updateMemoryState;
    };
};