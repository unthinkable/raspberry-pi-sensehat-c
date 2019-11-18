# Raspberry Pi Sense HAT C Library v0.1.0 {#mainpage}
#### [Gary Woodcock](mailto:gary.woodcock@unthinkable.com) – Unthinkable Research LLC

## Introduction

This library provides a C programming interface to the [Raspberry Pi Sense HAT](https://www.raspberrypi.org/products/sense-hat/) expansion board.

The genesis of this library came from my desire to use the Sense HAT in an existing C project on a Raspberry Pi 3. I was also quite interested in learning more about the Python/C API, and working through the design and implementation of a C library that wrapped the Sense HAT Python API seemed a reasonable way to become familiar with that.

This library calls through to the [Sense HAT Python library](https://pythonhosted.org/sense-hat/) via the [Python/C API](https://docs.python.org/3/c-api/index.html). I'll point out that a decent understanding of Python reference counting is very helpful in working with the Python/C API.

In the event that a new version of the Sense HAT is released that doesn't use the same hardware components as the current version, this library should continue to function properly with no code changes as long as the definition and behavior of the underlying Raspberry Pi Sense HAT Python library API doesn't change.

You may be asking, "Why not just write a native C library that directly interfaces with the various Sense HAT components over I2C or SPI?" That is an excellent question, especially since the Raspberry Pi Foundation has made the source code to the Python library openly available. It would be relatively straightforward to port this code to C. However, I really wanted to understand the general mechanism for interfacing C programs with Python libraries, and for now, I'm not particularly bothered by the performance impact of calling through the Python interpreter. 

## Requirements

### Hardware

This library is supported on Raspberry Pi hardware that supports the Sense HAT. Generally speaking, this includes Raspberry Pi hardware with a 40-pin GPIO connector - older Pi models with the 26-pin GPIO connector are not supported.

One note if you're planning on using a Raspberry Pi 4B... I've noticed that it's not the best idea to mount the Sense HAT directly on the Raspberry Pi 4B GPIO connector without accounting for the substantial amount of heat that the Raspberry Pi 4B CPU generates. For best results, I recommend using a ribbon cable to connect the Sense HAT (or any HAT, really) to the Raspberry Pi 4B. You may be able to get away with a direct mount if you've got active cooling (a fan) on the Raspberry Pi 4B, or possibly passive cooling with a heat sink - I haven't personally tried this yet. If you don't account for the heat, then your Sense HAT will also heat up, and may begin to behave unpredictably (or worse). 

### Software

The library has been extensively tested with Raspbian Stretch on a Raspberry Pi 3B Plus and Buster on a Raspberry Pi 4B - I make no claims about its suitability on Raspbian Wheezy or Jessie. While there are other operating systems that support the Raspberry Pi platform (Ubuntu or Windows IoT Core, for example), at this writing, this library is not explicitly supported in environments other than Raspbian.

Building the library requires `make`, `gcc` with C99 support and Python (version 2 or 3) - these are usually already present in Raspbian ISO distributions. I recommended that you also install `cppcheck`, `scan-build` (part of `clang` or `clang-tools`, depending on your Raspbian version), and `CUnit` - this will let you run code quality checks and the included unit tests. You'll also need to install `doxygen` if you want to generate the HTML documentation.

To run the example code, you'll need to connect a Raspberry Pi Sense HAT to your Raspberry Pi, and to verify that the Raspberry Pi Sense HAT Python library is installed and functioning properly (refer to [Sense HAT Install](https://pythonhosted.org/sense-hat/#install) and [Sense HAT API Reference](https://pythonhosted.org/sense-hat/api/) for detailed instructions).

## Building the Binaries

Building the binaries from source requires installing a development environment on a Raspberry Pi. The code can be built using the provided `bash`-based build system (recommended), or using the traditional `make` command with a `makefile`. 

### Building with the bash-Based Build System

I'll be honest, `cmake` makes my head hurt. Despite multiple attempts, I've never mastered its art and intricacies, and that's been frustrating because it seems to offer so much utility. As a consequence, I've developed a `bash`-based build system that I repurpose for use in a number of projects, and while it's not a standardized mechanism, it's made my life easier, and hopefully it won't prove too difficult for others to use.

The build system automates the process of building the various versions of the library (static and shared libraries in debug or release configurations), running code quality checks, running unit tests, and building the example program. It also simplifies management of the various build configuration parameters (e.g., `Debug` vs. `Release`).

Begin by opening a terminal window in the `raspberry-pi-sensehat-c/build directory` (it's _important_ that your current working directory is `raspberry-pi-sensehat-c/build`) and typing:

    ./build.sh --help

This will display the various options of the build system, as shown below:

    USAGE:  build.sh <args>

            All arguments are optional. With no arguments, the default behavior is:

            • Incremental debug build of libraries and programs
            • No unit testing
            • Minimal logging to console
            • No documentation build

            Possible argument values are:

            --check-env Checks the build support on the host environment.
            --clean     Forces a clean build instead of an incremental build.
            --debug     Builds debug version.
            --docs      Builds documentation using Doxygen.
            --help      Prints this usage notice.
            --release   Builds release version.
            --test      Runs unit tests after build.
            --verbose   Prints all log output to console.

            Prerequisites for running this build include:

            • clang or clang-tools (for scan-build, used with --debug builds)
            • cppcheck
            • CUnit (used with --test builds)
            • Python (version 2 or 3)

Next, try running this command:

    ./build.sh --check-env

This will give you feedback on whether your Raspbian system can fully support all the features of the build system, similar to that shown below:

    ******************************
    *** HOST ENVIRONMENT CHECK ***
    ******************************

    Sense HAT Python module: Installed 
                 Python 2.x: Installed (2.7.13)
                 Python 3.x: Installed (3.5.3)
                       make: Installed (v4.1)
                        gcc: Installed (v6.3.0)
                C99 support: Available
           clang scan-build: Installed
                   cppcheck: Installed (v1.76.1)
                      CUnit: Installed
                    Doxygen: Installed (v1.8.13)

`make` and `gcc` with C99 must be installed, and you'll need either Python 2.x or Python 3.x installed (if you have both installed, the build system will use whatever version `python` is aliased to). If you want to run code quality checks on the sources, you'll need `scan-build` and/or `cppcheck` installed. If you want to run unit tests, you'll need `CUnit` installed. If you want to generate HTML documentation from the source code, you'll need `doxygen` installed.

To build a clean debug version, run unit tests, and generate documentation, enter the following in the terminal:

    ./build.sh --debug --clean --test --docs

While building, progress information is displayed in the console, similar to what's shown below:

    *********************************
    *** SENSE HAT C LIBRARY BUILD ***
    *********************************

    Setting up...

    *************
    *** CLEAN ***
    *************

    Cleaning libsensehat.so...
    Cleaning libsensehat.a...
    Cleaning sensehat-example...
    Cleaning sensehat-test...
    Cleaning docs...

    ***************
    *** ANALYZE ***
    ***************

    Checking for clang scan-build...
    Checking libsensehat with clang scan-build...
    clang scan-build of libsensehat is good.

    Cleaning up after clang scan-build runs...
    Cleaning libsensehat...

    Checking for cppcheck...
    Checking libsensehat with cppcheck...
    cppcheck of libsensehat is good.

    *************
    *** BUILD ***
    *************

    Building libsensehat.so...
    Building libsensehat.a...
    Building sensehat-example...
    Building sensehat-test...

    *****************
    *** UNIT TEST ***
    *****************

    Sense HAT C library unit test has started.
    Sense HAT C library unit test has completed.
    Parsing libsensehat unit test results...
    1 CUnit test suite total.
    1 CUnit test suite run.
    3 CUnit test cases total.
    3 CUnit test cases run.
    3 CUnit test cases succeeded.
    669 CUnit assertions total.
    669 CUnit assertions run.
    669 CUnit assertions succeeded.

    *********************
    *** DOCUMENTATION ***
    *********************

    Building HTML documentation at /home/pi/raspberry-pi-sensehat-c/docs/html...
    Opening index.html in browser...

    *****************************************
    *** SENSE HAT C LIBRARY BUILD SUMMARY ***
    *****************************************

     Build started at: Fri Oct 18 11:47:31 CDT 2019.
    Build finished at: Fri Oct 18 11:49:27 CDT 2019.
         Elapsed time: 1 minute and 56 seconds.
        Configuration: Linux armhf
                       Debug
                       Clean build
                       With unit test
                       With documentation

After this command completes, you'll find the resulting binaries in the `raspberry-pi-sensehat-c/bin/linux/armhf/Debug` directory, and the generated HTML documentation in the `raspberry-pi-sensehat-c/docs/html` directory.

### Building with make

There are `makefiles` associated with the C library, the unit tests, and the example code. There are three source directories with `makefiles`:

    raspberry-pi-sensehat-c/src
    raspberry-pi-sensehat-c/test
    raspberry-pi-sensehat-c/example

To build a particular binary, `cd` to the appropriate source directory, and enter the following command in a terminal window:

    make

To clean a particular build, enter this command:

    make clean

Note that if you choose to build in this manner, you'll be using the default shell values for the various build parameters. These are set as follows:

    BUILD_CFG=Debug
    BUILD_CLEAN=0
    BUILD_DEBUG=1
    BUILD_DOCS=0
    BUILD_RELEASE=0
    BUILD_TEST=0
    BUILD_VERBOSE=0

With this configuration, you'll get an incremental debug build with no unit testing, and no documentation build. Of course, you can change these settings in your terminal session as needed.

## Running the Example Program 

The example program is intended to illustrate how to use the various functions exposed by the Raspberry Pi Sense HAT C library interface. Note that it's not exhaustive in terms of exercising every single option, but does provide a reasonable range of coverage.

I've ported most of the Python examples to C, including [`colour_cycle.py`](https://github.com/astro-pi/python-sense-hat/blob/master/examples/colour_cycle.py), [`compass.py`](https://github.com/astro-pi/python-sense-hat/blob/master/examples/compass.py), [`rainbow.py`](https://github.com/astro-pi/python-sense-hat/blob/master/examples/rainbow.py), [`rotation.py`](https://github.com/astro-pi/python-sense-hat/blob/master/examples/rotation.py), and [`text_scroll.py`](https://github.com/astro-pi/python-sense-hat/blob/master/examples/text_scroll.py). I didn't port the `joystick` examples, since they depend on the [`pygame`](https://www.pygame.org/) module.

Once you've built the example program, you can run it by opening a terminal in the `raspberry-pi-sensehat-c/bin/linux/armhf/_config_` directory, where `_config_` is either `Debug` or `Release`, depending on what you built. Next, enter the following command:

    ./sensehat-example

This will display an interactive console to exercise the various functions in the example program, as shown below:

    ************************************************
    *** Raspberry Pi Sense HAT C Library Example ***
    ************************************************

    Choose one of the following commands:

     1 - Prints this help notice.
     2 - Get C library version.
     3 - Color cycle example.
     4 - Compass example.
     5 - Rainbow example.
     6 - Rotation example.
     7 - Text scroll example.
     8 - Flash LEDs.
     9 - Load image.
    10 - Rotate LEDs.
    11 - Show letter.
    12 - Show message.
    13 - Flip LEDs horizontally.
    14 - Flip LEDs vertically.
    15 - Get compass heading in degrees.
    16 - Get environmental conditions.
    17 - Wait for event.
    18 - Quit (default).

    Enter your choice:

Choosing an option from the above list will either execute the function, or prompt for additional input. I believe the options are fairly self-explanatory, so I won't bore you with going through each option one at a time.

You can also exercise the example program from the command line. Try entering the following command in the terminal:

    ./sensehat-example --help

This will display the various command line options as shown below:

    ************************************************
    *** Raspberry Pi Sense HAT C Library Example ***
    ************************************************

    Available command line arguments are:

    --color-cycle-example                       Color cycle example.
    --compass-example                           Compass example.
    --environment                               Get environmental conditions.
    --flash                                     Flash LEDs.
    --heading                                   Get compass heading in degrees.
    --help                                      Prints this usage notice.
    --load-image=<path>                         Load image in file <path>.
    --rainbow-example                           Rainbow example.
    --rotation-example                          Rotation example.
    --show-letter=<letter>                      Show letter.
    --show-message=<message>                    Show message.
    --temperature                               Get temperatures in degrees Celsius and Fahrenheit.
    --text-scroll-example                       Text scroll example.
    --version                                   Returns C library version.
    --wait-for-event=<up|down|left|right|push>  Wait for event.

The command line will only accept a single option at a time - you can't chain multiple command line options on the same command entry. Try this:

    ./sensehat-example --color-cycle-example

This will cycle the LED colors on the Sense HAT LED matrix.

## Documentation

The documentation for this library is generated via Doxygen comments in the source files. To manually generate the HTML documentation, you first need to install [Doxygen](http://www.doxygen.nl/) 1.8 or later. Next, open a terminal window in the `raspberry-pi-sensehat-c/docs` directory, and enter the following command:

    doxygen Doxyfile

This will create a `html` directory in the `docs` directory. Open the `index.html` file in the `html` directory in a web browser to access the documentation starting point (e.g., this file).

## Potential Future Work

* Add support for gamma and low-light data descriptors.
* Add support for joystick event callbacks.
* Better argument validation for `SenseHAT_LEDLoadImage`. At the moment, this function does not validate that a file contains an image, nor that an image is an 8-bit image, nor that an image is 8x8 pixels (exactly 64 pixels).
* Prevent joystick events from generating input on the console.
* Cross-compilation support.
* More post-mortem documentation on working with Python/C API.

## Open Source

* The code is licensed under the [3-Clause BSD License](http://opensource.org/licenses/BSD-3-Clause).
* The project source code is hosted on [GitHub](https://github.com/unthinkable/raspberry-pi-sensehat-c).
* Please use [GitHub issues](https://github.com/unthinkable/raspberry-pi-sensehat-c/issues) to submit bugs and report issues.

## Acknowledgements

* [The Raspberry Pi Foundation](https://www.raspberrypi.org/), for creating such a wonderful playground for tinkerers of all stripes.
* [Steve Smith](mailto:kenobi@soresutechnologies.com), for invaluable assistance in testing the project, and in reducing the number of out-and-out distortions or misrepresentations I may have inadvertently set forth in source code and/or documentation.

