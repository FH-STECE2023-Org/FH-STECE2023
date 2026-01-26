#pragma once
#include "structs.h"
#include <door/motor/motor.h>
#include "stdbool.h"

class Outputs
{
private:
    std::shared_ptr<Motor> _motor;
    
public:
    Outputs(std::shared_ptr<Motor> motor); // TODO: display could be added
    void set_outputs(const output_t out);
};
