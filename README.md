![a kinetic sculpture](https://github.com/54aaron/Module-3/blob/main/img/IMG_8831.jpg?raw=true)
# Our Planet.

"Our Planet" is a kinetic sculpture.
Very simply, "Our Planet" consists of two rotating cardboard sculptures. One depicting an imagined planet, the other a cluster of pink stars.
The sculpture of the planet rotates indefinitely on its own axis and the cluster of stars "twinkle" by rotating back and forth.
This is all activated via a nifty little button that exists on the internet.

# How It Works

"Our Planet" isn't extremely complicated.
To offer a somewhat ground-up overview, this sculpture consists of an ESP32 microcontroller, servo motor, and stepper motor.
All of these devices are individually enclosed within a cardboard encasing.
This same encasing functions as a pedestal for the sculptures of stars and a planet - which the sculpture of the planet being attached to the stepper motor, and the stars attached to the servo motor. 

The sculpture may be "started" via a button present on the interface of a web server endpoint.
The ESP32, once connected to power and Wi-Fi, makes consistent API calls to this endpoint. By default, the endpoint returns false, but upon clicking the start button on its web interface, it returns true.

Once the ESP32 detects that "true" has been returned, then it facilitates its dual core processor to start spinning both the servo and stepper motors.

Below are more specific instructions regarding the code featured in the repository, and also more specific installation instructions.

# Code
## ESP32
### Libraries
To begin, the code in the ESP32 facilitates several libraries. 
- The ESP32Servo library to aid in controlling the servo motor.
- The Wi-Fi library to allow the device to connect to a local access point
- And the HTTP Client library to allow the device to make API calls via HTTP protocol

### Tasks
For the sake of being able to run both motors simultaneously as well as generally being able to perform multiple tasks at once, I took advantage of the ESP32's dual core processor. Each action is performed via its own respective task and, as such, I do not make use of the standard "loop()" function.

### Connecting to Wi-Fi
As alluded to earlier, the program leverages the Arduino WiFi library in order to connect to nearby Wi-Fi networks. Given an SSID and a password, the program facilitates the libraries begin function to connect to the network in the setup() function.

### API Calls
The process of making API calls is delegated to its own task which runs on core 0.
The code in this task constantly checks if the button on the web interface has been clicked and has returned "true." If it has returned true, the task sets a global variable "check" to 1. 
This "check" variable will be used to inform other tasks of whether or not true has been returned

### Motors
Each motor is assigned to its own task that runs on core 1. So, that means that both the stepper and servo motors are operated via their own respective tasks. Within these tasks, if it is ever determined that the global "check" variable has been set to 1, then the tasks will start each motor and they will begin turning.

## Hardware
  
### Flashing your ESP32 Device
After you're done with your code, you'll use the PlatformIO IDE's upload function to flash your device with the program via a USB-C cable between your computer and your ESP32's USB-C port. 

### Connecting Components 
All components were connected with the assistance of a breadboard and various jumper wires.
The stepper motor wires were connected to Ground, the 5v pin, as well as pins 25, 26, 27, and 33. The Servo motor wires were connected to Ground, the 5v pin, and pin 15. A potentiometer was also connected (ground, a 3.3v pin, and pin 12) but the potentiometer was never made functional.
  
![an ESP32 connected to two motors and potentiometer via a breadboard](https://github.com/54aaron/Module-3/blob/main/img/IMG_88071.jpg)

### Enclosure
The enclosure for this project was particularly important as it was intertwined with the overall visuals of the sculpture.

Similar to my last project, after the circuit had been built and all the components connected - it was then encased in a box of sorts made from cardboard and hot glued together. For this project, I went with a thinner cardboard that was easier to cut with a scissors. Moreover, the top of this enclosure was not hot glued together but was assembled using double sided tape. I wanted it to be removable in the case that I needed to troubleshoot the circuit (I did not need to).
The breadboard and motors had been secured to the bottom of this encasing using double sided tape. 

Moreover, large holes had been made on the top of this enclosure as to expose the motor shafts:
  
![An ESP32 circuit encased in a cardboard box](https://github.com/54aaron/Module-3/blob/main/img/IMG_8855.jpg)
  
After the overall circuit had been encased, I got to work on the planet and star sculptures.
This process was fairly straightforward and involved cutting out various cardboard shapes and hot gluing them together.

Once the sculptures had been assembled, they were attached to circular pieces of cardboard which would be attached to each motor. 

The planet sculpture was attached to the stepper motor via an incision made in the center of the circular pedestal.
The Star cluster sculpture had been attached to the servo motor via the use of a plastic propeller attachment and double-sided tape.

It might be worth considering the size of these sculptures as they would need to fit well with the motors.

I made the mistake of making the planet sculpture very large initially but it would always tip over its pedestal off the motor shaft.

![a very large cardboard planet sculpture](https://github.com/54aaron/Module-3/blob/main/img/IMG_8809.jpg)

And, as such, I needed to size down a bit:

![an tinier cardboard planet scultpure](https://github.com/54aaron/Module-3/blob/main/img/IMG_8810.jpg)

After everything had been assembled and hot glued together, I took the liberty of painting everything with acrylic paint.
Be sure to have enough paint and be aware that you may need multiple coats to fully cover the brown of the cardboard.

![an image of a eSP32 device running a program](https://github.com/54aaron/Module-3/blob/main/img/IMG_8828.jpg)
  
