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
struct entryType {
    networkCodeType networkCode;
    sting stationCode;
    typeOfBandType typeOfBand;
    typeOfInstrumentType typeOfInstrument;
    string orientation; // one to three characters, case insensitive
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
    high-gain,  // H
    low-gain,  // L
    accelerometer  // N
}

// function to open input file
void openInput(ifstream &inputFile, string fileName);

// function to open output file
void openOutput(ofstream &outputFile, string fileName);

// read header can then produce output header
void headerProcessing(ifstream &inputFile, ofstream &outputFile, string outputFileName);

// read table and then produce output
void tableProcessing(ifstream &inputFile, ofstream &outputFile);

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

// read and processing one entry
bool processOneEntry(ifstream &inputFile, entryType &entry, int entryNumber);

// convert a string to networkCodeType
bool networkCode_str2enum(string network_str, networkCodeType &network_enum);

// check the validity of station code
bool checkStationCode(string str);

// convert a string to typeOfBandType
bool bandType_str2enum(string bandType_str, typeOfBandType &bandType_enum);

// convert a string to typeOfInstrument
bool instrumentType_str2enum(string instrumentType_str, typeOfInstrumentType &instrumentType_enum);

// check the validity of orientation
bool checkOrientation(string str);

// return the name of a number month 
string string_to_month(string month);

// check whether a string contains only digits
bool is_digits(string str);

// change all the letters in a string to lower case
void lowerString(string &str);

// function to print error message to both terminal and error file
void errorMessage(const string &message);

// function to print error message to both terminal and error file and then exit the program
void errorMessageWithExit(const string &message);

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message);

// main function
int main() {

    ifstream inputFile;
    ofstream outputFile;
    string inputFileName;
    const string outputFileName = "yang.out";

    // prompt user for input file and open it
    cout << "Please Enter input file: ";
    cin >> inputFileName;
    inputFileName = "yang.in";
    openInput(inputFile, inputFileName);

    headerProcessing(inputFile, outputFile, outputFileName);
    tableProcessing(inputFile, outputFile);

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
        errorMessageWithExit("Cannot open input file: " + fileName + "\n");
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
            errorMessageWithExit("Cannot open output file: " + fileName + "\n");
        }
    }

}

// read header from input file
void headerProcessing(ifstream &inputFile, ofstream &outputFile, string outputFileName) {

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

// read table and then produce output
void tableProcessing(ifstream &inputFile, ofstream &outputFile) {

    int numberOfValidEntries = 0;
    int numberOfEntryRead = 0;
    static const int maximumValidEntries = 300;
    entryType entry[maximumValidEntries];

    // read the file to the end of the file or reach maximum valid entry number
    do {
        numberOfEntryRead++;

        if (processOneEntry(inputFile, entry[numberOfValidEntries], numberOfEntryRead)) {
            numberOfValidEntries++;
        }

    } while (!inputFile.eof() && numberOfValidEntries < maximumValidEntries)
    
    // print all the signals to output file
    for (int i = 0; i < numberOfValidEntries; i++) {
        for (int j = 0; j < entry[i].orientation.length(); j++) {
            stringstream singalStream;
            singalStream << getNetworkCode(entry[i].networkCode) << ".";
            singalStream << getStationCode(entry[i].stationCode) << ".";
            singalStream << getBandType(entry[i].typeOfBand);
            singalStream << getInstrumentType(entry[i].typeOfInstrument);
            singalStream << entry[i].orientation[j] << endl;

            outputFile << singalStream.str();
        }
    }
    
}

// read and processing one entry
bool processOneEntry(ifstream &inputFile, entryType &entry, int entryNumber) {

    string tmpStr;  // read as string and them convert it to enum type
    bool isValidEntry = true;

    // read network and them convert it to enum type
    inputFile >> tmpStr;
    if (!networkCode_str2enum(tmpStr, entry.networkCode)) {
        errorMessage("Entry # " + to_string(entryNumber) + " ignored. Invalid network");
        isValidEntry = false;
    }

    // read station code
    inputFile >> entry.stationCode;
    if (!checkStationCode(entry.stationCode)) {
        errorMessage("Entry # " + to_string(entryNumber) + " ignored. Invalid station code");
        isValidEntry = false;
    }

    // read type of instrument and convert it to enum type
    inputFile >> tmpStr;
    if (!bandType_str2enum(tmpStr, entry.typeOfBand)) {
        errorMessage("Entry # " + to_string(entryNumber) + " ignored. Invalid band type");
        isValidEntry = false;
    }

    // read type of band and convert it to enum type
    inputFile >> tmpStr;
    if (!instrumentType_str2enum(tmpStr, entry.typeOfInstrument)) {
        errorMessage("Entry # " + to_string(entryNumber) + " ignored. Invalid instrument type");
        isValidEntry = false;
    }

    // read orientation
    inputFile >> entry.orientation;
    if (!checkOrientation(entry.orientation)) {
        errorMessage("Entry # " + to_string(entryNumber) + " ignored. Invalid orientation");
        isValidEntry = false;
    }

    return isValidEntry;

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
            errorMessageWithExit("Error: invalid date of this earthquake!\n");
        }

        // delimer must be '/' or '-'
        if ((date[2] != '/' || date[5] != '/') && (date[2] != '-' || date[5] != '-')) {
            errorMessageWithExit("Error: invalid date format of this earthquake!\n");
        }
    } else {
        errorMessageWithExit("Error: invalid date of this earthquake!\n");
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
            errorMessageWithExit("Error: invalid time of this earthquake!\n");
        }

        // check for delimer
        if (time[2] != ':' || time[5] != ':' || time[8] != '.') {
            errorMessageWithExit("Error: invalid time format of this earthquake!\n");
        }
    } else {
        errorMessageWithExit("Error: invalid time of this earthquake!\n");
    }
}

// check the validity of time zone
void checkTimeZone(string timeZone) {

    // time zone must be three characters
    if (timeZone.length() != 3) {
        errorMessageWithExit("Error: invalid time zone of this earthquake!\n");
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
            errorMessageWithExit("Error: invalid magnitude type of this earthquake!\n");
        }
    } else {
        errorMessageWithExit("Error: invalid magnitude type of this earthquake!\n");
    }

}

// check the validity of magnitude
void checkMagnitude(float magnitude) {

    // magnitude must be a positive number
    if (magnitude <= 0) {
        errorMessageWithExit("Error: magnitude must be a positive number\n");
    }

}

// convert a string to networkCodeType
bool networkCode_str2enum(string network_str, networkCodeType &network_enum) {

    if (network_str == "CE") {
        network_enum = CE;
        return true;
    }

    if (network_str == "CI") {
        network_enum = CI;
        return true;
    }

    if (network_str == "FA") {
        network_enum = FA;
        return true;
    }

    if (network_str == "NP") {
        network_enum = NP;
        return true;
    }

    if (network_str == "WR") {
        network_enum = WR;
        return true;
    }

    // otherwise it's a invalid network
    return false;

}

// check the validity of station code
bool checkStationCode(string str) {

    // station code must be 3 captital letters or 5 numeric characters
    if (str.length() == 5 && is_digits(stationCode_str)) {
        return true;
    }

    if (str.length() == 3) {
        if (isupper(ststr(0)) && isupper(ststr(1)) && isupper(ststr(2))) {
            return true;
        }
    }

    return false;
}

// convert a string to typeOfBandType
bool bandType_str2enum(string bandType_str, typeOfBandType &bandType_enum) {

    // case insensitive so convert it to lower case first
    bandType_str = lowerString(bandType_str);

    if (bandType_str == "long-period") {
        bandType_enum = long-period;
        return true;
    }

    if (bandType_str == "short-period") {
        bandType_enum = short-period;
        return true;
    }

    if (bandType_str == "broadband") {
        bandType_enum = broadband;
        return true;
    }

    return false;
}

// convert a string to typeOfInstrument
bool instrumentType_str2enum(string instrumentType_str, typeOfInstrumentType &instrumentType_enum) {

    // case insensitive so convert it to lower case first
    instrumentType_str = lowerString(instrumentType_str);

    if (instrumentType_str == "high-gain") {
        instrumentType_enum = high-gain;
        return true;
    }

    if (instrumentType_str == "low-gain") {
        instrumentType_enum = low-gain;
        return true;
    }

    if (instrumentType_str == "accelerometer") {
        instrumentType_enum = accelerometer;
        return true;
    }

    return false;

}

// check the validity of orientation
bool checkOrientation(string str) {

    // case insensitive so convert it to lower case first
    str = lowerString(str);

    if (str.length() < 4) {
        if (isdigit(str(0))) {
            for (int i = 1; i < str.length(); i++) {
                if (!isdigit(str(i))) {
                    return false;
                } else {
                    return true;
                }
            }
        } else if (islower(str(0))) {
            for (int i = 1; i < str.length(); i++) {
                if (str(i) != 'n' && str(i) != 'e' && str(i) != 'z') {
                    return false;
                } else {
                    return true;
                }
            }
        }
    }

    return false;

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
    
    errorMessageWithExit("Error: invalid month of this earthquake!\n");

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

// change all the letters in a string to lower case
void lowerString(string &str) {

    for (int i = 0; i < str.length(); i++) {
        str[i] = tolower(str[i]);
    }

}

// function to print error message to both terminal and error file
void errorMessage(const string &message) {

    // check whether the error file is opened
    if (!logFile.is_open()) {
        openOutput(logFile, logFileName);
    }

    printOutput(logFile, message);

}

// function to print error message to both terminal and error file and then exit the program
void errorMessageWithExit(const string &message) {

    errorMessage(message);

    exit(EXIT_FAILURE);

}

// print messeage to both terminal and a file
void printOutput(ofstream &outputFile, const string &message) {

    cout << message;
    outputFile << message;

}