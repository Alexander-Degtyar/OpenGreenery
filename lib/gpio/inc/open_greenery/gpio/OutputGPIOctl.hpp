#ifndef OUTPUT_GPIO_CTL_HPP
#define OUTPUT_GPIO_CTL_HPP

#include "GPIOctl.hpp"
#include "LogicLevel.hpp"
#include "PinId.hpp"

namespace open_greenery
{
namespace gpio
{

class OutputGPIOctl final : GPIOctl
{
public:
    OutputGPIOctl(const PinId _pin);
    ~OutputGPIOctl() override final;
    void write(const LogicLevel _value) const;
};

}
}

#endif //OUTPUT_GPIO_CTL_HPP