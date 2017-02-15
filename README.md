# Perlin
A utility for generating 2D Perlin noise

# Build Instructions
1. Install [CMake](https://cmake.org/) 3.1.0 or later
2. Install [Boost](http://www.boost.org/)
3. Install [OpenImageIO](http://openimageio.org/)
4. Clone a copy of the repository:
    ```
    git clone https://github.com/SimonBoorer/Perlin.git
    ```
5. Build with CMake:
    ```
    cd Perlin
    mkdir build
    cd build
    cmake ..
    ```

# Usage
```
Usage: Perlin [options] output-file
Allowed options:
  -h [ --help ]                   produce help message
  -x [ --xres ] arg (=255)        x resolution
  -y [ --yres ] arg (=255)        y resolution
  -s [ --sample-size ] arg (=256) sample size
  -r [ --seed ] arg (=0)          psuedo-random seed
  -p [ --persistence ] arg (=0.5) persistence value
  -o [ --octaves ] arg (=2)       number of octaves
```

