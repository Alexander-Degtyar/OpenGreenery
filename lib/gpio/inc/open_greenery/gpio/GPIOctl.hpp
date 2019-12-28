#ifndef GPIO_CTL_HPP
#define GPIO_CTL_HPP

#include "PinId.hpp"

namespace open_greenery
{
namespace gpio
{

class GPIOctl
{
public:
    virtual ~GPIOctl() = default;
protected:
    GPIOctl(const PinId _pin);
    PinNumber pin() const;
private:
    PinNumber m_pin;
};

}
}

#endif //GPIO_CTL_HPP
