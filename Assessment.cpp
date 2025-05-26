#include "Assessment.h"
#include "StudentDetails.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype> // Only needed if allowed for toupper

// Helper for sorting
bool compareStudents(StudentDetails* a, StudentDetails* b) {
    return a->lastName < b->lastName;
}

Assessment* constructor(std::string assessmentName, float fullMarks) {
    Assessment* a = new Assessment;
    a->assessmentName = assessmentName;
    a->fullMarks = fullMarks;
    a->roster = new StudentDetails**[26];
    a->numberOfStudents = new int[26];

    for (int i = 0; i < 26; ++i) {
        a->roster[i] = NULL;
        a->numberOfStudents[i] = 0;
    }

    return a;
}

void destructor(Assessment*& assessment) {
    if (assessment == NULL) return;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            destructor(assessment->roster[i][j]);
        }
        delete[] assessment->roster[i];
    }

    delete[] assessment->roster;
    delete[] assessment->numberOfStudents;
    delete assessment;
    assessment = NULL;
}

void insertStudent(Assessment*& assessment, StudentDetails newStudent) {
    char firstChar = newStudent.lastName[0];
    if (firstChar >= 'a' && firstChar <= 'z') {
        firstChar = firstChar - ('a' - 'A');
    }

    int index = firstChar - 'A';
    if (index < 0 || index >= 26) return;

    int oldSize = assessment->numberOfStudents[index];
    int newSize = oldSize + 1;

    StudentDetails** newBucket = new StudentDetails*[newSize];

    for (int i = 0; i < oldSize; ++i) {
        newBucket[i] = assessment->roster[index][i];
    }

    newBucket[oldSize] = constructor(
        newStudent.firstName,
        newStudent.lastName,
        newStudent.studentNumber,
        newStudent.mark,
        newStudent.didPrepWork
    );

    delete[] assessment->roster[index];
    assessment->roster[index] = newBucket;
    assessment->numberOfStudents[index] = newSize;
}

void sort(Assessment* assessment) {
    for (int i = 0; i < 26; ++i) {
        int count = assessment->numberOfStudents[i];
        StudentDetails** arr = assessment->roster[i];

        for (int j = 0; j < count - 1; ++j) {
            for (int k = 0; k < count - j - 1; ++k) {
                if (compareStudents(arr[k + 1], arr[k])) {
                    StudentDetails* temp = arr[k];
                    arr[k] = arr[k + 1];
                    arr[k + 1] = temp;
                }
            }
        }
    }
}

float avg(Assessment* assessment) {
    float total = 0.0f;
    int count = 0;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            total += assessment->roster[i][j]->mark;
            count++;
        }
    }

    return (count == 0) ? 0.0f : (total / count);
}

int totalNumberOfStudents(Assessment* assessment) {
    int total = 0;
    for (int i = 0; i < 26; ++i) {
        total += assessment->numberOfStudents[i];
    }
    return total;
}

int numberThatCompletedPrep(Assessment* assessment) {
    int total = 0;
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            if (assessment->roster[i][j]->didPrepWork) {
                total++;
            }
        }
    }
    return total;
}

float passRate(Assessment* assessment) {
    if (!assessment || !assessment->roster || !assessment->numberOfStudents || assessment->fullMarks <= 0.0f) {
        return 0.0f;
    }

    int total = 0;
    int passed = 0;
    float passMark = assessment->fullMarks * 0.5f;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            total++;
            if (assessment->roster[i][j]->mark >= passMark) {
                passed++;
            }
        }
    }

    return (total == 0) ? 0.0f : (passed * 100.0f / total);
}

int distinction(Assessment* assessment) {
    int count = 0;
    float threshold = assessment->fullMarks * 0.75f;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            if (assessment->roster[i][j]->mark >= threshold) {
                count++;
            }
        }
    }

    return count;
}

int fullMarks(Assessment* assessment) {
    int count = 0;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            if (assessment->roster[i][j]->mark == assessment->fullMarks) {
                count++;
            }
        }
    }

    return count;
}

StudentDetails* bestStudent(Assessment* assessment) {
    StudentDetails* best = NULL;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            if (!best || assessment->roster[i][j]->mark > best->mark) {
                best = assessment->roster[i][j];
            }
        }
    }

    return best;
}

StudentDetails* worstStudent(Assessment* assessment) {
    StudentDetails* worst = NULL;

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < assessment->numberOfStudents[i]; ++j) {
            if (!worst || assessment->roster[i][j]->mark < worst->mark) {
                worst = assessment->roster[i][j];
            }
        }
    }

    return worst;
}

char** marksHistogram(Assessment* assessment) {
    // Validation check
    if (!assessment || !assessment->roster || !assessment->numberOfStudents || assessment->fullMarks <= 0.0f) {
        char** invalidGrid = new char*[1];
        invalidGrid[0] = new char[27];
        for (int i = 0; i < 26; ++i) {
            invalidGrid[0][i] = 'A' + i;
        }
        invalidGrid[0][26] = '\0'; // null-terminate
        return invalidGrid;
    }

    // Create 11x26 grid
    char** grid = new char*[11];
    for (int i = 0; i < 11; ++i) {
        grid[i] = new char[27];
        for (int j = 0; j < 26; ++j) {
            grid[i][j] = ' ';
        }
        grid[i][26] = '\0'; // null-terminate each row
    }

    // Last row = A-Z labels
    for (int col = 0; col < 26; ++col) {
        grid[10][col] = 'A' + col;
    }

    // Compute average mark per bucket and plot 'X'
    for (int col = 0; col < 26; ++col) {
        int count = assessment->numberOfStudents[col];
        float total = 0.0f;

        for (int j = 0; j < count; ++j) {
            total += assessment->roster[col][j]->mark;
        }

        if (count > 0) {
            float avg = total / count;
            int row = static_cast<int>((avg / assessment->fullMarks) * 10.0f);
            if (row > 9) row = 9; // avoid exceeding bounds

            // Fill from row 0 up to computed row (inclusive) with 'X'
            for (int r = 9 - row; r < 10; ++r) {
                grid[r][col] = 'X';
            }
        }
    }

    return grid;
}


void loadFromCSV(Assessment* assessment, std::string fileName) {
    std::ifstream file(fileName.c_str());
    if (!file.is_open()) return;

    std::string line;
    std::getline(file, line); // skip header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string firstName, lastName, studentNumberStr, markStr, didPrepStr;

        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, studentNumberStr, ',');
        std::getline(ss, markStr, ',');
        std::getline(ss, didPrepStr, ',');

        int studentNumber = 0;
        float mark = 0.0f;

        std::stringstream ssNum(studentNumberStr);
        ssNum >> studentNumber;

        std::stringstream ssMark(markStr);
        ssMark >> mark;

        bool didPrep = (didPrepStr == "t");

        StudentDetails* student = constructor(firstName, lastName, studentNumber, mark, didPrep);
        insertStudent(assessment, *student);
        destructor(student); // free temporary instance
    }

    file.close();
}
