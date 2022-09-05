# H5_MQTT

## Getting started
Get source by calling ```git clone --recursive https://github.com/blackout1471/H5_MQTT.git```

## Generate solution & Compile

### Windows
Double click the file called GenerateSolution_win.bat

Open in visual studio and compile like any other projects

### Linux Arm
The required packages for compiling are:
```sudo apt install build-essential```
```sudo apt install uuid-dev```

Make GenerateSolution_linux.sh & Premake/premake5 to executables -hint:```sudo chmod +x --filepath```

Run the GenerateSolution_linux.sh -hint:```./GenerateSolution_linux.sh``` from root path

Then do ```make```

options can be added to make for whichever config is wanted -hint:```make help```

### Linux x86
Same steps as Linux arm, but to be able to generate solution, premake for x86 has to be downloaded and moved into the Premake/ folder
before doing any generate.
https://premake.github.io/download/

### Tests
The tests will be generated as a executable in the project UnitTest and IntegrationTest folder.

Tests will be generated as executables in the same folder as the project.

## Dependencies
* Premake
* GTest


