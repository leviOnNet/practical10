#include "StudentDetails.h"
#include "Assessment.h"
#include <iostream>
#include <string>

int main() {
    Assessment* assess = constructor("Final Exam", 100.0f);
    loadFromCSV( assess,"students.csv");

    std::cout << "Total students loaded: " << totalNumberOfStudents(assess) << std::endl;

    // You can now use all your analytics functions:
    std::cout << "Pass rate: " << passRate(assess) << "%" << std::endl;

    destructor(assess);
    return 0;
}

