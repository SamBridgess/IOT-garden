#pragma once

#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "metrics.hpp"

namespace rand_vals {
    extern bool air_moisture_flag;
    extern bool soil_moisture_flag;
    extern uint8_t last_air_moisture;
    extern uint8_t last_soil_moisture;

    void init (size_t);

    metrics::temperature temperature ();
    metrics::air_moisture air_moisture ();
    metrics::soil_moisture soil_moisture ();
}  // namespace random