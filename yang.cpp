/******************************************************************************
-   yang.cpp
-   Prog Tools for Scits Engrs - CIVL 8903 Homework V
-
-   Homework Discription:
-   This assignment requires you to understand how to declare and manipulate a 
-   collections of objects using a arrays, enumerators, and structures. You
-   are expected to learn and to design and use functions in C++ using the ge-
-   neral style of the examples done in class, including declaration of param-
-   eters.
-
-   Author: Yang Yang
-
-   Github Repository: https://github.com/SwiftHickory/homework5.git
******************************************************************************/

#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <cstdlib>
#include <sstream>

using namespace std;

const string logFileName = "yang.err";
ofstream logFile;

// struct ot store valid entry
struct EntryType {
    networkCodeType networkCode;
    sting stationCode;
    typeOfBandType typeOfBand;
    typeOfInstrumentType typeOfInstrument;
    string orentation; // one to three characters, case insensitive
};

// case sensitive
enum networkCodeType {
    CE,
    CI,
    FA,
    NP,
    WR
}

// case insensitive
enum typeOfBandType {
    long-period,  // L
    short-period,  // B
    broadband  // H
}

// case insensitive
enum typeOfInstrumentType {
    H,  // High-Gain
    L,  // Low-Gain
    N  // Accelerometer
}

// function to open input file
void openInput(ifstream &inputFile, string fileName);

// function to open output file
void openOutput(ofstream &outputFile, string fileName);

// read header from input file
void readHeader(ifstream &inputFile, ofstream &outputFile, string outputFileName);

// check the validity of date and get month day and year
void checkDate(string date, string &month, string &day, string &year);

// check the validity of time and get hour, minute, second and millisecond
void checkTime(string time, string &hour, string &minute, string &second, string millisecond);

// check the validity of time zone
void checkTimeZone(string timeZone);

// check the validity of magnitude type
void checkMagnitudeType(string magnitudeType);

// check the validity of magnitude
void checkMagnitude(float magnitude);

// return the name of a number month 
string string_to_month(string month);

// check whether a string contains only digits
bool is_digits(string str);

// function to print message to logFile using function printOutput
void errorMessage(const string &message);

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message);

// main function
int main() {

    ifstream inputFile;
    ofstream outputFile;
    string inputFileName;
    int numberOfValidEntries = 0;
    int numberOfEntryRead = 0;
    static const int maximumValidEntries = 300;
    EntryType entry[maximumValidEntries];
    const string outputFileName = "yang.out";

    // prompt user for input file and open it
    cout << "Please Enter input file: ";
    cin >> inputFileName;
    inputFileName = "yang.in";
    openInput(inputFile, inputFileName);

    readHeader(inputFile, outputFile, outputFileName);

    inputFile.close();
    outputFile.close();

    // if open error file, close it
    if (logFile.is_open()) {
        logFile.close();
    }

    return 0;

}

// function to open input file
void openInput(ifstream &inputFile, string fileName) {

    inputFile.open(fileName.c_str());

    // perform sanity check it
    if (!inputFile.is_open()) {
        errorMessage("Cannot open input file: " + fileName + "\n");
    }

}

// function to open output file
void openOutput(ofstream &outputFile, string fileName) {

    outputFile.open(fileName.c_str());

    // perform sanity check it
    if (!outputFile.is_open()) {
        if (fileName == logFileName) {
            // if we can open error file, just print out to terminal
            cout << "Cannot open log file: " << logFileName << endl;
        } else {
            errorMessage("Cannot open output file: " + fileName + "\n");
        }
    }

}

// read header from input file
void readHeader(ifstream &inputFile, ofstream &outputFile, string outputFileName) {

    string eventID, date, time, timeZone, earthquakeName;
    string month, day, year;
    string hour, minute, second, millisecond;
    double evlo, evla, evdp;  // event longitude, latitude and depth
    string magnitudeType;
    float magnitude;

    // first line is event ID
    getline(inputFile, eventID);

    // second line is date time and time zone
    inputFile >> date >> time >> timeZone;

    checkDate(date, month, day, year);
    checkTime(time, hour, minute, second, millisecond);
    checkTimeZone(timeZone);

    // third line is earthquake name
    // avoid the enter key of last line
    getline(inputFile, earthquakeName);
    getline(inputFile, earthquakeName);

    // forth line is events information
    inputFile >> evlo >> evla >> evdp >> magnitudeType >> magnitude;

    checkMagnitudeType(magnitudeType);
    checkMagnitude(magnitude);

    // if all the infomation are correct, then write output header
    openOutput(outputFile, outputFileName);

    outputFile << "# " << day << " " << string_to_month(month) << " " << year << " ";
    outputFile << time << " " << timeZone << " " << magnitudeType << " " << magnitude << " ";
    outputFile << earthquakeName << " [" << eventID << "] (" << evlo << ", " << evla << ", " << evdp << ")" << endl;

}

// check the validity of date and get month day and year
void checkDate(string date, string &month, string &day, string &year) {

    // the length of date must be 9
    if (date.length() == 10) {
        // date format is mm/dd/year or mm-dd-year
        month = date.substr(0, 2);
        day = date.substr(3, 2);
        year = date.substr(6, 4);

        // month, day and year should be numbers 
        if (!is_digits(month + day + year)) {
            errorMessage("Error: invalid date of this earthquake!\n");
        }

        // delimer must be '/' or '-'
        if ((date[2] != '/' || date[5] != '/') && (date[2] != '-' || date[5] != '-')) {
            errorMessage("Error: invalid date format of this earthquake!\n");
        }
    } else {
        errorMessage("Error: invalid date of this earthquake!\n");
    }

}

// check the validity of time and get hour, minute, second and millisecond
void checkTime(string time, string &hour, string &minute, string &second, string millisecond) {

    // the length of time must be 12
    if (time.length() == 12) {
        // time format is hh:mm:ss.fff
        hour = time.substr(0, 2);
        minute = time.substr(3, 2);
        second = time.substr(6, 2);
        millisecond = time.substr(9, 3);

        //  hour, minute, second and millisecond should be numbers 
        if (!is_digits(hour + minute + second + millisecond)) {
            errorMessage("Error: invalid time of this earthquake!\n");
        }

        // check for delimer
        if (time[2] != ':' || time[5] != ':' || time[8] != '.') {
            errorMessage("Error: invalid time format of this earthquake!\n");
        }
    } else {
        errorMessage("Error: invalid time of this earthquake!\n");
    }
}

// check the validity of time zone
void checkTimeZone(string timeZone) {

    // time zone must be three characters
    if (timeZone.length() != 3) {
        errorMessage("Error: invalid time zone of this earthquake!\n");
    }
}

// check the validity of magnitude type
void checkMagnitudeType(string magnitudeType) {

    // the length of magnitudeType must be 2
    if (magnitudeType.length() == 2) {
        // magnitude type must be one of ML, Ms, Mb or Mw, which is case insensitive
        // get the lowercase of first and second letter of magnitude type
        char l1 = tolower(magnitudeType[0]);
        char l2 = tolower(magnitudeType[1]);

        if (l1 != 'm' || (l2 !='l' && l2 !='s' && l2 !='b' && l2 !='w')) {
            errorMessage("Error: invalid magnitude type of this earthquake!\n");
        }
    } else {
        errorMessage("Error: invalid magnitude type of this earthquake!\n");
    }

}

// check the validity of magnitude
void checkMagnitude(float magnitude) {

    // magnitude must be a positive number
    if (magnitude <= 0) {
        errorMessage("Error: magnitude must be a positive number\n");
    }

}

// return the name of a number month 
string string_to_month(string month) {

    if (month == "01") return "January";
    if (month == "02") return "February";
    if (month == "03") return "March";
    if (month == "04") return "April";
    if (month == "05") return "May";
    if (month == "06") return "June";
    if (month == "07") return "July";
    if (month == "08") return "August";
    if (month == "09") return "September";
    if (month == "10") return "October";
    if (month == "11") return "November";
    if (month == "12") return "December";
    
    errorMessage("Error: invalid month of this earthquake!\n");

    return "Just show this line to avoid warning";

}

// check whether a string contains only digits
bool is_digits(string str) {

    for (int i = 0; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;

}

// function to print message to logFile using function printOutput
void errorMessage(const string &message) {

    // check whether the error file is opened
    if (!logFile.is_open()) {
        openOutput(logFile, logFileName);
    }

    printOutput(logFile, message);

    exit(EXIT_FAILURE);

}

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message) {

    cout << message;
    outputFile << message;

}