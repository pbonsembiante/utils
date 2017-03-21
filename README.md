# Phanatos' Utils Library - PUtils 
[![Utils Code Quality](https://codeclimate.com/github/Phanatos-/utils/badges/gpa.svg)](https://codeclimate.com/github/Phanatos-/utils)
[![Utils Issue Count](https://codeclimate.com/github/Phanatos-/utils/badges/issue_count.svg)](https://codeclimate.com/github/Phanatos-/utils)
[![Utils Build Status](https://travis-ci.org/Phanatos-/utils.svg?branch=master)](https://travis-ci.org/Phanatos-/utils)
[![Documentation](https://codedocs.xyz/Phanatos-/utils.svg)](https://codedocs.xyz/Phanatos-/utils/)

__Copyleft (c) 2016 - 2017 Phanatos Utils Library by Patricio Bonsembiante (Phanatos)__

## Synopsis


This library contains a few utilities and common functions, data structure implementations, exception handling routines (very simple one), etc.

So far we have: 

* Lists
* Dictionaries 
* Exceptions (simple and lightweight exception handling framework) 
* Queue 
* Stack

Soon to have:

* More on lists (doubly linked lists, circular lists) 
* Logger
* String handling
* Configuration files

Nice to have: 

* Trees -> (BST, Red/Black, Balanced trees, etc)
* Sets
* Graphs 

## Code Example

WIP

## Motivation

I've started this as an utilitary for another project which got dropped after a while. I've found some of the code and decided to upload it to share it and keep improving it over time. 

I've learned (ok, still learning) how to use Autotools thanks to this little lib. Since I had no previous experience with it, any contributions are welcome. 

Please, feel free to PR :)

## Installation


To build the library run the following (you will need to have installed libtool): 
    
    > git clone git@github.com:Phanatos-/utils.git
    > autoreconf -fi
    > ./configure
    > make 

You should set a few flags (at least `-std=c11`) to do that use either:
   
    > ./configure CFLAGS='-std=c11'

or 

    > make CFLAGS='-std=c11'

To run the tests run: 

    > make check

It will build and run all the tests for the library. 

If you want to install the library you must have the required permisions:

    > sudo make install

## API Reference

WIP

## Tests

I've adopted Unity framework for unit testing the library, it is a very powerfull and flexible, fully written in C, portable and extensible unit testing framework. 

I'm slowly writing the tests for every functionality and probably won't write new functions untl the tests are finished. 

## Contributors

* Phanatos (Patricio Bonsembiante)
* Dipiert 

## License

I'm releasing this code under [The GNU General Public License v3](./COPYING) so everybody can use and contributy freely.
