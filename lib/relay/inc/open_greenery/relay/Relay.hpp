#ifndef RELAY_HPP
#define RELAY_HPP

#include <memory>
#include "open_greenery/gpio/IOutputPin.hpp"

namespace open_greenery
{
namespace relay
{

class Relay
{
public:
    Relay(std::shared_ptr<open_greenery::gpio::IOutputPin> _pin);

    ~Relay();

    void enable();

    void disable();

    bool enabled() const;

    void toggle();

private:
    bool m_enabled;
    std::shared_ptr<open_greenery::gpio::IOutputPin> m_gpio;
};

}
}


#endif //RELAY_HPP
