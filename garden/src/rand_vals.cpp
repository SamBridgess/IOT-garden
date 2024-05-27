#include "rand_vals.hpp"

namespace rand_vals {

    void init (size_t num)
    {
        srand(time(0) + num);
    }

    metrics::temperature temperature ()
    {
        double temp = MIN_NORMAL_TEMPERATURE
                    + static_cast<double>(rand())
                          / (static_cast<double>(
                              RAND_MAX / (MAX_NORMAL_TEMPERATURE - MIN_NORMAL_TEMPERATURE)));
        if ( rand() % 10 == 3 ) {
            double deviation = temp * 0.1;
            temp += (rand() % 2) ? deviation : -deviation;
        }

        return metrics::temperature { temp };
    }

    metrics::air_moisture air_moisture ()
    {
        if ( air_moisture_flag ) {
            last_air_moisture = 0;
            air_moisture_flag = false;
        } else {
            ++last_air_moisture;
            if ( last_air_moisture > 4 ) {
                air_moisture_flag = true;
            }
        }

        uint8_t moisture = 80 - (last_air_moisture * last_air_moisture);
        if ( rand() % 5 == 1 ) {
            moisture *= 0.95;
        }

        return metrics::air_moisture { moisture };
    }

    metrics::soil_moisture soil_moisture ()
    {
        if ( soil_moisture_flag ) {
            last_soil_moisture = 0;
            soil_moisture_flag = false;
        } else {
            ++last_soil_moisture;
            if ( last_soil_moisture > 10 ) {
                soil_moisture_flag = true;
            }
        }

        uint8_t moisture = 60 - (last_soil_moisture * last_soil_moisture) / 5;
        if ( rand() % 5 == 1 ) {
            moisture *= 0.95;
        }

        return metrics::soil_moisture { moisture };
    }
}  // namespace rand_vals