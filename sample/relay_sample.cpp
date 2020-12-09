#include <chrono>
#include <thread>
#include <open_greenery/gpio/GPIOFactory.hpp>
#include "open_greenery/relay/Relay.hpp"

namespace ogio = open_greenery::gpio;
namespace ogr = open_greenery::relay;
namespace chr = std::chrono;
namespace thr = std::this_thread;

int main()
{
    auto pin = ogio::GPIOFactory::getInstance().getOutputGPIOctl({7, ogio::Pinout::WIRING_PI});
    ogr::Relay LED(pin);
    for (std::uint8_t i{0u}; i < 3u; ++i)
    {
        LED.enable();
        thr::sleep_for(chr::milliseconds(1500));
        LED.disable();
        thr::sleep_for(chr::milliseconds(1500));
    }

    return 0;
}
