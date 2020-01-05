#ifndef RELAY_HPP
#define RELAY_HPP

#include <memory>
#include "open_greenery/gpio/OutputGPIOctl.hpp"
#include "open_greenery/gpio/PinId.hpp"

namespace open_greenery
{
namespace relay
{

class Relay
{
public:
    Relay(const open_greenery::gpio::PinId _pin);

    ~Relay();

    void enable();

    void disable();

    bool enabled() const;

private:
    bool m_enabled;
    std::shared_ptr<open_greenery::gpio::OutputGPIOctl> m_gpio;
};

}
}


#endif //RELAY_HPP
