# Today

A minimal To-Do list with adjustable deadlines.

### Philosophy

The intent of the deadlines is to help user (me) set realistic deadlines. The tasks is either completed or not in the provided time frame. If I've missed it, I will have to accept that and make another task to finish the job.

### Features
	* No frills console To-Do list
	* History of tasks
	* Customizable via source code

### Customize

The config.cpp file contains various options that one can tweak.

### Dependencies

The code depends on Boost.DateTime and rang.hpp(included in repo) libraries.

### How I use it

I've put it in my .bashrc file, so that each time open a terminal window I would be reminded of my tasks.

### Installation

#### From sources

To compile the sources and install the program, in the folder with cloned repository run
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/usr ..
$ make
# make install
```
If you'll omit the `CMAKE_INSTALL_PREFIX`, by default, the application will be installed in `/usr/local/bin`. You may also use `-GNinja` flag in order to build with `ninja` build system.

#### Packages

Additionally, an AUR package `today-todo` is available. To install the program on Arch-based Linux distributions, you may wish to use an aurhelper. An example with `yay`
```
$ yay -S today-todo
```
