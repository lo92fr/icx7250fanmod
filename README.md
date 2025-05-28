# icx7250fanmod
A guide to Ruckus ICX7250-48-p fan mode

## Warning

The modification will of course wold warranty of your switch.
An can ever kill the switch if something goes wrong in your modification.
I'm myself have some wrong things happens durring this mods.
So lets be warn that it can cost new to buy a new switch if the worst case.
And that of course, I won't assume any dommage in regards of modification you've made !

## parts 

- an Esp32 board, any brand should make it.
- 2 mosfet driver (one for each fan group) :
   ICQUANZX 5PCS DC 5V-36V 15A (Max 30A) 400 W Dual MOSFET
   https://www.amazon.fr/dp/B07VRCXGFY?ref=ppx_yo2ov_dt_b_fed_asin_title

- 1 voltage regulator:
   ARCELI 5 Pcs Ultra Petit DC-DC 3A Puissance Module abaisseur réglable Convertisseur Buck 24V À 12V 9V 5V 3V Arduino
   https://www.amazon.fr/dp/B07MY2XP21?ref=ppx_yo2ov_dt_b_fed_asin_title

- 1 DHT 22 temperature and humidity sensor (optional):
   Lot de 5 capteurs de température DHT/2302-22 modules - Capteur d'humidité - Avec platine et câble - 3,3-5,5 V - Compatible avec Arduino, DHT, capteur numérique de température et d'humidité
   https://www.amazon.fr/dp/B0D25WCHWQ?ref=ppx_yo2ov_dt_b_fed_asin_title

- Some resistor : mainly 4,7 kOhm.
- Some capacitor : mainly 4,7 nF and also 10uF and/or 47uF electrolytic for mofset output.

## Introduction

I've owned a Ruckus ICX7250-48-p since now 2/3 years.
This is a very great swith, 48 gigabit port, POE, and 8 SFP+ port enabling 10gb communication.
But one matter about this switch, like many others, is that is mainly a data center switch, with very loud fan by default.

By default, the switch come with 3 Foxconn fan:
Brand : Foxconn
Model : PIA040H12P
Size  : 40 mm x 40 mm x 28 mm
RPM   : 18000 tr/min
Noise : 55.00 dB (A)
CFM   : 24.03 CFM

![WhatsApp Image 2025-05-28 à 16 57 00_db6bd5dc](https://github.com/user-attachments/assets/93780589-8178-4ea5-9a74-0bc924a8c2ed)


As you can see, high CFM, but with also very high RPM, and so heavy high frequency noise!

Mine was already have a first fan mode made by the first owner of the swith.
The 3 fan was replace by this model:

Brand : Mechatronics
Model : MR4020X12B1-RSR
RPM   : 12000 tr/min
Noise : 41,0dB(A)
CFM   : 17,3CFM (0,484m³/min)

![WhatsApp Image 2025-05-28 à 16 57 00_96b30e04](https://github.com/user-attachments/assets/745d29bf-3efd-4212-8b11-37a61835d91d)

As you can see, this model is running slower : 12000 RPM at max speed, but still have a good CFM value : 17,3, and lower noise production : 41,0 dB.

The matter is at full speed, this fan is still very noisy.
ICX 7250 only manage fan with two different speed : full 12 000 RPM speed, and a slower speed (around 5000 RPM if I remember).
When you start the switch, it will run at full speed until the OS fully load.
After this, it will switch two low speed.

The is some thermal rule two manage this 2 fan speed.
You can look at them by issue a "show chassis" command.
```
The stack unit 1 chassis info:

Power supply 1 (AC - PoE) present, status ok
Power supply 2 not present
Power supply 3 not present

Fan 1 ok, speed (auto): 1<->[[2]]
Fan 2 ok, speed (auto): 1<->[[2]]
Fan 3 ok, speed (auto): 1<->[[2]]

Fan controlled temperature:
        Rule 1/2 (MGMT THERMAL PLANE): 50.0 deg-C
        Rule 2/2 (AIR OUTLET NEAR PSU): 51.0 deg-C

Fan speed switching temperature thresholds:
        Rule 1/2 (MGMT THERMAL PLANE):
                Speed 1: NM<-----> 95       deg-C
                Speed 2:        85<----->105 deg-C (shutdown)
        Rule 2/2 (AIR OUTLET NEAR PSU)***active***:
                Speed 1: NM<-----> 41       deg-C
                Speed 2:        34<----->105 deg-C (shutdown)

Fan 1 Air Flow Direction:  Front to Back
Fan 2 Air Flow Direction:  Front to Back
Fan 3 Air Flow Direction:  Front to Back
Slot 1 Current Temperature: 50.7 deg-C (Sensor 1), 51.0 deg-C (Sensor 2)
Slot 2 Current Temperature: NA
        Warning level.......: 60.0 deg-C
        Shutdown level......: 105.0 deg-C
Boot Prom MAC : cc4e.24e1.811a
Management MAC: cc4e.24e1.811a
```

When the temperature is below the threshold, fan will go to low speed.
When the temperature is over, it will trigger to full speed (12000 RPM).
And if temperature reach the limit, it will shutdown (reboot) the switch.

The main matter with this is that you only have two speed, low and full.
Low is very quite, can hear it when I'm around 1 meter of the swithc.
Full if very loud, can hear the tv set correctly when it running at that speed !

My ICX 7250 is installed in a wall inclosure neer a window with low air flow inside the inclosure.
In winter, the ICX 7250 run at low speed near around all the time.
Comming to summer time, between May to September, the Switch start to toggle to full speed mode between 4pm to 21pm due to temperature increase :(


## My modifications

When previous owner sell me the switch, it add an extra sunon fan, but tell me it never take time to install it.

The fan spec is:
Brand : Sunon 
Model : MF60151V3-1000U-A99
Size  : 60x60x15mm
RPM   : 3000 RPM
Noise : 21.3 dBA
CFM   : 17.6CFM


![WhatsApp Image 2025-05-28 à 16 57 00_b2c32ff3](https://github.com/user-attachments/assets/441b6d54-a9cb-4125-b5ed-c445862de63c)

As you can see, this fan seems really interresting.
Good CFM level, quite the same as the Mechatronics one, but much lower RPM speed : only 3000 RPM, and very little noise : 21,3 dbA.
The only matter is that this is a 60x60 size fan, so you can't install it in the switch without modifications.

### Installation of the fan

My choice was to add two of this fans.
- One will be other the main 7250 asic, the one with the big radiator component.
  This is one we can see as "MGMT THERMAL PLANE" temperature.

- The other will be in front of the PSU where you have some available space.
  The idea is to lower the "IR OUTLET NEAR PSU" temperature.

Let's give a photos on this to make it more clear.
- The left fan will come other the big black radiator between the POE board and the fan header.
- The right fan will come between the small space between the managment card and the PSU.
   ![WhatsApp Image 2025-05-28 à 16 57 03_2d394f66](https://github.com/user-attachments/assets/e4f43a02-bb8f-4a73-a2b6-253e3cf2f74a)

To fit the fan inside the enclosure, I have no over choice that to cut the enclosure.
I use for this a dremel, with a metal cutting disc.
Note that if you don't want to do this, you have some other choices :
- Some are buying pexiglass to replace the existing enclosure.
- Some seems to have succes just putting the fan inside the enclosure without making any outside hool.

My choice was to mount the fan as intake fan, so it will make a flow between this 2 new fans, and the old 3 fans at the back of the unit.
Could not say if it's the better way to go, I've not tested the other configurations.

Also, make sure to get the good dimension before cutting the enclosure.
On the left fan, I've made a mistake, and as you can see, there is some space between the fan edge and the enclosure cuts !

### Electronics warns

When I start this mode, my first idea was not to made a full fan mode (with electronics change) like this one.
But I start to solder new wire on the fan header for the 2 new fans.
And my big mistake was two not disconnect the fan header before I've made the soldering.
Of course, switch was powered offline before the soldering.
But because of soldering temperature, or perharps becauses of soldering electricity dischard, I've killed the first fan header.
After this, the header only reports fan as running as "Fan 1 Speed at 164 RPM" :(

I'm not totally sure of the reason of this failure, but it seems also that existing fan headers will not be able to give sufficient power for all 5 fans.


### The full scheme Idea

The final idea was to completly replace the existing fan logic mangement inside the Ruckus OS.
With the mod, we will be able:
- To foul the existing header, let the believe that there is some fan attached to them even if not the case.
- To manage two group of fan, one for the existing rear fan, and one for the new two top fan, enable two manage RPM speed from the ESP32.
- To verify the fan speed of the rear fan and display it using the ESP32.
- And finally two have temperature and humidity measurement using the DTH22 sensors.

All of this features will be exposed to a Web site management interface using the ESP32 functionnality.
There will be a captive portal to enable setup of the ESP32 wifi, as weel to do some on-air update.


### Basic full level schema
```
|----------|
| ICX 7250 |
| PSU      |
|----------|
     |
     |-------------------------------------------------------|
    12V Rail                                                 |
     |                                                       |
     |                       |-------|                       |
     |                       |       |                       |
     |                       |       |                       |
     |                       |       |--------------- Mosfet Driver ------- Fan Group 1 (Rear)
|----------|                 |       |
| Voltage  |                 | Esp32 |--------------- Mosfet Driver ------- Fan Group 2 (Rear) ---|----- Fan2-1
| Regulator|---- 3.3v--------|       |                                                            |----- Fan2-2
|----------|                 |       |--------------- Fan Header 1                                |----- Fan2-3
                             |       |--------------- Fan Header 2                                          |
|-----------|                |       |--------------- Fan Header 3                                          |
|           |                |       |                                                                      |
|  DHT-22   |----------------|       |---------------- Tach Input 1 --------|                               |
|           |                |       |---------------- Tach Input 2 --------|-------------------------------|
|-----------|                |       |---------------- Tach Input 3 --------|
                             |-------|
```


- ICX 7250 PSU will be used to get raw 12V voltage.
- A voltage regulator will be used to downscaler 12V voltage to 3.3V usable for esp32.
- 2 Mofset driver will be feed from GPIO output to deliver PWM signal to the mofset, and so regulated voltage to the fan group, enable us to modulate the RPM of fan group.
- 3 GPIO input will be used to deliver PWM signal to each 3 existing fan header on the ICX7250 to fool the fan RPM detection.
- 2 GPIO input will be used to read the yellow tachometer wire from the existing fan to verify the RPM of the fans.



![WhatsApp Image 2025-05-28 à 16 57 00_7e351f3e](https://github.com/user-attachments/assets/d8b38ce2-bf4b-4832-a857-f2f39661493d)


![WhatsApp Image 2025-05-28 à 16 57 00_75b99067](https://github.com/user-attachments/assets/733c7685-aea4-405f-a188-be8b617f8ea6)

![WhatsApp Image 2025-05-28 à 16 57 00_0fbf6045](https://github.com/user-attachments/assets/210c701c-df29-4e82-8062-79e6b9c639ea)

![WhatsApp Image 2025-05-28 à 16 57 00_20b7042a](https://github.com/user-attachments/assets/8c356363-1b74-44de-b68a-ed6ea75be225)

![WhatsApp Image 2025-05-28 à 16 57 00_b389e760](https://github.com/user-attachments/assets/2fa067c5-f248-4fa3-a5a1-96d8d2c9dd1b)

![WhatsApp Image 2025-05-28 à 16 57 00_c294588a](https://github.com/user-attachments/assets/62d2f6eb-631c-4a74-80ac-dfac4d052c12)




