# resolve-robotics-uri-cpp

Pure C++ library (that only depends on C++ standard library) to resolve a package:// (ROS-style) or model:// (Gazebo-style) URI to an absolute filename.

## Installation

The `resolve-robotics-uri-cpp` is composed by a single sellf-contained header, `ResolveRoboticsURICpp.h`. You can directly use the header in your project, or install the library using one method described in the following.

### Installation via FetchContent

~~~cmake
include(FetchContent)
FetchContent_Declare(
  ResolveRoboticsURICpp
  GIT_REPOSITORY https://github.com/ami-iit/resolve-robotics-uri-cpp
  GIT_TAG        v0.0.1 # Or change this to use another release or tag
)
FetchContent_MakeAvailable(ResolveRoboticsURICpp)

# Use the provided target
target_link_libraries(<...> PRIVATE ResolveRoboticsURICpp::ResolveRoboticsURICpp)
~~~

### Install from source and find the CMake package in your project

~~~bash
git clone https://github.com/ami-iit/resolve-robotics-uri-cpp
cd resolve-robotics-uri-cpp
cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=./build/install -DCMAKE_BUILD_TYPE=Release .
cmake --build build
cmake --install build
export CMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH:`pwd`/build/install
~~~

Then in your project:
~~~cmake
find_package(ResolveRoboticsURICpp REQUIRED)

# Use the provided target 
target_link_libraries(<...> PRIVATE ResolveRoboticsURICpp::ResolveRoboticsURICpp)
~~~

## Usage in C++

Add `#include <ResolveRoboticsURICpp.h>` to your C++ file, then take inspiration from the following examples.

If you want to get the location of the `iCubGazeboV2_7` iCub model installed from [`icub-models`](https://github.com/robotology/icub-models):

~~~cxx
std::optional<std::string> absolute_path = ResolveRoboticsURICpp::resolveRoboticsURI("package://iCub/robots/iCubGazeboV2_7/model.urdf")
~~~

If you want to get the location of the `ergoCubSN00`  model installed from [`ergocub-software`](https://github.com/icub-tech-iit/ergocub-software):

~~~cxx
std::optional<std::string> absolute_path = ResolveRoboticsURICpp::resolveRoboticsURI("package://ergoCub/robots/ergoCubSN000/model.urdf")
~~~

If you want to get the location of the `panda`  model installed by [`moveit_resources_panda_description`](https://index.ros.org/p/moveit_resources_panda_description/):

~~~cxx
std::optional<std::string> absolute_path = ResolveRoboticsURICpp::resolveRoboticsURI("package://moveit_resources_panda_description/urdf/panda.urdf")
~~~

## Command Line usage

`resolve-robotics-uri-cpp` also install a command line tool called `resolve-robotics-uri-cpp` for use in scripts, that can be used as:

~~~
resolve-robotics-uri-cpp package://iCub/robots/iCubGazeboV2_7/model.urdf
~~~

For example,  on bash this can be used to easily convert the a urdf specified via `package://` to an sdf (assuming you have Gazebo installed), using the [backtick operator](https://www.redhat.com/sysadmin/backtick-operator-vs-parens):
~~~
gz sdf -p `resolve-robotics-uri-cpp package://iCub/robots/iCubGazeboV2_7/model.urdf`
~~~

## Python version

A package implementing the same logic in pure Python is available at https://github.com/ami-iit/resolve-robotics-uri-py .

## Contributing

To simplify setting up the development environment of the project, this repo uses [pixi](https://pixi.sh).

In particular, to run the unit tests of the project you can just:

~~~bash
git clone https://github.com/ami-iit/resolve-robotics-uri-cpp
cd resolve-robotics-uri-cpp
pixi run test
~~~

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[BSD-3-Clause](https://spdx.org/licenses/BSD-3-Clause.html)
