package org.example;

import io.prometheus.metrics.core.metrics.Gauge;

public class Metric {
    private Integer gardenId;
    private Integer temperature;
    private Integer airHumidity;
    private Integer soilMoisture;

    private static final Gauge temperatureMetric = Gauge.builder()
            .name("temperature")
            .labelNames("garden_id")
            .register();

    private static final Gauge airHumidityMetric = Gauge.builder()
            .name("air_humidity")
            .labelNames("garden_id")
            .register();

    private static final Gauge soilMoistureMetric = Gauge.builder()
            .name("soil_moisture")
            .labelNames("garden_id")
            .register();

    @Override
    public String toString() {
        return String.format("graden_id: %d   temperature: %d   airHumidity: %d   soilMoisture: %d",
                gardenId, temperature, airHumidity, soilMoisture);
    }

    public Integer getGardenId(){
        return this.gardenId;
    }
    public Integer getTemperature(){
        return this.temperature;
    }
    public Integer getAirHumidity(){
        return this.airHumidity;
    }
    public Integer getSoilMoisture(){
        return this.soilMoisture;
    }

    public void observeMetrics() {
        temperatureMetric.labelValues(gardenId.toString()).set(temperature);
        airHumidityMetric.labelValues(gardenId.toString()).set(airHumidity);
        soilMoistureMetric.labelValues(gardenId.toString()).set(soilMoisture);
    }

}
