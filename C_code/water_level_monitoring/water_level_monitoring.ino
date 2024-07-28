ultrasonic sensor

/*

// Take readings from the ultrasonic sensor.

// Send the readings to the database. 

// Database  --> Visualize the data.

// Send a notification when the level reaches a critical point.

*/

// WiFi and PubSubClient libraries

#include <WiFi.h>

#include <PubSubClient.h>


// WiFi and Password // MQTT server Connection.


// WiFi Credentials.

const char *ssid = "WIFI_SSID";            // Replace with your WiFi name.

const char *password = "WIFI_PASSWORD";  // Replace with your WiFi password.

// MQTT Broker Settings.

const char *mqtt_broker = "broker.emqx.io";

const char *mqtt_topic = "water_level";

const char *mqtt_username = "emqx";

const char *mqtt_password = "public";

const int mqtt_port = 1883;


//MQTT TOPICS.

const char* = "river/highland/tank_one/level";

const char* = "river/middle/tank_two/level";

const char* = "river/lowland/tank_three/level";



// Ultrasonic sensor_one at the highlands.

const int trig_one = 1;

const int echo_one = 2;

// An led that will alert us when the first jerican is half full---> [Notification: High rainfall being experienced in the highlands] [AT]

const int led_highlands = 13;



// Ultrasonic sensor two. [Middle section]

int trig_two = 3;

int echo_two = 4;

//An led to notify the visually impared that it will flood in low lands.

const int led_alert = 8; 

// buzzer for those who can only hear to know it will flood in the low lands

const int buzzer1 = 9;

// Ultrasonic sensor three. [Lowland section] // Led to notify when the last jerican is almost full.

const int trig_three = 5;

const int echo_three = 6;

// defining the speed of sound

const int SOUND_SPEED = 0.034;


const int trig_four = 10;

const int echo_four = 



void setup() {
    
// put your setup code here, to run once:
    
    Serial.begin(115200);
    
// SETUP THE WIFI AND MQTT. 


    
    connectToWiFi();
    
    mqtt_client.setServer(mqtt_broker, mqtt_port);
    
    mqtt_client.setKeepAlive(60);
    
    mqtt_client.setCallback(mqttCallback); // Corrected callback function name
    
    connectToMQTT();
    
    // Initialize the pins for all ultrasonic sensors / led and buzzer.
  
    pinMode(trig_one, OUTPUT);
    pinMode(echo_one, INPUT);
    
    pinMode(trig_two, OUTPUT);
    pinMode(echo_two, INPUT);
    
    pinMode(trig_three, OUTPUT);
    pinMode(echo_three, INPUT);

    pinMode(trig_four, OUTPUT);
    pinMode(echo_four, INPUT);
    
    pinMode(led_six, OUTPUT);
    
    pinMode(buzzer1, OUTPUT);

    

}

void connectToWiFi() {

    delay(10);

    Serial.println();
  
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
      
        delay(500);
        
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
}





void connectToMQTT() {
    while (!mqtt_client.connected()) {
      
        String client_id = "esp32-client-" + String(WiFi.macAddress());
        
        Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
        
        if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          
            Serial.println("Connected to MQTT broker");
            
            mqtt_client.subscribe(mqtt_topic);
            
            mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP32 ^^"); // Publish message upon successful connection
            
        } else {

            Serial.print("Failed, rc=");
            
            Serial.print(mqtt_client.state());
            
            Serial.println(" try again in 5 seconds");
            
            delay(5000);
        }
    }
}

void loop() {

    // put your main code here, to run repeatedly:

    if (!mqtt_client.connected()){

      connectToMQTT(); 
      
      }
    
    // Get distances from each sensor for each tank
    
    float distanceTank1 = getDistance(trig_one, echo_one);
    
    float distanceTank2 = getDistance(trig_two, echo_two);
    
    float distanceTank3 = getDistance(trig_three, echo_three);

    float distanceTank4 = getDistance(trig_four, echo_four);
    
    // Print the distances for each tank
    
    Serial.print("Tank 1 Distance: ");
    Serial.print(distanceTank1);
    Serial.println(" cm");
    

    
    Serial.print("Tank 2 Distance: ");
    Serial.print(distanceTank2);
    Serial.println(" cm");

    
    
    Serial.print("Tank 3 Distance: ");
    Serial.print(distanceTank3);
    Serial.println(" cm");


    Serial.print("Tank 4 Distance: ");
    Serial.print(distanceTank4);
    Serial.println(" cm");

  mqtt_client.publish(tank1_topic, String(distanceTank1).c_str(), true);
  mqtt_client.publish(tank2_topic, String(distanceTank2).c_str(), true);
  mqtt_client.publish(tank3_topic, String(distanceTank3).c_str(), true);
  mqtt_client.publish(tank4_topic, String(distanceTank4).c_str(), true);
    
    
    
    delay(1000); // Wait for 1 second before next reading


    
    
    }
    






























  

}
