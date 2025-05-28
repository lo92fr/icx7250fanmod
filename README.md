# icx7250fanmod
A guide to Ruckus ICX7250-48-p fan mode

## Warning

The modification will of course wold warranty of your switch.
An can ever kill the switch if something goes wrong in your modification.
I'm myself have some wrong things happens durring this mods.
So lets be warn that it can cost you to buy a new switch in the worst case.
And that said, of course, I will not assume any dommage in regards of modification you've made !

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
```
Brand : Foxconn
Model : PIA040H12P
Size  : 40 mm x 40 mm x 28 mm
RPM   : 18000 tr/min
Noise : 55.00 dB (A)
CFM   : 24.03 CFM
```
![WhatsApp Image 2025-05-28 à 16 57 00_db6bd5dc](https://github.com/user-attachments/assets/93780589-8178-4ea5-9a74-0bc924a8c2ed)


As you can see, high CFM, but with also very high RPM, and so heavy high frequency noise!

Mine was already have a first fan mode made by the first owner of the swith.
The 3 fan was replace by this model:

```
Brand : Mechatronics
Model : MR4020X12B1-RSR
RPM   : 12000 tr/min
Noise : 41,0dB(A)
CFM   : 17,3CFM (0,484m³/min)
```
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
```
Brand : Sunon 
Model : MF60151V3-1000U-A99
Size  : 60x60x15mm
RPM   : 3000 RPM
Noise : 21.3 dBA
CFM   : 17.6CFM
```

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

### The differents steps of the mode

#### Fan header modification

You first need to modify the fan header connector.
We will only keep the yellow wire as we will power the fan through the mofset driver.

First unplug the red, black and yellow connector from the plug.
For this, you can use a little flat screwdriver to push the connection from the hole on the connector.
You will need after this to unsolder the yellow wire from the metal connector, and solder a new one with gpio connector to plug into the esp32.
This is direct connection, we don't need any electronic circuit adapter for this connection.

![WhatsApp Image 2025-05-28 à 16 57 00_7e351f3e](https://github.com/user-attachments/assets/d8b38ce2-bf4b-4832-a857-f2f39661493d)

![WhatsApp Image 2025-05-28 à 16 57 00_75b99067](https://github.com/user-attachments/assets/733c7685-aea4-405f-a188-be8b617f8ea6)

![WhatsApp Image 2025-05-28 à 16 57 00_20b7042a](https://github.com/user-attachments/assets/8c356363-1b74-44de-b68a-ed6ea75be225)

#### Voltage regulator

Voltage regulator will give us 3.3v from 12v ICX7250 PSU rail.
I make the choice to not use available 3.3v from ICX7250 PSU.
I take the 12V from the connector comming from the PSU to the main board.

![WhatsApp Image 2025-05-28 à 16 57 00_f53cb117](https://github.com/user-attachments/assets/6ec341fd-0f2e-487f-abfa-3a2b05b70978)

![WhatsApp Image 2025-05-28 à 16 57 00_a4d894be](https://github.com/user-attachments/assets/0c23fd22-c34e-47b0-b924-bc32f068fbd1)

![WhatsApp Image 2025-05-28 à 16 57 00_6fa28c7c](https://github.com/user-attachments/assets/3600c672-9c97-4bcc-bbec-eb49a4fe92dd)

![WhatsApp Image 2025-05-28 à 16 57 00_a4100fe5](https://github.com/user-attachments/assets/d33882e9-a56a-4f9d-a7e6-3886f67b3293)

I've cutted the yellow and black wire on the first row connection, and derive additional wire.
The wire I used was comming from KNX wire, which is 0.80 mm2 section, rigid wire.
I would recommand to use rigid wire as it will be more simple to organized stable connection inside your ICX7250.

After this, you will need to soldier the cable comming from the PSU derivation to your voltage regulator.

![WhatsApp Image 2025-05-28 à 16 57 01_d6f3f89e](https://github.com/user-attachments/assets/f485b3fc-9cac-4d3d-9e7e-fb25fef6a10c)

On the bottom level, you can see 2 black and 2 yellow wire.
One are comming from the PSU derivation.
The other will be used later to power the Mofset driver.

On the top level, you have and red/black wire pair which is your 3.3v output.

You will need to set you voltage regulator to the good voltage.
Take your multimeter on the red/black output, and use a screw driver to turn the little resistor (the one just under the red wire) until you have the good voltage.

![WhatsApp Image 2025-05-28 à 16 57 01_6f222808](https://github.com/user-attachments/assets/7ac4f045-6b9b-468e-bfcf-f57dcb348210)

The voltage regulator can be put bellow the PSU wire, maitains using double-stick tape

![WhatsApp Image 2025-05-28 à 16 57 02_a1bbbdd4](https://github.com/user-attachments/assets/6e935c27-cd4c-4901-a79f-8e8c5f0d672d)


#### Mosfet connection

![WhatsApp Image 2025-05-28 à 16 57 01_ce346ff7](https://github.com/user-attachments/assets/e5944ebf-323e-465c-8d38-432eab986bfd)


You will have:

- On the right, wire comming from the voltage regulator (12V PSU rail).
- On the left, wire going to the fan. You will need to add a electrolytic capacitor to smooth the voltage on the output.
- On the downside, the command wire coming from the ESP32 pin output.

About capacitor choice : 
I have no good advice to give about it. 
You will have to try various capacity, possibly from 1uf to 470uf.
Upper value will better smooth the signal, lower capacity will give better response time.

Without capacitor, you will certainly observe behaviour like:
- Whistle comming from the fan at lower RPM frequency, because of resonance inside fan wire.
- Difficulty to operate at lower RPM speed.

### Fan header fool

ICX 7250 will not work if at least one fan is detected to work correctly.
The check is done as soon as the ICX 7250 finish to boot, and ICX will shutdown if it not detectd any fan.

Two prevent this, we can feed a PWN signal to the ICX 7250 fan header from the ESP32.
You will just need to attach a PWM GPIO output pin to the fan header yellow wire as describe before.
Code to generate the PWM signal will be describe later.

![WhatsApp Image 2025-05-28 à 16 57 01_409ada3b](https://github.com/user-attachments/assets/3e9bb69c-2537-4e9c-b8f2-85a5786c6b41)


### DHT22

DHT22 will need 3 wire connection.
You will need Red and Black wire comming from the 3.3v rail we get from the voltage regulator.
We will also need a Yellow wire that will be connected to a GPIO input pin on the Esp32.

Also notes that you will need a pull-up resistor between the red write (3.3v) and the Yellow wire (signal to gpio).
The resistor could have value between 4,7 kOhm to 10kOhm.

Also notes that wire of DHT22 can be different in regards to DHT22 package delivery. You can have 3 pin and 4 pin version.
Take a look to this page to help you : https://www.upesy.fr/blogs/tutorials/esp32-dht22-am2302-humidity-temperature-sensor-with-arduino-code?srsltid=AfmBOor08pP1xIK-uycE1Stwxz_ntsxQneosMDtc13h-1sgAEJhuZX2v


![WhatsApp Image 2025-05-28 à 16 57 02_a62f7cc5](https://github.com/user-attachments/assets/c52c0329-4127-4ca7-92d9-0bce3c40d5eb)


### 3.3V alimentation

You will need many derivation on the 3.3V rail :
- Wire coming from the voltage regulator.
- Wire to the ESP32 3.3V and GND pin to power the ESP32.
- Wire to the DHT 22 to power it.
- Wire to the capacitor/resitor electronic assembly we use for tachometer reading.

The best I've found is to solder the different wire together, and then use heat shrink tubing to isolate the wire from metal parts.
I've put all this connection in the little space behind the POE connector on the back of the unit.
    
![WhatsApp Image 2025-05-28 à 16 57 03_adeeae21](https://github.com/user-attachments/assets/6976a45e-b3a3-4b30-a8f9-75fbcbf51490)

### Tachometer yellow cable reading

This is probably the most difficult part.
The signal comming from the yellow fan wire enable us to read the current RPM speed.
The signal is generally a square signal, voltage and frequency may vary from fan brand to another.
We can read this signal from a GPIO pin.

First we need two make sure that voltage is ok for the ESP32. 
For memory, esp32 can accept 3.3V voltage on their pin.
Don't try to feed 12V voltage, or you will fry immedialty your Esp32.

If you voltage is not correct, you will have two use a voltage divider.
This generally consist of two resistor, one connect to your 3.3v input, the other connect to GND.
At the middle point, you would connect your fan yellow wire, and your GPIO pin.
Something like this:

```
3.3V =========== R1 =========== Yellow fan Pin ------------ R2 -------- GND
                                      |
                                      | 
                                    GPIO Pin
```


I won't give there any value for R1 and R2 as it will depend on you fan spec.
So please look on internet for starting value, and then check with a multimeter that voltage are correct.

On my side, voltage was already ok since the start.
So all I have to done is to add a pull up resistor to the 3.3V power because tachometer signal is open collector, so you will need this pullup to prevent false reading.
I've also add a little capacitor (around 4,7nf) ground to the gnd to smooth the signal.
This can help to read the signal at lower speed.

The main troubles is that PWN signal from the mofset driver will introduce noise into the tachometer signal.
So it can be very hard to find a good combinaison of capacitor / resistor to read the signal properly.
At first, I was only able to read the signal correctly between 10 000 RPM and 12 000 RPM.
After adding the capacitor, and also some software filtering, I achieve to have correct reading between 6 000 RPM and 12 000 RPM.

I was never able to read signal bellow 5500 RPM correctly, infact I was able to downspeed the Mechatronic fans as low as around 4200 RPM.

### Other PWM and Tachometer considerations

I've used the Mechatronic fan that come from the previous ownwer.
This is good fan but only with 3 wire connection: +12V,GND, and tachometer signal.
A better solution would be to have a fan with 4 wire connection : +12V,GND, tachometer signal, and PWN signal.

With such a fan:
- You can remove the mofset driver, and drive the fan directly from the PWM signal.
- Possibly, the tachometer signal will be far less noisy that we one we have with mofset driver.

But I've don't test this solution as I've don't have this 4 wire fan.
And don't even verify that Mechatronic have such reference in stock.

### Fan Performance comparison

You will find bellow some performance comparaison that I've made in regards to the different fan brand.
- RPM was verified using : @tobecheck
- CMF was measured using an RUZIZAO Anemometer : https://www.amazon.fr/dp/B0BGKBJ3VT?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1
  CMF is not very accurate, but it can give a good idea of what we can have.&
- Db is take from iPhone xxx DB Meter application : Db1 around 5cm, and Db2 around 30cm.

#### 12000 RPM / 17,3 CFM / Mechatronic

RPM       | Voltage | Db   | Db (2)  | Fréq     | Centre | Mi  | Bord | Centre C | Mi C | Bord C | M/S (1) | M/S (2) | Débit (1) | Débit (2) | Déb(1) m3/s | Déb(2) m3/s | CFM (1) | CFM (2) | CFM Moy
----------|---------|-------|--------|----------|--------|-----|------|----------|------|--------|---------|---------|-----------|-----------|-------------|-------------|---------|---------|---------
2915 RPM  | 3,25 V  | 46 Db | 40 Db  | 800 Hz   | 2      | 1.2 | 1    | 0.5      | 0.6  | 0.25   | 1.2     | 1.35    | 0.00192   | 0.00216   | 0.1152      | 0.1296      | 4.0682  | 4.5768  |
11600 RPM | 11,8 V  | 68 Db | 60 Db  | 1900 Hz  | 6.8    | 5.3 | 3.3  | 1.7      | 2.65 | 0.825  | 4.08    | 5.175   | 0.006528  | 0.00828   | 0.39168     | 0.4968      | 13.8320 | 17.5443 | 15.6882
11820 RPM | 12,08 V |       |        |          | 7.4    | 5.2 | 2.6  | 1.85     | 2.6  | 0.65   | 4.44    | 5.1     | 0.007104  | 0.00816   | 0.42624     | 0.4896      | 15.0525 | 17.2901 | 16.1713
5020 RPM  | 4,87 V  | 47 Db | 45 Db  | 800 Hz   | 3      | 2.5 | 1.5  | 0.75     | 1.25 | 0.375  | 1.8     | 2.375   | 0.00288   | 0.0038    | 0.1728      | 0.228       | 6.1024  | 8.0517  | 7.0771
7000 RPM  | 6,65 V  | 54 Db | 50 Db  | 600 Hz   | 4.6    | 3.2 | 1.8  | 1.15     | 1.6  | 0.45   | 2.76    | 3.2     | 0.004416  | 0.00512   | 0.26496     | 0.3072      | 9.3570  | 10.8487 | 10.1028
8000 RPM  | 7,66 V  | 55 Db | 52 Db  | 2100 Hz  | 5.2    | 3.2 | 2.2  | 1.3      | 1.6  | 0.55   | 3.12    | 3.45    | 0.004992  | 0.00552   | 0.29952     | 0.3312      | 10.5774 | 11.6962 | 11.1368
9000 RPM  | 8,69 V  | 60 Db | 55 Db  | 2400 Hz  | 5.8    | 4   | 2.4  | 1.45     | 2    | 0.6    | 3.48    | 4.05    | 0.005568  | 0.00648   | 0.33408     | 0.3888      | 11.7979 | 13.7303 | 12.7641
10000 RPM | 9,84 V  | 65 Db | 58 Db  | 800 Hz   | 6.2    | 4.2 | 3    | 1.55     | 2.1  | 0.75   | 3.72    | 4.4     | 0.005952  | 0.00704   | 0.35712     | 0.4224      | 12.6116 | 14.9169 | 13.7642

#### 18000 RPM / 24 CFM / 55 Db / Foxconn

RPM | Voltage | Db | Db (2) | Fréq | Centre | Mi | Bord | Centre C | Mi C | Bord C | M/S (1) | M/S (2) | Débit (1) | Débit (2) | Déb(1) m3/s | Déb(2) m3/s | CFM (1) | CFM (2) | CFM Moy
----|---------|----|--------|------|--------|----|------|----------|------|--------|---------|---------|-----------|-----------|-------------|-------------|---------|---------|---------
16500 RPM | 12,08 V | 78 Db | 73 Db | 1400 Hz | 12.4 | 8 | 5 | 3.1 | 4 | 1.25 | 7.44 | 8.35 | 0.011904 | 0.01336 | 0.71424 | 0.8016 | 25.2231 | 28.3082 | 26.7657
16100 RPM | 11,45 V | 80 Db | 74 Db | 1400 Hz | 12.4 | 8.6 | 4.5 | 3.1 | 4.3 | 1.125 | 7.44 | 8.525 | 0.011904 | 0.01364 | 0.71424 | 0.8184 | 25.2231 | 28.9015 | 27.0623
3000 RPM | 3,311 V | 43 Db | 40 Db | 6000 Hz | 2.3 | 1.7 | 1 | 0.575 | 0.85 | 0.25 | 1.38 | 1.675 | 0.002208 | 0.00268 | 0.13248 | 0.1608 | 4.6785 | 5.6786 | 5.1785
5000 RPM | 3,57 V | 48 Db | 45 Db | | 4 | 3 | 1.5 | 1 | 1.5 | 0.375 | 2.4 | 2.875 | 0.00384 | 0.0046 | 0.2304 | 0.276 | 8.1365 | 9.7468 | 8.9417
7000 RPM | 4,53 V | 56 Db | 50 Db | 600 Hz | 5.6 | 4.4 | 2.4 | 1.4 | 2.2 | 0.6 | 3.36 | 4.2 | 0.005376 | 0.00672 | 0.32256 | 0.4032 | 11.3911 | 14.2389 | 12.8150
8000 RPM | 5,08 V | 58 Db | 54 Db | 700 Hz | 6.7 | 4.5 | 2.8 | 1.675 | 2.25 | 0.7 | 4.02 | 4.625 | 0.006432 | 0.0074 | 0.38592 | 0.444 | 13.6286 | 15.6797 | 14.6542
9000 RPM | 5,74 V | 60 Db | 56 Db | 750 Hz | 7.5 | 5.6 | 3.5 | 1.875 | 2.8 | 0.875 | 4.5 | 5.55 | 0.0072 | 0.00888 | 0.432 | 0.5328 | 15.2559 | 18.8157 | 17.0358
12000 RPM | 8 V | 70 Db | 64 Db | 1000 Hz | 9.6 | 7 | 4.5 | 2.4 | 3.5 | 1.125 | 5.76 | 7.025 | 0.009216 | 0.01124 | 0.55296 | 0.6744 | 19.5276 | 23.8162 | 21.6719
14000 RPM | 9,61 V | 74 Db | 70 Db | 1200 Hz | 11.4 | 7.4 | 5.2 | 2.85 | 3.7 | 1.3 | 6.84 | 7.85 | 0.010944 | 0.01256 | 0.65664 | 0.7536 | 23.1890 | 26.6131 | 24.9011

#### 3000 RPM / 17,6 CFM / Sunon

RPM | Voltage | Db | Db (2) | Fréq | Centre | Mi | Bord | Centre C | Mi C | Bord C | M/S (1) | M/S (2) | Débit (1) | Débit (2) | Déb(1) m3/s | Déb(2) m3/s | CFM (1) | CFM (2) | CFM Moy
----|---------|----|--------|------|--------|----|------|----------|------|--------|---------|---------|-----------|-----------|-------------|-------------|---------|---------|---------
3050 RPM | 12,08 V | 45 Db | 40 Db | | 3.5 | 2.4 | 1.6 | 0.875 | 1.2 | 0.4 | 2.1 | 2.475 | 0.00756 | 0.00891 | 0.4536 | 0.5346 | 16.0187 | 18.8792 | 17.4490
3020 RPM | 11,97 V | 45 Db | 40 Db | | 3.2 | 2.8 | 1.5 | 0.8 | 1.4 | 0.375 | 1.92 | 2.575 | 0.006912 | 0.00927 | 0.41472 | 0.5562 | 14.6457 | 19.6420 | 17.1439
2050 RPM | 7,61 V | 40 Db | 38 Db | | 2.5 | 1.3 | 0.9 | 0.625 | 0.65 | 0.225 | 1.5 | 1.5 | 0.0054 | 0.0054 | 0.324 | 0.324 | 11.4420 | 11.4420 | 11.4420



![image](https://github.com/user-attachments/assets/b1f5eb81-83f2-4d40-903c-95afdaef0d4f)

### Final shoot of the mode

#### Global view

![WhatsApp Image 2025-05-28 à 16 57 02_f199f262](https://github.com/user-attachments/assets/c18548a5-26a0-4ffd-96cb-b4628de7ce06)

#### Zoom on the Mofset driver, Esp32, DHT22 and voltage regulator

![WhatsApp Image 2025-05-28 à 16 57 02_dd424d2d](https://github.com/user-attachments/assets/7e40c40d-6b64-4a29-a495-b2d3c38af47c)

#### The mofset and Esp32 in final position

![WhatsApp Image 2025-05-28 à 16 57 02_39650c9e](https://github.com/user-attachments/assets/62b46c32-c8f8-40ea-9790-5bdc0cb55a61)

#### The DHT22 and voltage regulator

![WhatsApp Image 2025-05-28 à 16 57 02_53a92070](https://github.com/user-attachments/assets/2ace85c0-aba7-4bd5-a1fe-7275d5b7d53e)




### The Web interface

For now, the interface is for advanced user, enable to modify frequency and duty cycle.
It was more easy to do the test.
I will certainly rewrite it to only have 3 or 4 button by fan group with predefined RPM like :

- For back fan : 5000 RPM, 8000 RPM, 10000 RPM & 12000 RPM.
- For top level fan : 2000 RPM, 3000 RPM.

![image](https://github.com/user-attachments/assets/50f83212-85e7-439b-8cd9-52ce9d8e9c47)


The interface have a setup page provider by ESP FS Web Server, where you can setup your Wifi connection.
![image](https://github.com/user-attachments/assets/a828ffe3-26e6-4401-ab90-a1447d3918f7)

There is also a firmware update page to update the firmeware on-air.

![image](https://github.com/user-attachments/assets/013115aa-718a-4c74-b27a-dbaca2826dfd)

And finally an edit page that enables to modify file system on the esp32:

![image](https://github.com/user-attachments/assets/a4671078-cc60-4ccf-be84-d25edf2e20c2)





![WhatsApp Image 2025-05-28 à 16 57 00_0fbf6045](https://github.com/user-attachments/assets/210c701c-df29-4e82-8062-79e6b9c639ea)

![WhatsApp Image 2025-05-28 à 16 57 00_b389e760](https://github.com/user-attachments/assets/2fa067c5-f248-4fa3-a5a1-96d8d2c9dd1b)

![WhatsApp Image 2025-05-28 à 16 57 00_c294588a](https://github.com/user-attachments/assets/62d2f6eb-631c-4a74-80ac-dfac4d052c12)
