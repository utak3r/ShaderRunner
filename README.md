# ShaderRunner
Helper tool for developing OpenGL fragment shaders, compatible with [ShaderToy's](https://www.shadertoy.com/) templates.

For the plans of its development, check [Projects tab](https://github.com/utak3r/ShaderRunner/projects).
Of course, you're welcome to join me in developing this little tool :)

## Building for Windows
This is what's needed:
* CMake 3.5 or 3.6
* Qt 5.x (confirmed working are 5.7.0, 5.7.1 and 5.8.0)
* Visual Studio 2015. You could also try with 2013, but don't try with any lower ones, due to usage of C++ lambdas.

If you want to enter some custom paths etc, go and create `set_user_paths.cmd` file. When you're ready, call `generate_VisualStudio.cmd` script. Then you can go with either `devel_VisualStudio.cmd` script for opening Visual Studio with the solution configured, or just `build_VisualStudio-release-clean.cmd` for building it from the console. The resulting bins will be put in `distrib.install` folder.

## Building for Linux
This is what's needed:
* CMake 3.5 or 3.6
* Qt 5.x (confirmed working are 5.7.0, 5.7.1 and 5.8.0)
* GCC. I've checked only version 6.2.1. You can check lower versions and report your results.

To build it, go like this:
```
./generate_linux
cd build-Linux
make
```
The outcome will be placed in `build-Linux/bin` directory.
