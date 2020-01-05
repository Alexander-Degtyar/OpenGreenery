#include <chrono>
#include <iostream>
#include <thread>
#include "open_greenery/gpio/GPIOFactory.hpp"

namespace ogio = open_greenery::gpio;
namespace chr = std::chrono;
using clk = chr::system_clock;
namespace thr = std::this_thread;
int main()
{
    auto & iof = ogio::GPIOFactory::getInstance();
    // LED blink sample
    const auto LED = iof.getOutputGPIOctl({0u, ogio::Pinout::WIRING_PI});
    for (std::uint8_t i{0u}; i < 10u; ++i)
    {
        LED->write(ogio::LogicLevel::HIGH);
        thr::sleep_for(chr::milliseconds(100));

        LED->write(ogio::LogicLevel::LOW);
        thr::sleep_for(chr::milliseconds(100));
    }

    // Digital sensor sample
    const auto water_sensor = iof.getInputGPIOctl({2u, ogio::Pinout::WIRING_PI}, ogio::Pull::DOWN);
    clk::time_point timeout{clk::now() + chr::seconds(60)};
    while (clk::now() < timeout)
    {
        if (water_sensor->read() == ogio::LogicLevel::LOW)
            std::cout << "WATER";
        std::cout << std::endl;

        thr::sleep_for(chr::milliseconds(10));
    }

    return 0;
}
