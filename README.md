# bryllite-core
Bryllite Core Build Environment setup example

# Environment
* [Visual Studio 2015 Commuity Edition](https://www.visualstudio.com/ko/vs/older-downloads/)
* [CMake-3.11.1](https://cmake.org/download/)
* [Boost-1.66](https://sourceforge.net/projects/boost/files/boost-binaries/1.66.0/)
* [Cygwin-2.10.0](https://www.cygwin.com)
* [Git-2.17.0](https://git-scm.com/downloads)

# git clone
~~~
git clone https://github.com/ssueim-labs/bryllite-core.git
~~~

# build on win32
on project root directory
~~~
mkdir build.win32
cd build.win32
cmake ..
msbuild bryllite.sln
~~~
output binary on `bin/{Debug|Release}/${TargetFileName}.exe`

# build on cygwin(linux)
on project root directory
~~~
mkdir build
cd build
cmake ..
make
~~~
output binary on `bin/${OutputFileName}`

*Tested on Cygwin-2.10, Ubuntu 17.04-LTS*
