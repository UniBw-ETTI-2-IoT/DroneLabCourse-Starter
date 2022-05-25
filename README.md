# DroneLabCourse-Starter
PlatformIO Starterproject for the Drone Lab Course of the UniBw/ETTI2 Lecture "Internet of Things". The Instructions are provided in ILIAS.


## `include`
contains the header files 
- `main.h`
- `background.h`

## `lib`
contains the libraries
- `pubsubclient` from https://github.com/Suxsem/pubsubclient (modded by Suxsem for QoS support)
- `RMTT_Libs` from https://github.com/RoboMaster/RMTT_Libs (modded by me to allow sending commands without response)

## `PythonTello2_SIMULATOR`
- is only interesting if you for some reasons experience technical difficulties and can't test your code with two real drones
- contains the Python Script `SimulatedTello2.py` that can simulate the second flying tello by sending the expected MQTT-Messages at the right moment.
- requires the Python-Package `paho-mqtt` -> `pip install paho-mqtt`
- check / change the AircraftIDs to match yours!

## `src`
contians the source files
- `main.cpp` **(you will work only on this file)**
- `background.cpp`