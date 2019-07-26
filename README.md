# HENRY
HENRY is the public Github Repository created and maintained by the NHS Sailbot team. The [Documentation](https://github.com/NHS-Sailbot/HENRY/tree/master/docs) contains all the necessary information to understand the codebase's structure and functionality.

## Using the Sailbot library:
The Sailbot library uses a build system called [Premake](https://premake.github.io/), who's binaries are located in the build folder of this repository. To create a program with this, simply add your source files to the testing/src folder and include the library. [A further explaination of this](https://github.com/NHS-Sailbot/HENRY/tree/master/docs/WritingAPremakeScript) can be found in the Documentation.

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
#include <iostream>
#include <sailbot.hpp>

struct TData {
    char bytes[9];
    float floats[3];
};

struct RData {
    unsigned short words[3];
    char bytes[8];
};

void print_data(const RData &d) {
    std::cout << d.bytes << ' ';
    for (unsigned int i = 0; i < 3; ++i)
        std::cout << d.words[i] << ' ';
    std::cout << '\n';
}

TData tdata = {"Computer", {0.4, 0.5, 0.6}};
RData rdata;

int main() {
    sailbot::system::init();

    while (sailbot::system::update()) {
        if (sailbot::system::should_tick()) {
            sailbot::system::handshake(&tdata, sizeof(TData), &rdata, sizeof(RData));
            print_data(rdata);
        }
    }
}
```
### [.../test.ino]
```cpp
struct RData {
    char bytes[9];
    float floats[3];
};

struct TData {
    unsigned short words[3];
    char bytes[8];
};

void setup() {
  Serial.begin(57600);
}

TData tdata = {{1, 2, 3}, "Arduino"};
RData rdata;

void loop() {
  while (Serial.available() > 0) {
    Serial.readBytes(reinterpret_cast<unsigned char *>(&rdata), sizeof(RData));
    for (unsigned int i = 0; i < sizeof(TData); ++i)
      Serial.write(*(reinterpret_cast<unsigned char *>(&tdata) + i));
  }
  Serial.flush();
}
```