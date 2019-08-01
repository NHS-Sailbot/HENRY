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
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct RData {
    math::Vec2 gps, rc_left, rc_right;
    float wind_direction;
};

TData tdata = {0, 1, 1024, 1024};
RData rdata;

int main() {
    if (sailbot::system::init("\\\\.\\COM3", 57600))
        return 1;

    while (sailbot::system::update(&tdata, sizeof(TData), &rdata, sizeof(RData))) {
        std::cout << rdata.rc_left.x << ", " << rdata.rc_left.y << ", ";
        std::cout << rdata.rc_right.x << ", " << rdata.rc_right.y << "  |  ";
        std::cout << rdata.gps.y << ", " << rdata.gps.x << "  |  " << rdata.wind_direction << '\n';
    }
}

```

### [.../test.ino]
```cpp
struct Vec2 {
    float x, y;
};

struct RData {
    unsigned char m1_dir, m2_dir;
    unsigned int m1_pow, m2_pow;
};

struct TData {
    Vec2 gps, rc_left, rc_right;
    float wind_direction;
};

void setup() {
  Serial.begin(57600);
}

TData tdata = {
  {42.80212, -70.863186},
  {0.2, 0.4}, {0.6, 0.8},
  0.0
};
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