#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "open_greenery/driver/ads1115/ADS1115.hpp"
#include "open_greenery/gpio/GPIOFactory.hpp"

namespace ogd = open_greenery::driver;
namespace ogio = open_greenery::gpio;
namespace chr = std::chrono;
using clk = chr::system_clock;
namespace thr = std::this_thread;

int main()
{
    std::cout << "Hello OpenGreenery!" << std::endl;

    auto & iof = ogio::GPIOFactory::getInstance();

    // LED blink sample
    const auto LED = iof.getOutputGPIOctl({0u, ogio::Pinout::WIRING_PI});
    for (std::uint8_t i {0u}; i < 10u; ++i)
    {
        LED->write(ogio::LogicLevel::HIGH);
        thr::sleep_for(chr::milliseconds(100));

        LED->write(ogio::LogicLevel::LOW);
        thr::sleep_for(chr::milliseconds(100));
    }

    // Digital sensor sample
    const auto water_sensor = iof.getInputGPIOctl({2u, ogio::Pinout::WIRING_PI}, ogio::Pull::DOWN);
    clk::time_point timeout {clk::now() + chr::seconds(60)};
    while (clk::now() < timeout)
    {
        if (water_sensor->read() == ogio::LogicLevel::LOW)
            std::cout << "WATER";
        std::cout << std::endl;
    
        thr::sleep_for(chr::milliseconds(10));
<<<<<<< HEAD
    }

    // ADC reading sample
    try
    {
        ogd::ADS1115 adc (ogd::ADS1115::Address::GND);
        std::cout << "GND:             " << adc.read(ogd::ADS1115::Channel::A0) << std::endl;
        std::cout << "3.3V after 1kOm: " << adc.read(ogd::ADS1115::Channel::A1) << std::endl;
        std::cout << "3.3V after 2kOm: " << adc.read(ogd::ADS1115::Channel::A2) << std::endl;
    }
    catch(const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
=======
>>>>>>> origin/master
    }

    return 0;
}
