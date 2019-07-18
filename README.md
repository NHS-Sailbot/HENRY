# HENRY

HENRY is the public Github Repository created and maintained by the NHS Sailbot team. The [Documentation](https://github.com/NHS-Sailbot/HENRY/tree/master/docs) contains all the necessary information to understand the codebase's structure and functionality.

## Using the Sailbot library:

The Sailbot library uses a build system called [Premake](https://premake.github.io/), who's binaries are located in the build folder of this repository. To create a program with this, simply add your source files to the testing/src folder and include the library. [A further explaination of this](https://github.com/NHS-Sailbot/HENRY/tree/master/docs/Premake) can be found in the Documentation.

## To build the files:
- Linux:
    - open the terminal in the root directory of this repository
    - type `./build/build.sh`
- Windows:
    - open the `build` folder
    - double click the file `build.bat` to generate the Visual Studio solution

[more information on building from source](https://github.com/NHS-Sailbot/HENRY/tree/master/docs/BuildingFromSource.md)

## Example:

### [testing/src/main.cpp]
```cpp
#include <sailbot.hpp>

int main() {
    sailbot::comm::open_device(16);
    return 0;
}
```
### [.../test.ino]
```cpp
void setup() {
    Serial.begin(57600);
}

struct Data {
    char letters[4];
    unsigned int integer;
    float decimal;
};

constexpr static const unsigned int DATA_SIZE = sizeof(Data);

void loop() {
    unsigned char buff[DATA_SIZE];
    unsigned char index = 0;
    while (Serial.available() > 0) {
        const unsigned char byte_read = Serial.read();
        Serial.write(byte_read);
        buff[index] = byte_read;
        index++;
    }
    Data data = *reinterpret_cast<Data *>(buff);
    Serial.println(data.number);
}
```