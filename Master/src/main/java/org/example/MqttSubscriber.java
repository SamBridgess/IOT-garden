package org.example;
import com.google.gson.Gson;
import io.prometheus.metrics.exporter.httpserver.HTTPServer;
import io.prometheus.metrics.instrumentation.jvm.JvmMetrics;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;


import java.sql.SQLException;


public class MqttSubscriber {


    public static void main(String[] args) throws SQLException {
        DatabaseWriter databaseWriter = new DatabaseWriter("jdbc:postgresql://database:5432/postgres", "postgres", "postgres");

        JvmMetrics.builder().register();

        String broker = "tcp://mqtt-broker:1883";
        String topic = "metrics";

        try(MqttClient client = new MqttClient(broker, MqttClient.generateClientId());
            HTTPServer metricsServer = HTTPServer.builder().port(1488).buildAndStart()) {
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    System.out.println("Connection lost: " + cause.getMessage());
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    System.out.println("Message arrived. Topic: " + topic + " Message: " + new String(message.getPayload()));

                    String msg = new String(message.getPayload());
                    Metric metric = new Gson().fromJson(msg, Metric.class);
                    databaseWriter.writeToDB(metric);
                    metric.observeMetrics();
                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {

                }
            });
            MqttConnectOptions opts = new MqttConnectOptions();
            opts.setKeepAliveInterval(10000000);
            opts.setAutomaticReconnect(true);
            opts.setConnectionTimeout(100);

            System.out.println("connecting");

            client.connect(opts);
            System.out.println("connected");

            client.subscribe(topic);

            System.out.println("Subscribed to topic: " + topic);

            while (true);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
