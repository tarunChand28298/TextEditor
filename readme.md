# Web Server:
As the title suggests, this is a text editor similar to notepad but with tabs. Suitable for quickly whiping up a text editor and open multiple tabs to edit text and make quick notes

## Motivation
This is a hobby project I did as a part of learning to make cross platform user interfaces in C++ using WxWidgets.

## Screenshots:
![1](https://github.com/tarunChand28298/TextEditor/blob/master/screenshots/1.png?raw=true)
![2](https://github.com/tarunChand28298/TextEditor/blob/master/screenshots/2.png?raw=true)

## Technologies used:
* C++ (MSVC)
* WxWidgets
* C++ STL

## Features:
* Edit text
* Open existing text documents
* Save text documents
* Work on multiple text documents open on different tabs
* Rearrange the tabs into docable regions

# Installation:
0. WxWidgets is a dependency for this, make sure you have WxWidgets SDK on your computer
1. Clone this repository
2. open the .sln file in visual studio 2019
3. point the include directories to WxWidgets include directory
4. point the libraries directory for the linker to where the .lib for WxWidgets is located
5. Build the solution
6. the built binary is inside the `bin` folder of the project directory

