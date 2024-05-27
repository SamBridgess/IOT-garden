#pragma once

#include "generator.hpp"
#include "rand_vals.hpp"

namespace generators {

    Generator<metrics::temperature> temperature_generator ();
    Generator<metrics::air_moisture> air_moisture_generator ();
    Generator<metrics::soil_moisture> soil_moisture_generator ();
    Generator<metrics::garden_info> garden_info_generator ();

}  // namespace generators