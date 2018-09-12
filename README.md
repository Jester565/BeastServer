# :japanese_ogre:BeastServer
The boost asio and beast examples makes using their libraries seem excessively difficult. BeastServer is a good example of utilizing
the two libraries and provides extra features including simple request routing and client management.

## Installation
Requirements: [CMake](https://cmake.org/), [Boost](https://www.boost.org/) Threading & Regex
```
mkdir build
cd build
cmake ..
```
On windows, build using the sln file.  On unix, run make.

### Troubleshooting
If cmake can't find Boost you can specify your boost location in the cmake command like so:
```
cmake -DBoost_INCLUDE_DIRS="/my/boost_1_66_0" -DBoost_LIBRARY_DIRS="/my/boost_1_66_0/stage/lib" ../
```

On windows, this project expects static boost libraries with MT linking.
