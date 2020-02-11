#include <chrono>
#include <cstdint>
#include <iostream>
#include "open_greenery/sensor/AnalogSensor.hpp"
#include "open_greenery/adc/ADCFactory.hpp"

namespace og = open_greenery;

int main()
{
    const auto adc_address = og::driver::ADS1115::Address::GND;
    const auto adc_mode = og::driver::ADS1115::MUX::SINGLE_0;
    const auto reader = og::adc::ADCFactory::getInstance().getReader(adc_address, adc_mode);
    const auto period = std::chrono::milliseconds(500);

    open_greenery::sensor::AnalogSensor sensor(reader, period);
    og::sensor::AnalogSensor::Notificator ntr = [](const std::uint16_t _val){std::cout << _val << std::endl;};
    sensor.subscribe(ntr);

    // Sleep main thread; Still print notifications from sensor
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
