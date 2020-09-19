#include <csignal>
#include <SQLiteCpp/SQLiteCpp.h>
#include <iostream>
#include <forward_list>
#include <open_greenery/database/SensorReader.hpp>
#include <open_greenery/database/IrrigationConfigReader.hpp>
#include <open_greenery/pump/Pump.hpp>
#include <open_greenery/irrigation/IrrigationController.hpp>

namespace og = open_greenery;
namespace ogdb = open_greenery::database;
namespace ogdf = open_greenery::dataflow;

static std::forward_list<og::irrigation::IrrigationController> s_controllers;

void signalHandler(int signal)
{
    std::for_each(s_controllers.begin(), s_controllers.end(), [](auto & ctl){ctl.stop();});
    exit(signal);
}

int main()
{
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

    std::shared_ptr<SQLite::Database> db;
    try
    {
        db = std::make_shared<SQLite::Database>("/home/pi/og/db/open_greenery.db3", SQLite::OPEN_READONLY);
    }
    catch (const SQLite::Exception & ex)
    {
        std::cerr << "Database exception: " << ex.getErrorStr() << std::endl;
        return -1;
    }

    std::shared_ptr<ogdf::IIrrigationConfigDataProvider> cfg_reader =
            std::make_shared<ogdb::IrrigationConfigReader>(ogdb::Table(db, "IrrigationConfig"));

    const auto cfg_records = cfg_reader->read();
    if (cfg_records.empty())
    {
        std::cerr << "Database doesn't contain irrigation configurations" << std::endl;
        return -2;
    }

    for (const auto & cfg : cfg_records)
    {
        std::cout << "----- Irrigation service configuration -----" << std::endl
                  << "Pump pin: " << int(cfg.pin.pin) << std::endl
                  << "Dry level: " << cfg.dry << " Wet level: " << cfg.wet << std::endl
                  << "Watering volume: " << cfg.watering_volume << " ml" << std::endl
                  << "Watering period: " << cfg.watering_period.count() << std::endl
                  << "Soil moisture sensor: " << cfg.soil_moisture_sensor << std::endl;

        std::shared_ptr<ogdf::ISensorReadProvider> soil_moisture_reader =
                std::make_shared<ogdb::SensorReader>(ogdb::Table(db, cfg.soil_moisture_sensor));
        std::shared_ptr<og::pump::IPump> pump =
                std::make_shared<og::pump::Pump>(cfg.pin);

        auto & ctl = s_controllers.emplace_front(cfg, soil_moisture_reader, pump);
        ctl.start();
    }
    while (true) {} // Execute service until termination signal
    return 0;
}