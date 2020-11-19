#ifndef INPUT_GPIO_CTL_HPP
#define INPUT_GPIO_CTL_HPP

#include "GPIOFactory.hpp"
#include "GPIOctl.hpp"
#include "IInputPin.hpp"
#include "LogicLevel.hpp"
#include "PinId.hpp"
#include "Pull.hpp"

namespace open_greenery
{
namespace gpio
{

class InputGPIOctl final : public IInputPin, public GPIOctl
{
public:
    InputGPIOctl(const PinId _pin, const Pull _pull);
    ~InputGPIOctl() override;
    LogicLevel read() const override;
};

}
}

#endif //INPUT_GPIO_CTL_HPP
