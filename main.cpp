#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include "open_greenery/gpio/InputGPIOctl.hpp"
#include "open_greenery/gpio/OutputGPIOctl.hpp"

namespace ogio = open_greenery::gpio;
namespace chr = std::chrono;
using clk = chr::system_clock;
namespace thr = std::this_thread;

int main()
{
    std::cout << "Hello OpenGreenery!" << std::endl;

    // LED blink sample
    ogio::OutputGPIOctl LED ({0u, ogio::Pinout::WIRING_PI});
    for (std::uint8_t i {0u}; i < 10u; ++i)
    {
        LED.write(ogio::LogicLevel::HIGH);
        thr::sleep_for(chr::milliseconds(100));

        LED.write(ogio::LogicLevel::LOW);
        thr::sleep_for(chr::milliseconds(100));
    }

    // Digital sensor sample
    ogio::InputGPIOctl water_sensor ({0u, ogio::Pinout::WIRING_PI}, ogio::Pull::DOWN);
    clk::time_point timeout {clk::now() + chr::seconds(60)};
    while (clk::now() < timeout)
    {
        if (water_sensor.read() == ogio::LogicLevel::LOW)
            std::cout << "WATER";
        std::cout << std::endl;
    
        thr::sleep_for(chr::milliseconds(10));
    }

    return 0;
}
