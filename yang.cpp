/******************************************************************************
-   yang.cpp
-   Prog Tools for Scits Engrs - CIVL 8903 Homework IV
-
-   Homework Discription:
-   Demonstrate that you have secured access to the computer resources necessa-
-   ry to be successful in hte course. Also, perfect your use of function.
-
-   Author: Yang Yang
-
-   Github Repository: https://github.com/SwiftHickory/homework4.git
******************************************************************************/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <climits>

using namespace std;

const string errorFileName = "yang.err";
ofstream errorFile;

typedef unsigned long long int ullong;

// function to open input file
void openInput(ifstream &inputFile, string fileName);

// function to open output file
void openOutput(ofstream &outputFile, string fileName);

// function to print message to errorFile using function printOutput
void errorMessage(const string &message);

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message);

// function to produce first num number of fibonacci senquence using int
bool fibonacciSequenceInt(int num, ullong *seq);

// function to produce first num number of fibonacci senquence using double
void fibonacciSequenceDouble(int num, double *seq);

// print out the fibonacci sequence of int
void printFibonacciInt(int num, ullong *seq, ofstream &outputFile);

// print out the fibonacci sequence of double
void printFibonacciDouble(int num, double *seq, ofstream &outputFile);

// determine number of digits in a positive integer
int numOfDigits(ullong num);

// main function
int main() {

    ifstream inputFile;
    ofstream outputFile;
    const string inputFileName = "yang.in";
    const string outputFileName = "yang.out";
    ostringstream messageStream;  // used for output to both file and terminal
    int n;  // 10*n is total numbers to compute
    int m;  // total numbers to compute

    // produce a simple paragraphy messege
    messageStream << "I was able to compile this code using the HPC at the University of Memphis. ";
    messageStream << "When I compiled it there, it did not produce any warning message. ";
    messageStream << "The HPC uses a GNU C++ compiler that can be considered a good up-to-date standard. ";
    messageStream << "I also version-controlled this code using git, and used a remote repository hosted by github. ";
    messageStream << "If I can do this, so can you!!!" << endl << endl;

    // produce another message
    messageStream << "I am so cool, that I was also able to write a code that produces the first ";
    messageStream << "M numbers of the Fibonacci sequence. Here they are: " << endl;

    // print the message to both terminal and error file
    printOutput(outputFile, messageStream.str());

    openOutput(outputFile, outputFileName);
    openInput(inputFile, inputFileName);

    // get the number need to compute
    inputFile >> n;
    if (n < 1) {
        errorMessage("Input file is wrong. Must be a positive number!\n");
        exit(EXIT_FAILURE);
    }
    m = 10*n;

    // compute fibonacci sequence
    //allocate memory for fibonacci sequecne
    ullong *seqInt;
    seqInt = new ullong [m];

    // if the maximum fibonacci number does not exceed the limit of ullong
    if (fibonacciSequenceInt(m, seqInt)) {
        printFibonacciInt(m, seqInt, outputFile);

        delete seqInt;
    } else {
        double *seqDouble;
        seqDouble = new double [m];

        fibonacciSequenceDouble(m, seqDouble);
        printFibonacciDouble(m, seqDouble, outputFile);

        delete seqDouble;

        // print out error message
        errorMessage("Warning: Number exceed the maximum integer. Use double instead!\n");
    }

    inputFile.close();
    outputFile.close();

    // if open error file, close it
    if (errorFile.is_open()) {
        errorFile.close();
    }

    return 0;

}

// function to produce first num number of fibonacci senquence using int
bool fibonacciSequenceInt(int num, unsigned long long *seq) {

    // initialize first two elements
    seq[0] = 0;
    seq[1] = 1;

    // compute the rest elements
    for (int i = 2; i < num; i++) {
        // if the number exceed the maximum of the integer, stop computing and return false
        if (ULLONG_MAX - seq[i - 1] < seq[i - 2]) {
            return false;
        } else {
            seq[i] = seq[i - 1] + seq[i - 2];
        }
    }

    return true;

}

// function to produce first num number of fibonacci senquence using double
void fibonacciSequenceDouble(int num, double *seq) {

    // initialize first two elements
    seq[0] = 0;
    seq[1] = 1;

    // compute the rest elements
    for (int i = 2; i < num; i++) {
        seq[i] = seq[i - 1] + seq[i - 2];
    }

}

// print out the fibonacci sequence of int
void printFibonacciInt(int num, ullong *seq, ofstream &outputFile) {

    int numberOfWidth = numOfDigits(seq[num - 1]) + 1;  // used for setw
    const int numbersPerLine = 10;
    ostringstream outputStream;  // used for output to both file and terminal

    for (int i = 0; i < num; i++) {

        outputStream << setw(numberOfWidth) << seq[i];

        // print \n every numbersPerLine numbers
        if ((i + 1) % numbersPerLine == 0 ) {
            outputStream << endl;
        }
    }

    printOutput(outputFile, outputStream.str());

}

// print out the fibonacci sequence of double
void printFibonacciDouble(int num, double *seq, ofstream &outputFile) {

    int numberOfWidth = 15;  // used for setw
    const int numbersPerLine = 10;
    ostringstream outputStream;  // used for output to both file and terminal

    for (int i = 0; i < num; i++) {

        outputStream << setw(numberOfWidth) << scientific << seq[i];

        // print \n every numbersPerLine numbers
        if ((i + 1) % numbersPerLine == 0 ) {
            outputStream << endl;
        }
    }

    printOutput(outputFile, outputStream.str());

}

// function to open input file
void openInput(ifstream &inputFile, string fileName) {

    inputFile.open(fileName.c_str());

    // perform sanity check it
    if (!inputFile.is_open()) {
        errorMessage("Cannot open input file: " + fileName + "\n");
        exit(EXIT_FAILURE);
    }

}

// function to open output file
void openOutput(ofstream &outputFile, string fileName) {

    outputFile.open(fileName.c_str());

    // perform sanity check it
    if (!outputFile.is_open()) {
        if (fileName == errorFileName) {
            // if we can open error file, just print out to terminal
            cout << "Cannot open error file: " << errorFileName << endl;
        } else {
            errorMessage("Cannot open output file: " + fileName + "\n");
        }

        exit(EXIT_FAILURE);
    }

}

// function to print message to errorFile using function printOutput
void errorMessage(const string &message) {

    // check whether the error file is opened
    if (!errorFile.is_open()) {
        openOutput(errorFile, errorFileName);
    }

    printOutput(errorFile, message);

}

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message) {

    cout << message;
    outputFile << message;

}

// determine number of digits in a positive integer
int numOfDigits(ullong num) {

    int digits = 0;

    while (num) {
        num /= 10;
        digits++;
    }

    return digits;

}