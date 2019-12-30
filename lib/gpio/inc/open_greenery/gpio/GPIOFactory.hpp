#ifndef GPIO_FACTORY_HPP
#define GPIO_FACTORY_HPP

#include <map>
#include <memory>
#include "InputGPIOctl.hpp"
#include "OutputGPIOctl.hpp"
#include "PinId.hpp"
#include "Pull.hpp"

namespace open_greenery
{
namespace gpio
{

class InputGPIOctl;
class OutputGPIOctl;

class GPIOFactory
{
public:
    static GPIOFactory & getInstance();

    GPIOFactory(const GPIOFactory &) = delete;
    GPIOFactory & operator=(GPIOFactory &) = delete;

    std::shared_ptr<InputGPIOctl> getInputGPIOctl(const PinId _pin, const Pull _pull);
    std::shared_ptr<OutputGPIOctl> getOutputGPIOctl(const PinId _pin);
private:
    GPIOFactory() = default;
    std::map<PinNumber, std::shared_ptr<GPIOctl>> m_ctls;
};

}
}

#endif //GPIO_FACTORY_HPP
