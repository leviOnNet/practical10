#include "StudentDetails.h"
#include <sstream>
#include <iostream>


// Constructor from CSV line: "firstName,lastName,studentNumber,mark,didPrepWork"
StudentDetails* constructor(std::string line) {
    std::stringstream ss(line);
    std::string token;
    std::string fName, lName;
    int studentNumber;
    float mark;
    bool didPrepWork;

    getline(ss, fName, ',');
    getline(ss, lName, ',');

    getline(ss, token, ',');
    std::stringstream ssNum(token);
    ssNum >> studentNumber;

    getline(ss, token, ',');
    std::stringstream ssMark(token);
    ssMark >> mark;

    getline(ss, token, ',');
    didPrepWork = (token == "true" || token == "1");

    return constructor(fName, lName, studentNumber, mark, didPrepWork);
}

// Constructor from individual fields
StudentDetails* constructor(std::string fName, std::string lName, int studentNumber, float mark, bool didPrepWork) {
    StudentDetails* stdDetails = new StudentDetails;
    stdDetails->firstName = fName;
    stdDetails->lastName = lName;
    stdDetails->studentNumber = studentNumber;
    stdDetails->mark = mark;
    stdDetails->didPrepWork = didPrepWork;
    return stdDetails;
}

// Destructor
void destructor(StudentDetails*& stdDetails) {
    if (stdDetails != NULL) {
        delete stdDetails;
        stdDetails = NULL;
    }
}

// Convert StudentDetails to string
std::string toString(StudentDetails& stdDetails) {
    std::ostringstream oss;
    oss << stdDetails.firstName << " "
        << stdDetails.lastName << ", "
        << "Student Number: " << stdDetails.studentNumber << ", "
        << "Mark: " << stdDetails.mark << ", "
        << "Prep Work: " << (stdDetails.didPrepWork ? "Yes" : "No");
    return oss.str();
}
