# HENRY

HENRY is the public Github Repository created and maintained by the NHS Sailbot team. The [Documentation](https://github.com/NHS-Sailbot/HENRY/wiki/Home) contains all the necessary information to understand the codebase's structure and functionality.

## Using the Sailbot library:

The Sailbot library uses a build system called [Premake](https://premake.github.io/), who's binaries are located in the build folder of this repository. To create a program with this, simply add your source files to the testing/src folder and include the library. [A further explaination of this]() can be found in the Documentation.

To build the files:
- Linux: 
    - open the terminal in the root directory of this repository
    - type `./build/build.sh` [how this could go wrong]()
- Windows: 
    - open the `build` folder
    - double click the file `build.bat` to generate the Visual Studio solution [how this could go wrong]()

[more information on building from source](https://github.com/NHS-Sailbot/HENRY/tree/master/docs/BuildingFromSource.md)

## Example:

[testing/src/main.cpp]
```cpp
#include <sailbot.hpp>

int main() {
    sailbot::comm::open_device(16);
    return 0;
}
```
