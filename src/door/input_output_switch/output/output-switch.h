#pragma once

class OutputSwitch {
    public:
        enum class State {
            OUTPUT_LOW,
            OUTPUT_HIGH,
        };
    
        OutputSwitch() = default;
        virtual ~OutputSwitch() = default;
    
        // Forbid copying (important for classes that own hardware resources)
        OutputSwitch(const OutputSwitch&) = delete;
        OutputSwitch& operator=(const OutputSwitch&) = delete;
    
        // Also forbid moving (this interface has no value semantics)
        OutputSwitch(OutputSwitch&&) = delete;
        OutputSwitch& operator=(OutputSwitch&&) = delete;
    
        // Must be implemented by all concrete classes
        virtual void set_state(State state) = 0;
    };