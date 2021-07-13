This is a portable linear algebra library defined in the matrix.h file. Which takes matrices as input and
based on the function selected it can perform transpose or multiplication of matrices.

# Environment
I wrote and tested the code on Ubuntu 20.04 Desktop

# Pre-requisites
## Basic packages
```bash
sudo apt-get install build-essential gdb
```

## Cmake
```bash
sudo apt-get update && upgrade
sudo snap install cmake -classic
```
## Ninja (Optional)
```bash
sudo apt install ninja-build
```
# Compile
## Without Ninja
```bash
cmake .. && make
```
## With Ninja
```bash
cmake -G Ninja .. && ninja
```

# How to run the application
## Pseudo command
`./matmul <mode> <function> <show execution time or not> <no of threads> <row1> <col1> <matrix values 1> <row2> <col2> <matrix values 2>`

## Arguments
```
mode: Choose between two modes manual or test. manual mode lets you manually input matrices and select which function to
perform transpose or multiply.
function: Choose between transpose or multiply.
Execution time: Boolean to choose whether to display execution time or not. Type true or false to switch between on or off.
No of threads: If you want to perform multithreaading enter no of threads ortherwise for single thread option just enter 1.
row1: Number of rows in first matrix.
cols1: Number of cols in first matrix.
matrix values 1: The values of the first matrix which can be entered by separating each value with a comma.
row2: Number of rows in second matrix.
cols2: Number of cols in second matrix.
matrix values 2: The values of the second matrix which can be entered by separating each value with a comma.
```
## Manual mode command example
### Transpose
```bash
./matmul manual transpose true 1 2 2 1,2,2,1
```
### Multiplication
```bash
./matmul manual multiply false 2 3 3 1,2,3,4,5,6,7,8,9 3 1 4,5,6
```
## Test mode command example
This is an automatic mode where the code is ran on six test cases and after running, it lets you know how many cases have passed.

```bash
./matmul test true
``` 
or
```bash
./matmul test false
```
# Files
## main.cpp
It is the main file which when you run you get the option to choose manual or test mode. Choose which function to run in
manual mode. Either to use multithreading or single threaded operation. This file also has the test cases defined.
## matrix.h
It is the header file which has the entire library defined. This file can be used independently if required as a normal
library and with any other file and you can call the functions to perform transpose and multiplication functions.

# Additional Notes
I have written the code in the best modular way possible so that it is required to be compiled just once and you can
choose from various arguments.

To run custom test cases it will require the main.cpp to be modified.

The multithreaded option is only useful when the matrix size is very large, for small matrices it is not useful as there
is large overhead which makes using single threaded a better choice.

The code seems to be a bit long, if external libraries like gflags, lest test case framework, etc. would be used the
code would be much shorter.