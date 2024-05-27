CREATE TABLE metrics (
    id SERIAL PRIMARY KEY,
    time timestamp,
    garden_id integer,
    temperature integer,
    airhumidity integer,
    soilmoisture integer
);