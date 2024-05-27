package org.example;

import java.sql.*;

public class DatabaseWriter {
    Connection connection;
    public DatabaseWriter(String url, String username, String password) throws SQLException {
        this.connection = DriverManager.getConnection(url, username, password);
    }

    public void writeToDB(Metric metric) throws SQLException {
        Timestamp timestamp = new Timestamp(System.currentTimeMillis() - 3*3600000);

        String sql = "INSERT INTO metrics (time, garden_id, temperature, airHumidity, soilMoisture) VALUES (?, ?, ?, ?, ?)";


        PreparedStatement statement = connection.prepareStatement(sql);
        statement.setTimestamp(1,timestamp);
        statement.setInt(2, metric.getGardenId());
        statement.setInt(3, metric.getTemperature());
        statement.setInt(4, metric.getAirHumidity());
        statement.setInt(5, metric.getSoilMoisture());

        statement.executeUpdate();

        System.out.println(metric);
        System.out.println("Statement executed");
    }
}
