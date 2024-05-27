#pragma once

#include <inttypes.h>
#include <string>

#include "jsonable.hpp"

namespace metrics {

    struct temperature : jsonable::helper<temperature> {
        std::string name = "temperature";
        double _value = 0;

        explicit temperature(double degrees) : _value(degrees) { }

        temperature() = default;
    };

    struct air_moisture : jsonable::helper<air_moisture> {
        std::string name = "airHumidity";
        uint8_t _value;

        explicit air_moisture(uint8_t percents) : _value(percents) { }

        air_moisture() = default;
    };

    struct soil_moisture : jsonable::helper<soil_moisture> {
        std::string name = "soilMoisture";
        uint8_t _value;

        explicit soil_moisture(uint8_t percents) : _value(percents) { }

        soil_moisture() = default;
    };

    struct garden_info : jsonable::jsonable_interface {
        temperature _temperature;
        air_moisture _air_moisture;
        soil_moisture _soil_moisture;
        size_t _garden_id = 0;

        garden_info(temperature temperature, air_moisture air_moisture, soil_moisture soil_moisture)
            : _temperature(temperature), _air_moisture(air_moisture), _soil_moisture(soil_moisture)
        {
        }

        garden_info() = default;

        std::string const virtual convert_to(uint8_t tabs, bool last) const override
        {
            return std::string(tabs, ' ') + "{\n" + std::string(tabs + 4, ' ') + "\"gardenId\": " + std::to_string(_garden_id) + ",\n" + _temperature.convert_to(tabs + 4, false) + _air_moisture.convert_to(tabs + 4, false)
                 + _soil_moisture.convert_to(tabs + 4, true) + std::string(tabs, ' ') + "}" + (last ? "\n" : ",\n");
        }
    };

}  // namespace metrics