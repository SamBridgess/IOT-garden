#include "generators.hpp"

namespace generators {

    Generator<metrics::temperature> temperature_generator ()
    {
        for ( ;; ) {
            co_yield rand_vals::temperature();
        }
    }

    Generator<metrics::air_moisture> air_moisture_generator ()
    {
        for ( ;; ) {
            co_yield rand_vals::air_moisture();
        }
    }

    Generator<metrics::soil_moisture> soil_moisture_generator ()
    {
        for ( ;; ) {
            co_yield rand_vals::soil_moisture();
        }
    }

    Generator<metrics::garden_info> garden_info_generator ()
    {
        for ( ;; ) {
            co_yield metrics::garden_info { rand_vals::temperature(), rand_vals::air_moisture(),
                                            rand_vals::soil_moisture() };
        }
    }

}  // namespace generators