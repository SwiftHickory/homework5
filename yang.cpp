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

// function to open input file
void openInput(ifstream &inputFile, string fileName);

// function to open output file
void openOutput(ofstream &outputFile, string fileName);

// function to print message to errorFile using function printOutput
void errorMessage(const string &message);

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message);

// function to produce first 10*n number of Fibonacci sequence
void fibonacciSequence(int numberOfRaw, ofstream &outputFile);

// main function
int main() {

    ifstream inputFile;
    ofstream outputFile;
    const string inputFileName = "yang.in";
    const string outputFileName = "yang.out";
    ostringstream messageStream;  // used for output to both file and terminal
    int n;  // 10*n is totoal numbers to compute

    // produce a simple paragraphy messege
    messageStream << "I was able to compile this code using the HPC at the University of Memphis. ";
    messageStream << "When I compiled it there, it did not produce any warning message. ";
    messageStream << "The HPC uses a GNU C++ compiler that can be considered a good up-to-date standard. ";
    messageStream << "I also version-controlled this code using git, and used a remote repository hosted by github. ";
    messageStream << "If I can do this, so can you!!!" << endl << endl;

    // produce another message
    messageStream << "I am so cool, that I was also able to write a code that produces the first ";
    messageStream << "M numbers of the Fibonacci sequence. Here they are: " << endl;

    openOutput(errorFile, errorFileName);
    openOutput(outputFile, outputFileName);
    openInput(inputFile, inputFileName);

    // print the message to both terminal and error file
    printOutput(outputFile, messageStream.str());

    // get the number need to compute
    inputFile >> n;

    fibonacciSequence(n, outputFile);

    inputFile.close();
    outputFile.close();
    // if open error file, close it
    if (errorFile.is_open()) {
        errorFile.close();
    }

    return 0;

}

// function to produce first 10*n number of Fibonacci sequence
void fibonacciSequence(int numberOfRaw, ofstream &outputFile) {

    unsigned long long fn;  // F[n]
    unsigned long long fn_1 = 1;  // F[n-1]
    unsigned long long fn_2 = 0;  // F[n-2]
    ostringstream outputStream;  // used for output to both file and terminal
    int numberOfWidth = numberOfRaw > 10 ? 21 : 2*numberOfRaw + 2;  // used for setw

    outputStream << setw(numberOfWidth) << fn_2 << setw(numberOfWidth) << fn_1;

    // the remaining 8 number in the first row
    for (int j = 2; j < 10; j++) {
        fn = fn_1 + fn_2;
        fn_2 = fn_1;
        fn_1 = fn;

        outputStream << setw(numberOfWidth) << fn_1;
    }
    outputStream << endl;

    // the remaining number
    for (int i = 1; i < numberOfRaw; i++) {
        for (int j = 0; j < 10; j++) {
            // if the number exceed the maximum of the integer, stop computing and print error messege
            if (ULLONG_MAX - fn < fn_1) {
                outputStream << endl;
                errorMessage("Warning: Stop! Number exceed the maximum integer.\n");
                goto stop;
            }

            fn = fn_1 + fn_2;
            fn_2 = fn_1;
            fn_1 = fn;

            outputStream << setw(numberOfWidth) << fn_1;
        }

        outputStream << endl;
    }

    stop:
    // print the sequence
    printOutput(outputFile, outputStream.str());

    return;

}

// function to open input file
void openInput(ifstream &inputFile, string fileName) {

    inputFile.open(fileName.c_str());

    // perform sanity check it
    if (!inputFile.is_open()) {
        errorMessage("Cannot open input file: " + fileName + "\n");
    }

    return;
}

// function to open output file
void openOutput(ofstream &outputFile, string fileName) {

    outputFile.open(fileName.c_str());

    // perform sanity check it
    if (!outputFile.is_open()) {
        if (fileName == errorFileName) {
            // if we can open error file, just print out to terminal
            cout << "Cannot open output file. Error only printed to terminal!" << endl;
            cout << "Cannot open error file: " << errorFileName << endl;
        } else {
            errorMessage("Cannot open output file: " + fileName + "\n");
        }

        exit(EXIT_FAILURE);
    }

    return;
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

    return;

}
