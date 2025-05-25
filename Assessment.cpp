#include "Assessment.h"
#include "StudentDetails.h"


// Comparison function for sort
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
    if (!assessment) return;
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
    // Get bucket index based on the first letter of last name
    char firstChar = std::toupper(newStudent.lastName[0]);
    int index = firstChar - 'A';

    // Safety check
    if (index < 0 || index >= 26) return;

    int oldSize = assessment->numberOfStudents[index];
    int newSize = oldSize + 1;

    // Allocate new array for the bucket
    StudentDetails** newBucket = new StudentDetails*[newSize];

    // Copy existing student pointers
    for (int i = 0; i < oldSize; ++i) {
        newBucket[i] = assessment->roster[index][i];
    }

    // Deep copy the new student
    newBucket[oldSize] = constructor(
        newStudent.firstName,
        newStudent.lastName,
        newStudent.studentNumber,
        newStudent.mark,
        newStudent.didPrepWork
    );

    // Delete old bucket
    delete[] assessment->roster[index];

    // Assign updated bucket
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
    float total = 0;
    int count = 0;

    for (int i = 0; i < 26; ++i) {
        int studentsInBucket = assessment->numberOfStudents[i]; // not a pointer!
        for (int j = 0; j < studentsInBucket; ++j) {
            if (assessment->roster[i][j] != NULL) {
                total += assessment->roster[i][j]->mark;
                count++;
            }
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
    // Validate input
    if (assessment == NULL ||
        assessment->roster == NULL ||
        assessment->numberOfStudents == NULL ||
        assessment->fullMarks == 0.0f) {
        return 0.0f;
    }

    int total = 0;
    int passed = 0;
    float passMark = assessment->fullMarks * 0.5f;

    for (int i = 0; i < 26; ++i) {
        int count = assessment->numberOfStudents[i];
        if (count > 0) {
            for (int j = 0; j < count; ++j) {
                StudentDetails* student = assessment->roster[i][j];
                if (student != NULL && student->mark >= passMark) {
                    passed++;
                }
            }
            total += count;
        }
    }

    if (total == 0) return 0.0f;

    return (passed * 100.0f) / total;
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
            if (best == NULL || assessment->roster[i][j]->mark > best->mark) {
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
            if (worst == NULL || assessment->roster[i][j]->mark < worst->mark) {
                worst = assessment->roster[i][j];
            }
        }
    }
    return worst;
}

char** marksHistogram(Assessment* assessment) {
    char** grid = new char*[11];
    for (int i = 0; i < 11; ++i) {
        grid[i] = new char[26];
        for (int j = 0; j < 26; ++j) {
            grid[i][j] = ' ';
        }
    }

    for (int col = 0; col < 26; ++col) {
        for (int row = 0; row < assessment->numberOfStudents[col]; ++row) {
            float mark = assessment->roster[col][row]->mark;
            int band = (int)((mark / assessment->fullMarks) * 10.0f);
            if (band > 10) band = 10;
            grid[band][col] = '*';
        }
    }
    return grid;
}



void loadFromCSV(Assessment* assessment, std::string fileName) {
    std::ifstream file(fileName.c_str());
    std::string line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string firstName, lastName, studentNumberStr, markStr, didPrepStr;

        std::getline(ss, firstName, ',');
        std::getline(ss, lastName, ',');
        std::getline(ss, studentNumberStr, ',');
        std::getline(ss, markStr, ',');
        std::getline(ss, didPrepStr, ',');

        int studentNumber;
        float mark;

        std::stringstream ssNum(studentNumberStr);
        ssNum >> studentNumber;

        std::stringstream ssMark(markStr);
        ssMark >> mark;

        bool didPrep = (didPrepStr == "t");

        StudentDetails* student = constructor(
            firstName,
            lastName,
            studentNumber,
            mark,
            didPrep
        );

        insertStudent(assessment, *student);
        // Do not delete student — roster owns the pointer.
    }
}
