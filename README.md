# SHA-512 - Theory of Algorithms Project 2021

## Introduction
Theory of Algorithms Project - 4th Year. The goal of this project is to create a C program without the use of external libraries to take in a file or the path to a file as input through the command line and output the SHA-512 digest of the contents of the file.

## Compilation
Clone this repository:

``` 
git clone https://github.com/ArnasSteponavicius00/theory-of-algorithms.git
```

Change into the project directory and create the executable using Make:

```
cd project
make sha512
```

Run the executable using:

```
./sha512 path/to/file
```

### **Extras**
#### Test
If the *coreutils* package is installed, you can run:
```
make test
```
to compare the output of this program to the sha512sum utility to make sure the program is creating the correct hash digest.
#### Clean
Using:
```
make clean
```
you can remove the sha512 executable that was created.

## Author:
* Arnas Steponavicius

## References:
* [Ian McLoughlin](https://github.com/ianmcloughlin)
* [Secure Hash Standard NIST](https://www.nist.gov/publications/secure-hash-standard)

