# arduino
Basic code for Adafruit Feather M0

* sketch_sep07a: Interaction with pressure and temperature sensor
* WiFiWebClientRepeating: Sending data to web client
* ConnectWithWPA: Connect to WiFi
* blink_wifi_101: let LED blink on Adafruit board
* server: backend-code to store the data and visualize it in a line-chart

A demo of the graph printing the data can be found here: [http://tf.xampit.de/chart.html](http://tf.xampit.de/chart.html)
It's nothing special, but the project helped us to make first contact with Arduino-programming.

For a working solution some sort of auto-leveling mechanism or a second sensor as reference point would be necessary as the air pressure changes a lot withing 24 hours (as you can see in the big drop in the graph below):

![Image of airpressure messurement over 15 hours](http://tf.xampit.de/chart.png)
