#include "StudentDetails.h"
#include "Assessment.h"
#include <iostream>
#include <string>


int main() {
    Assessment* assess = constructor("Final Exam", 100.0f);
    loadFromCSV(assess, "students.csv");

    std::cout << "===== Assessment Analytics =====" << std::endl;
    std::cout << "Assessment Name: " << assess->assessmentName << std::endl;
    std::cout << "Full Marks: " << assess->fullMarks << std::endl;

    int total = totalNumberOfStudents(assess);
    std::cout << "Total students loaded: " << total << std::endl;

    int completedPrep = numberThatCompletedPrep(assess);
    std::cout << "Students who completed prep work: " << completedPrep << std::endl;

    float average = avg(assess);
    std::cout << "Average mark: " << average << std::endl;

    float pass = passRate(assess);
    std::cout << "Pass rate: " << pass << "%" << std::endl;

    int distinctionCount = distinction(assess);
    std::cout << "Number of distinctions: " << distinctionCount << std::endl;

    int fullMarksCount = fullMarks(assess);
    std::cout << "Number of full marks: " << fullMarksCount << std::endl;

    StudentDetails* best = bestStudent(assess);
    if (best) {
        std::cout << "Top student: " << best->firstName << " " << best->lastName
                  << " - Mark: " << best->mark << std::endl;
    }

    StudentDetails* worst = worstStudent(assess);
    if (worst) {
        std::cout << "Lowest student: " << worst->firstName << " " << worst->lastName
                  << " - Mark: " << worst->mark << std::endl;
    }

    // Optionally display histogram (if implemented)
    char** hist = marksHistogram(assess);
    std::cout << "\n===== Mark Distribution Histogram =====" << std::endl;
    for (int i = 0; i < 11; ++i) {
        std::cout << (i * 10) << "-" << ((i + 1) * 10 - 1) << ": " << hist[i] << std::endl;
        delete[] hist[i]; // clean row
    }
    delete[] hist;

    destructor(assess);
    return 0;
}


