<h1 align="center">METRONOME</h1>
<p align="center">
  Implementation of a metronome using the MSP432 board
  <br />
  <br /><br />
</p>




# About The Project
This application allows the user to set up a metronome in two different ways: via a digital interface or by making sounds.
The interface can be modified from the settings, allowing for a small range of personalization. Other settings allow for
different experiences with the application.




<br>




### Installation




 Clone the repo (or fork it)
   ```sh
   git git@github.com:EmanueleCarrara/Embedded-Software-Project.git
   ```






## Requirements






### Hardware Requirements


In this project we used the following  hardware:


* Texas Instruments MSP432P401R microcontroller
* Educational BoosterPack MKII:
    * Microphone
    * Buzzer
    * LCD Screen
    * Joystick
    * Buttons
    * Led






<br><br>


### Software requirements


To use this project, you will need the following software:


* MSP432-specific version of the [MSP Driver Library](https://www.ti.com/tool/MSPDRIVERLIB)
* [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE)
<br> <br>




## Project Layout
    Project
    ├── Debug                  # Compiled files
    ├── LcdDriver              # Driver libraries of T.I. for LCD
    ├── Functions              #
    │   ├── fundec.h           # File with functions and variables declarations
    │   ├── peripherals.c      # GPIO/ADC section
    │   ├── graphic.c          # LCD-related code
    │   └── timerfun.c         # File with timers' functions and variables
    ├── targetConfigs          # Target configuration files
    ├── main.c                 # Main source code
    └── README.md


<br>




## Getting started




1. Open the project in Code Composer Studio.
2. Go to: _Project → Properties → Build → Arm Compiler → Include Options_ and add the _source_ directory located in _simplelink_msp432p4_sdk_3_40_01_02/_
3. Go to: _Project → Properties → Build → Arm Linker → File Search Path_ and add:
    * _msp432p4xx_dreivelib.lib_ directory_ located in _[..]/source/ti/devices/msp432p4xx/driverlib/ccs/_
4. Build and run the project.




<br>




## User guide


The home page offers the user two option:
 * Metronome
 * Settings <br>

To navigate the menu the user can move the joystick either up or down to the desired option.
By either tilting the joystick right or using the upper button the user can confirm it's choice.
To return to the previous section the user can move the joystick left or use the lower button.
The "Metronome" section offers two more choice that will decide how the bpm for the metronome is set: <br>
* "Digital" : upon selection the screen will show the current value of the bpm. By moving the joystick up or down the user can change the value.
  * The metronome has a lower limit of 45 bpm and an upper limit of 210 bpm.
  * After confirming the choice the metronome will start.
* "Microphone" : the user will be asked to produce two sounds in sequence. These two sounds will be used by the application to compute the desired bpm based on the time between them.
  * After a blank loading screen a green screen will be shown by the metronome who will wait for the first of the two sound needed from the user
  * Following the first sound a yellow screen will be displayed and the user will be required to produce a second sound.
  * The interval of time between the sounds will be used to decide the desired bpm.
  * Once the sounds are registered the metronome will start playing
  * Should the user fail to produce a second sound within 60 seconds, the metronome will start with a bpm of 60 <br>
  
The "Settings" section offers many different options:
* "Led on/off" : either turn the led on or off
  * "ON"
  * "OFF"
* "Led color" : change the color of the led
  * "Red"
  * "Green"
  * "Blue"
* "Animation" : turn the animation on or off and choose if the animation should move or not
  * "OFF"
  * "Static"
  * "Moving"
* "Background" : change the background color
  * "Red"
  * "Black"
* "Text" : change the text color
  * "White"
  * "Black"
  * "Green"




## Video and PowerPoint


- Video [video](https://youtu.be/3l_956rOnLY)
- PowerPoint [slides](https://github.com/EmanueleCarrara/Embedded-Software-Project/blob/main/Project%20presentation.pptx)










# Team Members




The project was developed with equal effort from all the members and the code was always written when all the members were present or in a call




<br> <br>


<br>


## Acknowledgments


Students: <br>
* Emanuele Carrara - emanuele.carrara@studenti.unitn.it <br>
* Samuele Garzon - samuele.garzon@studenti.unitn.it <br>


Embedded Software for the Internet of Things Course - Professor: <a href="https://webapps.unitn.it/du/it/Persona/PER0212812/Didattica">Yildrim Kasim Sinan</a>


Project Link: [https://github.com/EmanueleCarrara/Embedded-Software-Project](https://github.com/EmanueleCarrara/Embedded-Software-Project)


## Copyright
MIT Licence or otherwise specified. See [license file](LICENSE) for details.


<p align="right">(<a href="#readme-top">back to top</a>)</p>
