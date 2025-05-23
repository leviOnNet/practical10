#ifndef STUDENTDETAILS_H
#define STUDENTDETAILS_H

#include <string>
#include <sstream>

struct StudentDetails{
    std::string firstName;
    std::string lastName;
    float mark;
    int studentNumber;
    bool didPrepWork;
};

StudentDetails* constructor(std::string line);
StudentDetails* constructor(std::string fName, std::string lName, int studentNumber, float mark,  bool didPrepWork);
void destructor(StudentDetails*& stdDetails);
std::string toString(StudentDetails& stdDetails);

#endif /*STUDENTDETAILS_H*/