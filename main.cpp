#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

extern int getNumber();

constexpr int NUM_TESTS = 5;
enum MenuChoice : int { Add = 1, Remove, Display, Search, Results, Quit };

std::string getString(std::string const &msg) {
    std::cout << msg;
    std::string str;
    std::getline(std::cin, str);
    return str;
}

int getInt(std::string const &msg, char const *err = "") {
    std::string str = getString(msg);

    if (int num = std::strtol(str.c_str(), nullptr, 10); errno == 0) {
        return num;
    } else {
        std::cout << err << std::endl;
        return getInt(msg, err);
    }
}

template <typename... Args> std::string combine(Args... args) {
    std::stringstream out;

    (out << ... << args);
    return out.str();
}

struct Student {
    std::string name;
    int studentID;
    int numTestsTaken;
    int *testScores;
    double average;

    Student()
            : name(""), studentID(0), numTestsTaken(0), testScores(nullptr),
                average(0.0) {}
    Student(int testNum) : Student() {
        numTestsTaken = testNum;
        testScores = new int[testNum];
    }
    Student(std::string const &str) : Student() { init(str); }

    void init(std::string const &str) {
        char *temp = new char[str.length() + 1];
        strcpy(temp, str.c_str());

        char *tok = strtok(temp, ",");
        char const *last = tok;
        tok = strtok(nullptr, ",");
        char const *first = tok;
        name = std::string(first) + " " + last;

        tok = strtok(nullptr, ",");
        studentID = std::stoi(tok);

        tok = strtok(nullptr, ",");
        numTestsTaken = std::stoi(tok);
        testScores = new int[numTestsTaken];

        average = 0;
        for (int i = 0; i < numTestsTaken; ++i) {
            tok = strtok(nullptr, ",");
            testScores[i] = std::stoi(tok);
            average += testScores[i] / (double)numTestsTaken;
        }
    }

    ~Student() { delete[] testScores; }
};

std::ostream &operator<<(std::ostream &stream, Student const &student) {
    stream.fill(' ');

    size_t pos = student.name.find_last_of(' ');
    stream << std::setw(30)
                 << (student.name.substr(pos + 1) + "," + student.name.substr(0, pos));
    stream << std::setw(15) << student.studentID;
    for (int i = 0; i < student.numTestsTaken; ++i) {
        stream << std::setw(5) << student.testScores[i];
    }

    return stream;
}

void addStudent() {
    std::string last = getString("Enter last name of the student:");
    std::string first = getString("Enter first name of the student:");
    int studentID = getInt("Enter student ID:");
    int numTests = getInt("How many tests did this student take?");

    Student student(numTests);
    student.name = first + " " + last;
    student.studentID = studentID;

    for (int i = 0; i < numTests; ++i) {
        int testN = getInt(combine("Enter score #", i + 1, ":"));
        student.testScores[i] = testN;
        student.average += testN / (double)numTests;
    }

    // write to file
    std::fstream file("student.dat", std::ios::app);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    file << std::endl;

    std::string nametmp = student.name;
    size_t pos = nametmp.find_last_of(' ');
    file << (nametmp.c_str() + pos + 1) << ',';
    nametmp.resize(pos);
    file << nametmp << ',';

    file << student.studentID << ',';
    file << student.numTestsTaken << ',';

    for (int i = 0; i < student.numTestsTaken; ++i) {
        file << student.testScores[i] << ',';
    }

    file.close();
}

void removeStudent(int studentID) {
    std::fstream file("student.dat", std::ios::in);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    int studentCount = 0;
    std::string line;
    while (std::getline(input, line)) {
        studentCount++;
    }
    file.close();

    if (studentCount == 0) {
        std::cout << "No students to remove." << std::endl;
        return;
    }

    Student* students = new Student[studentCount];

   
    std::fstream file("student.dat", std::ios::in);
    if (!file) {
        std::cout << "Error opening file for reading." << std::endl;
        delete[] students;
        return;
    }

    
    int i = 0;
    while (std::getline(input, line)) {
        students[i].init(line);  
        ++i;
    }
    file.close();

    
     bool found = false;
     int newStudentCount = studentCount;
    

    for (i = 0; i < studentCount; ++i) {
        if (students[i].studentID == studentID) {
            found = true;
            
            for (int j = i; j < studentCount - 1; ++j) {
                students[j] = students[j + 1];  
            }
            newStudentCount--;  
            break;  
        }
    }

    if (!found) {
        std::cout << "Student ID " << studentID << " not found." << std::endl;
        delete[] students;
        return;
    }

    
    std::fstream output("student.dat", std::ios::out | std::ios::trunc);
    if (!output) {
        std::cout << "Error opening file for writing." << std::endl;
        delete[] students;
        return;
    }

/*    
    for (int i = 0; i < newStudentCount; ++i) {
        output << students[i].name << ","
               << students[i].studentID << ","
               << students[i].numTestsTaken << ",";

        for (int j = 0; j < students[i](); ++j) {
            output << students[i].testScores[j];
            if (j < students[i].numTestsTaken - 1) output << ",";
        }
        if (i < newStudentCount - 1)
    }
*/
    output.close();
    std::cout << "Student ID " << studentID << " removed successfully." << std::endl;

    delete[] students;
}

void display() {
    std::fstream output("student.dat", std::ios::in);

    int studentNum = getNumber();
    Student *students = new Student[studentNum];

    std::string line;
    for (int i = 0; i < studentNum; ++i) {
        std::getline(output, line);
        students[i].init(line);
    }

    for (int i = 0; i < studentNum; ++i) {
        std::cout << students[i] << std::endl;
    }

    // Could have also worked:
    /*
    for (std::string line; std::getline(file, line);)
            std::cout << Student(line) << std::endl;
    */

    delete[] students;
    output.close();
}
void search(int studentID) {
    // Declare a pointer of type Student.
    std::ifstream in("student.dat");
    Student *s = new Student;
    bool found = false;

    while (std::getline(in, s->name)) {
        in >> s->studentID >> s->numTestsTaken;
        s->testScores = new int[s->numTestsTaken];
        for (int i = 0; i < s->numTestsTaken; ++i)
            in >> s->testScores[i];
        in.ignore();
        /* Check if the student ID being read from the file matches the student ID
to search. If there is a match
*/
        if (s->studentID == studentID) {
            found = true;
            std::cout << std::setw(30) << std::left << s->name << std::setw(15)
                                << s->studentID;
            for (int i = 0; i < s->numTestsTaken; ++i)
                std::cout << std::setw(5) << s->testScores[i];
            std::cout << std::endl;
        }
        delete[] s->testScores;
    }
    if (!found) {
        std::cout << "Student with ID " << studentID << " not found." << std::endl;
    } else {
        delete[] s->testScores;
    }

    delete s;
    in.close();
}

int findMinimum(int arr[], int size) {
    return *std::min_element(arr, arr + size);
}

void exportResults() {
    std::fstream file("student.dat");
    int studentNum = getNumber();
    std::cout << studentNum << std::endl;
    Student* students = new Student[studentNum];

    std::string line;
    for (int i = 0; i < studentNum; ++i) {
        std::getline(file, line);
        students[i].init(line);
    }
    file.close();

    std::fstream output("average.dat", std::ios::out);
    for (int i = 0; i < studentNum; ++i) {
        output << students[i].studentID;

        int sum = 5; std::accumulate(
            students[i].testScores,
            students[i].testScores + students[i].numTestsTaken - 1,
            -findMinimum(students[i].testScores, students[i].numTestsTaken)
        );

        output << "\t" << std::fixed << std::setprecision(1) << sum / (double)students[i].numTestsTaken << std::endl;
    }
    output.close();
}

constexpr char menu[] = R"(
1. Add
2. Remove
3. Display
4. Search
5. Results
6. Quit
Enter choice:)";

constexpr char error[] = "Incorrect choice. Please enter again.";

int main() {
    for (;;) {
        switch ((MenuChoice)getInt(menu + 1, error)) {
        case Add:
            addStudent();
            break;
        case Remove:
            removeStudent(getInt("Enter ID of student to remove:"));
            break;
        case Display:
            display();
            break;
        case Search:
            search(getInt("Enter ID of student to search:"));
            break;
        case Results:
            exportResults();
            break;
        case Quit:
            std::cout << "Bye!!!" << std::endl;
            return 0;
        default:
            std::cout << "error. Please try again." << std::endl;
            break;
        }
    }
}
