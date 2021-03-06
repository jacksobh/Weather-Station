#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x??, 0x??, 0x??, 0x??, 0x??, 0x?? }; //input your MAC Adress

IPAddress ip(000, 000, 0, 000); // Assign an IP Addrss

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

  // For analog reading
   #define LightSensor A0
   #define DHTSensor 7
   #define DHTValue DHT22
   float LightValue = 0.0;
     

   DHT dht(DHTSensor,DHTValue);
void setup() {

   
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  dht.begin();
}



void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
            
          // Equations for sensors
          LightValue = analogRead(LightSensor);
          // DHT sensor
          float humidity = dht.readHumidity();
          float temperature = (1.8) * dht.readTemperature() + 32.0;
          
          // Start of HTML
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
           client.println("<body bgcolor=#ff9966");
             client.println("<head><font face= Georgia color = green>");
               client.println("This is Ben Jackson");
                client.println("<br>");
              client.println("<center><h1><h1>Weather Currently</h1><head><br>");

          client.print("<h3>The current time of the day:  ");
         if (LightValue <= 390)
          {
            client.print("Night");
          }
          else if (LightValue < 750)
          {
             client.println("Dawn or Dusk");
          }
          else
          {
             client.println("Day");
          }
           client.println("<br>");
                   
                  client.print("<center>It is currently:  ");
                  client.println(temperature);
                  client.println("&deg;");
                  client.println(" outside with a humidity of ");
                  client.println(humidity);
                  client.println("%.");
                  client.println("<br>");
                  client.println("Check the extended weather in your area ");
                  client.println("<a href=http://www.weather.com target=_blank>here</a>");
                  client.println("<br>");
                  client.println("<br>");

//Will tell you what you should wear
                  if (temperature < 45)
                       {
                        if(humidity < 30)
                        {
                         client.println("It pretty chilly with a dry cold.<br>I would suggest wearing winter jacket with thick pants.");
                        }
                        else if(humidity < 60)
                        {
                             client.println("It's cold outside.<br>I would suggest wearing winter jacket today.");
                        }
                        else
                        {
                           client.println("It's a mild temperature but it also might be raining.<br>I would suggest wearing winter jacket as well as the radar to see if it's going to rain.");
                        }
                        
                       }
                    
                   if (temperature < 80)
                      {
                        if(humidity < 30)
                        {
                         client.println("It's a good temperature.<br>A good time to wear shorts and a long sleeve.");
                        }
                        else if(humidity < 60)
                        {
                             client.println("It's a great temperature.<br>I would suggest wearing shorts and a t-shirt or a long sleeve.");
                        }
                        else
                        {
                          client.println("It is quite humid outside.<br>I would suggest wearing shorts and a short sleeve. Also, check to radar to see if it's going to rain.");
                        }
                        
                       }
                  else
                       {
                       client.println("It's hot outside.<br>I would suggest wearing a quick dry shirt.");
                       }
                       

             client.println("<h3/></font></html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(5);
    // close the connection:
    client.stop();
   // Serial.println("client disconnected");
   // Serial.println(LightValue); // to figure out the else statments
    
    Ethernet.maintain();
  }
}

