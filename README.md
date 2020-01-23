# Henry

Henry is the public Github Repository created and maintained by the NHS Sailbot team. More information can be found on [the wiki](https://github.com/NHS-Sailbot/Henry/wiki).

## Building from source:

On a default, unmodified Ubuntu installation:

```
sudo apt install git build-essential cmake libjpeg-dev
git clone https://github.com/NHS-Sailbot/Henry --recurse-submodules ~/Dev/Henry
mkdir ~/Dev/Henry/build && cd $_
cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8
./Master/Master
```