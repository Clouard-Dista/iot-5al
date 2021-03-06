# Projet IOT : Sensors Dashboard

> Partcipants : NOIRET Rémy, TALLOIR Vincent, COUVREUR Mathys, LACROIS Gaëtan

![Alt text](img/gauge.png?raw=true "jauges")

## Description

Our project aim is to be able to get thermal, humidity and gas measurement data with sensors and display these data in a webpage with some graph:

* Temperature (in °C).
* Humidity rates (in %).
* Measure of Propane, Hydrogen, Methane, Liquefied Petroleum Gas, Tin dioxide (in ppm/10).

To access the web application go to card_IP/index.html.
The web application is provided with data which can be found in JSON at card_IP/sensor.html.
Both webpage cant be load simultaneously.

A Thermometer contains button which allows stop sensor and LED.

Data will be display on a Web interface and the device will contains 3 LED :

*  Red: Warning when a issue arise on at least 2 sensors.
*  Yellow: Warning when a issue arise with a sensors.
*  Green: no issues with sensors.

The temperature sencor is consireded to have an issue if the temperature is over 30°C.
The humidity sensor is considered to have an issue if the humidity is under 20%.
The gaz sensor is considered to have an issue if there is over 750 ppm ( or 75 ppm/10).

## How does it work ?
At the start, the Arduino card connect on a WiFi network and modify SSID and Password in Arduino Code.
If you need to access externaly you need a static ip adresse and  open http port (80) on your rooter (you can also use a VPN bridge for security reason).

![Alt text](img/plan.png?raw=true "jauges")

## Demos 

Video : https://drive.google.com/file/d/1anOhr3EHaznXVS37VgCjXTeWFHOaMWAH/view?usp=drivesdk

## Hardware 

| Reference        | Product     | Qty  | Price
| ------------- |:-------------:| -----:|-----:| 
| ESP8266      | WEMOS D1 Mini ESP8266 Microcontroller | 1 | 3.99€      |
| KY-015      | Button Red Diameter: 22 mm for 12 ou 24 Volts     |   1 |  3.90€     |
| KY-015      | t° and humidity sensor DHT-11      |   1 |  2.07€     |
| MQ-2  | Gas Sensor : MQ-2 (mesure de Propane, Hydrogen, Methane, Gaz de pétrole liquéfié, Dioxyde d'étain)     |   1 | 4.30€      |

## Environements

Clone project : <https://github.com/Clouard-Dista/iot-5al.git>

If you don't have arduino :

1. download it here: https://www.arduino.cc/en/main/software
2. If you use windows/OSx you will probably need drivers: https://wiki.wemos.cc/downloads

## Start Arduino IDE

Setup the board :

1. Open the Preferences window
2. In the Additional Board Manager URLs field, enter this URL : http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Open Tools -> Board -> Boards Manager...
4. Type esp and the esp8266 platform will appear, install it
5. Select your board: Tools -> Board -> Wemos D1 R2 & mini


## Add external libraries

The project uses the following libraries :

    - Arduino
        - Adafruit Unified Sensors
        - DHT snsor library
        - SPIFFSReadServer (see : https://tttapa.github.io/ESP8266/Chap11%20-%20SPIFFS.html for implementation)
    - Web Library:
        - Highchart: Library Js for graphs in Web application
        - D3.js: Library for gauge in application

## Developpement

    • Thuesday :
        ◦ Coding to testing red button and LED
        ◦ Beginning to implement audio sensor
        ◦ Implementation Hydro sensor and heat sensor 
    • Wednesday :
        ◦ Create Web Application gauge and display the values of sensors
        ◦ Implementation to gas sensor
        ◦ try to implement audio sensor and gave up
        ◦ Beginning of the implementation of different LED color
    • Thursday:
        ◦ Modelisation and printing 3D case
        ◦ Finalisation of the implementation of Different LED Color
        ◦ add Graph in the applications

## Issues

Thursday:
- Issue with LED color to display the good color when a issue arise
- Issue with Highchart due to data being send asynchronously. This feature has been disabled due to lack of time to fix it
- SSL certificat for push SMS push API

## Contributions

- Hardware and Adruino code : TALLOIR Vincent
- Web app : COUVREUR Mathys, LACROIS Gaëtan
- Arduino code & front binding (SMS API was removed)




