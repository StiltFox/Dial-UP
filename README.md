# Stilt Fox&reg; Dial Up
### version 1.x.x
Stilt Fox&reg; Dial Up is a collection of libraries designed to make a networked program easier to write. These
libraries focus on the server side, with the idea that the program you're going to be writing will be serving
requests. HTTP version 1.1 is supported but may not be up to full ISO specification.

This program also allows for parsing raw socket data. This means that if you aren't writing something based on HTTP
then you can still just use the socket on its own.

## Legal Stuff
Stilt Fox&reg; is not liable for any damages done to your hardware. For more information see LICENSE file.\
Stilt Fox&reg; is trademarked. You may not use the Stilt Fox™ name, however this code is free to reference and use.\
You may contribute to this library, however all contributions will share the same license as this library, and you agree
that Stilt Fox&reg; owns the copyright for any contributions.

------------------------------------------------------------------------------------------------------------------------
## Important!
These instructions assume you are using a Linux based operating system to compile the code. No testing or compiling
has been performed on Windows by Stilt Fox&reg;. If you choose to use Windows as your main development platform you are
on your own. If you have a strong desire to use Stilt Fox&reg; libraries and products, switching to Linux is highly
recommended.

With The death of Windows 10 (as of the time of writing this) directly around the corner, the roadmap to support Windows
systems has been completely canceled.
------------------------------------------------------------------------------------------------------------------------
## Prerequisites
### Main Program
These libraries and programs are required to build and run this project.
- Cmake
  - version 3.5.0 or greater
- C++ compiler
  - must support C++ 20 standard or higher
  - we at Stilt Fox&reg; use g++
- Stilt Fox&reg; Stand Mixer
  - this can be found [here](https://github.com/StiltFox/StandMixer)
## Installation
### Linux
The following commands will install the libraries to your system. These will exist globally to all users. If you wish to
install for a single user, please adjust accordingly. Also, you will require sudo privileges to run the 
`cmake --install .` command.

```shell
mkdir build
cd build
cmake -DSFSkipTesting=true -DCMAKE_BUILD_TYPE=Release .. 
cmake --build .
cmake --install .
cd ..
```
### Mac
Mac installation works basically the same as linux, but you will need to include the toolchain file 
[mac.cmake](mac.cmake).

```shell
mkdir build
cd build
cmake -DSFSkipTesting=true -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../mac.cmake .. 
cmake --build .
cmake --install .
cd ..
```

### Installing without sudo privileges
up and to this point we've been assuming you have sudo privileges and are able to install libraries 'system-wide'. If
for some reason you dont have access to `/usr/local` we can instead install to the home directory.

In linux you should have a folder called `.local`. You can install the files here using the following command in place
of the normal installation command: `cmake --install . --prefix ${HOME}/.local`. 

On Mac, you would have a `Libraries` folder. Similar to Linux, you can use the following command instead of the normal
installation command: `cmake --install . --prefix ${HOME}/Libraries`.

Doing this will cause problems when CMake needs to find one of these packages. `find_package(DialUp REQUIRED)` will fail
without help. To fix this we need to tell cmake where to find the cmake configuration files. To do this we will add the
following line to our `~/.bashrc` file, or however you persist environment variables: `export `


## Linking to Stilt Fox&reg; Dial-Up
Linking to Stilt Fox&reg; Dial-Up is easy. In your CMakeLists.txt file include the following line: \
`find_package(Dial-Up REQUIRED)` \
Then to link your to your project use the following line: \
`target_link_libraries(MyProject StiltFox::DialUp::PortAuthority)` \
Please notice that each module that you want to link must be linked seperatly as shown above. There is no way to just
include all modules. This allows you to include only what you need in your build.