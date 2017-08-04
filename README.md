# Phanatos' Utils Library - PUtils 
[![Code Quality](https://codeclimate.com/github/Phanatos-/utils/badges/gpa.svg)](https://codeclimate.com/github/Phanatos-/utils)
[![Issue Count](https://codeclimate.com/github/Phanatos-/utils/badges/issue_count.svg)](https://codeclimate.com/github/Phanatos-/utils)
[![Build Status](https://travis-ci.org/Phanatos-/utils.svg?branch=master)](https://travis-ci.org/Phanatos-/utils)
[![Documentation](https://codedocs.xyz/Phanatos-/utils.svg)](https://codedocs.xyz/Phanatos-/utils/)
[![Code Coverage](https://codecov.io/gh/Phanatos-/utils/branch/master/graph/badge.svg)](https://codecov.io/gh/Phanatos-/utils)



__Copyleft (c) 2016 - 2017 Phanatos Utils Library by Patricio Bonsembiante (Phanatos)__

## Synopsis

This library is a collection of utilities  (common functions, data structure implementations, exception handling routines, etc.)

The main idea is to provide a useful and configurable library containing reusable and well tested code for common tasks that are usually required.

So far we have the following implementations:

* Linked List
* Dictionary
* Exceptions (simple and lightweight exception handling framework) 
* Queue (implemented using linked lists)
* Stack (implemented using linked lists)

On-going development:

* More on lists (doubly linked lists, circular lists) 
* Logger
* String handling
* Configuration file handling (Properties-like files)

Nice to have: 

* Trees -> (BST, Red/Black, Balanced trees, etc)
* Sets -> A collection of items which does not allow duplicates
* Graphs -> Graphs Data Structure handling and common algorithms

## Code Examples

The best place to learn how to use the library are the [tests](/test), which show the main features of each implementation and the considerations that should be taken into account.

To avoid namespace collisions with other libraries we use the prefix `p+(name)` where name is a name related to the main goal of the implementation, i.e:

```
/* PLIST */
...
plist *aList = plist_create();
plist *otherList
/* some code here */
plist_merge(aList, otherList);
...

/* PSTACK */
...
pstack *stack = pstack_create();
pstack_push(stack, someValue);
...
```

The only exception to this is the exception framework wich defines the `try`, `catch` and `throw` keywords.

```
/* PEXCEPT */
...
PEXCEPT_T e;
try {
    if(!someCondition()) {
        throw(MY_CUSTOM_EXCEPTION);
        //Anything after point is never executed
    }
} catch(e) {
    printf("Error code: %i", e);
}
...
```

## Motivation

I've started this as an utilities library for a previous project which got dropped after a while. I've found some of the code and decided to upload it to share it and keep improving it over time.

The library was previously using the Autotools Build System, I've learned (ok, still learning) a lot on how to use Autotools thanks to this little lib.

I've recently chosen to move to the [CMake](https://cmake.org) Build System since it can target various systems easily and I've found it simpler and easier to use and configure. Just like with Autotools, I'm still learning to use the Build System and any improvements are welcome.

Please, feel free to PR any improvements to it.

## Installation

You need to download and install [CMake](https://cmake.org). For instructions on how to install/configure CMake you can refer to [CMake's Documentation](https://cmake.org/cmake/help/latest/).

Once CMake is installed you have two choices:

   - Run it from command line
   - Use it from your favorite IDE

If running from command line you should use:

  - `git clone https://github.com/Phanatos-/utils.git`
  - `cd utils`
  - `mkdir build && cd build`
  - `cmake ..` (You can, and should, set compiler/linker flags to use in the build using -D. You can look at [Travis YML](./.travis.yml) for an example)
  - `make`

If using it from an IDE, the steps and required configuration might vary widely from one IDE to the other and you should check the IDE's documentation related to CMake based projects.

## Tests

I've adopted [Unity](https://github.com/ThrowTheSwitch/Unity) framework for unit testing the library, it is a very powerful and flexible, fully written in C, portable and extensible unit testing framework. You can refer to the project's documentation to fully understand the framework.

I'm currently writing the tests for the available code and will not write any new functionality until all the current code is tested.

You can run `ctest` moving to the test directory in your `build` directory:

  - `cd build && cd test`
  - `ctest`

## Contributors

* [Phanatos](https://github.com/Phanatos-) (Patricio Bonsembiante)
* [Dipiert](https://github.com/Dipiert)

## License
PUtils is released under [The GNU General Public License v3](./COPYING) so everybody can use and contribute freely.
