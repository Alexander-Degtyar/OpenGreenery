#include <iostream>
#include <string>
#include <thread>
#include <QDebug>
#include "open_greenery/adc/ADCFactory.hpp"
#include "open_greenery/database/SensorReader.hpp"
#include "open_greenery/database/SensorWriter.hpp"
#include "open_greenery/sensor/AnalogSensorPublisher.hpp"

namespace og = open_greenery;

int main ()
{
    // ADS1115 A0 port reader
    auto adc_reader = og::adc::ADCFactory::getInstance().getReader(
            og::driver::ADS1115::Address::GND,
            og::driver::ADS1115::MUX::SINGLE_0);
    constexpr auto period = std::chrono::milliseconds(500);

    // Publisher of ADC readers data each 0.5 sec
    og::sensor::AnalogSensorPublisher a0(adc_reader, period);

    // Open database file
    auto db = std::make_shared<SQLite::Database>("/tmp/sensor_data.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;

    const og::database::Table table(db, "A0");// Declare the database table for sensor data
    // Database sensor writer saves data to the table
    std::unique_ptr<og::dataflow::ISensorDataReceiver> receiver = std::make_unique<og::database::SensorWriter>(table);

    // Notificator that saves sensor data to the database
    og::sensor::AnalogSensorPublisher::Notificator ntr = [&receiver](const std::int16_t _val)
    {
        try
        {
            receiver->write(_val);
            std::cout << "ADC value [" << _val << "] successfully saved" << std::endl;
        }
        catch (std::exception &e)
        {
            std::cerr << "SQLite exception: " << e.what() << std::endl;
        }
    };
    a0.subscribe(ntr);// Transfer notificator to the A0 sensor
    // Pause main thread; Sensor publisher continues save ADC data via SensorWriter in the Notificator
    const auto from = QDateTime::currentDateTime();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    const auto to = QDateTime::currentDateTime();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    a0.stop();

    // Read saved to the DB data
    if (table.valid())
    {
        std::unique_ptr<og::dataflow::ISensorDataProvider> provider =
                std::make_unique<og::database::SensorReader>(table);
        for (const auto & pair : provider->read(from, to))
        {
            const auto [dt, val] = pair;
            qDebug() << dt << " | " << val;
        }
    }
    else
    {
        std::cerr << "Invalid table: " << table.name << std::endl;
    }

    return 0;
}
