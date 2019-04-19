# Projet IOT : tableau de bord sondes

> Partcipants : NOIRET Rémy, TALLOIR Vincent, COUVREUR Mathys, LACROIS Gaëtan

![Alt text](img/gauge.png?raw=true "jauges")

## Description

Topic of the project have made a thermometer and display:

* Temperature
* Humidity rates
* Measure of Propane, Hydrogen, Methane, Gaz de pétrole liquéfié, Dioxyde 'étain.

A Thermometer is composed a button who allows stop sensor and LED.

Data will be display on a Web interface but the device will be make up 3 LED :

*  Red: Warning when a issue arise at least 2 sensors.
*  Yellow: Warning when a issue arise with a sensors.
*  Green: no any issues with sensors.

## How does it work ?
At the start, the Arduino card connects a WiFi network and modify SSID and Password in Arduino Code.

![Alt text](img/plan.png?raw=true "jauges")

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
2. If you use windows/OSx you will probably need drivers: https://www.wemos.cc/downloads

## Start Arduino IDE

Setup the board :

1. Open the Preferences window
2. In the Additional Board Manager URLs field, enter this URL : http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Open Tools -> Board -> Boards Manager...
4. Type esp and the esp8266 platform will appear, install it
5. Select your board: Tools -> Board -> Wemos D1 R2 & mini


## Add external libraries

The project uses the following libraries :

    - Arduino Library
        - Adafruit_Circuit_Playground
        - Bridge
        - Esplora
        - Ethernet
        - Firmata
        - GSM
        - Keyboard
        - LiquidCrystal
        - Mouse
        - Robot_Control
        - Robot_Motor
        - RobotIRremote
        - SD
        - Servo
        - SpacebrewYun
        - Stepper
        - Temboo
        - TFT
        - WiFi
    - Web Library:
        - Highchart: Library Js for graphs in application Web
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
- Issue compatibility between Highchart and D3js to display gauge and graph in a same web application


