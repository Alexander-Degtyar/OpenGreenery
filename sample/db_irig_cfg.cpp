#include <iostream>
#include <open_greenery/database/IrrigationConfigReader.hpp>
#include <open_greenery/database/IrrigationConfigWriter.hpp>
#include <open_greenery/database/SensorReader.hpp>
#include <open_greenery/gpio/PinId.hpp>

namespace og = open_greenery;

int main()
{
    std::cout << "Create/open DB" << std::endl;
    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/open_greenery.db3", SQLite::OPEN_READWRITE);
    std::cout << "SQLite database file " << db->getFilename() << " opened successfully" << std::endl;
    og::database::IrrigationConfigWriter writer ({db, "IrrigationConfig"});

    open_greenery::gpio::PinId pin {0, og::gpio::Pinout::WIRING_PI};
    std::int16_t dry {13500};
    std::int16_t wet {8000};
    std::uint16_t watering_volume {10};
    std::chrono::seconds watering_period {5};
    const std::string soil_moisture_table {"A3"};
    open_greenery::database::SensorReader soil_moisture_reader ({db, soil_moisture_table});
    writer.write({pin, dry, wet, watering_volume, watering_period, "A3"});

    og::database::IrrigationConfigReader reader ({db, "IrrigationConfig"});
    const auto record = reader.read(pin);
    std::cout << "Pin: " << record.pin.pin << " Dry: " << dry << " Wet: " << wet << " Vol.: " << watering_volume
        << " Period: " << watering_period.count() << " Reader: " << soil_moisture_table;

    return 0;
}