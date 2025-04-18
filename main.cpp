#include <iostream>
#include <fstream>
#include <string>

extern int getNumber();

constexpr int NUM_TESTS = 5;
enum MenuChoice {
    Add = 1, Remove, Display, Search, Results, Quit};

template <typename T>
T getInput(char const* msg) {
    T out;
    std::cout << msg;
    if constexpr (std::is_same<T, std::string>::value)
        std::getline(std::cin, out);
    else
        std::cin >> out;
    return out;
}

struct Student {
    std::string name;
    int studentID;
    int numTestsTaken;
    int* testScores;
    double average;

    Student() : name(""), studentID(0), numTestsTaken(0), testScores(nullptr), average(0.0) {}
    Student(int testNum) : Student() {
        numTestsTaken = testNum; 
        testScores = new int[testNum];
    }
    Student(std::string const& str) : Student() { init(str); }

    void init(std::string const& str) {
        char* temp = new char[str.length() + 1];
        strcpy(temp, str.c_str());

        char* tok = strtok(temp, ",");
        char const* last = tok;
        tok = strtok(nullptr, ",");
        char const* first = tok;
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




std::ostream& operator<<(std::ostream& stream, Student const& student) {
    stream.fill(' ');

    size_t pos = student.name.find_last_of(' ');
    stream << std::setw(30) << (student.name.substr(pos + 1) + "," + student.name.substr(0, pos));
    stream << std::setw(15) << student.studentID;
    for (int i = 0; i < student.numTestsTaken; ++i) {
        stream << std::setw(5) << student.testScores[i];
    }

    return stream;
}

void addStudent() {
    std::string last = getInput<std::string>("Enter last name of the student:");
    std::string first = getInput<std::string>("Enter first name of the student:");
    int studentID = getInput<int>("Enter student ID:");
    int numTests = getInput<int>("How many tests did this student take?");

    Student student(numTests);
    student.name = first + " " + last;
    student.studentID = studentID;

    for (int i = 0; i < numTests; ++i) {
        int testN = getInput<int>((std::string("Enter score #") + std::to_string(i + 1) + ":").c_str());
        student.testScores[i] = testN;
        student.average += testN / (double)numTests;
    }

    // write to file
    std::fstream file("student.dat", std::ios::app);

    std::string nametmp = student.name;
    size_t pos = nametmp.find_last_of(' ');
    file << (nametmp.c_str() + pos) << ',';
    nametmp[pos] = '\0';
    file << nametmp << ',';

    file << student.studentID << ',';
    file << student.numTestsTaken << ',';

    for (int i = 0; i < student.numTestsTaken; ++i) {
        file << student.testScores[i] << ',';
    }

    file << std::endl;
    file.close();
}

void removeStudent(int studentID) {
      // Call getNumber to get current number of students in data file
        //getNumber(){
            
        //}
    // insert dynamic array of the structure type Student. use # of students in the file.
        //struct Student() {
        std::fstream file("students.dat", std::ios::in | std::ios::out);
    /* In a loop, read one line of the file at a time and store appropriate data in the dynamic array.
  - While reading check if the student ID being read from the file matches the student ID of the student to be removed.
 - If you find match, use a Boolean flag to store that you have found a match.
 - Copy the entire file to your dynamic array whether or not you find a match.
        */
       file.close();
}
void display() {
    std::fstream file("student.dat", std::ios::in);

    int studentNum = getNumber();
    Student* students = new Student[studentNum];

    std::string line;
    for (int i = 0; i < studentNum; ++i) {
        std::getline(file, line);
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

    file.close();
}
void search(int studentID) {
    // Declare a pointer of type Student. 
    //  using a loop, read each line of the file and store the appropriate data into the appropriate members of the structure pointer.
    
    /* Check if the student ID being read from the file matches the student ID to search. If there
is a match
*/
  //  -  Set a Boolean flag to true to indicate match has been found.
        //bool 
    /* - Display the data corresponding data of the matched student using the following
format:
         - Allocate 30 spaces for the entire name
         - Allocate 15 spaces for the student ID
        - Allocate 5 spaces each for the test score
        - You don’t need to display the number of tests
*/
    // If the Boolean is false i.e. no match is found, display an appropriate message for the use
    //           std::cout << " No match has been found." << endl;   
    
    std::fstream file("students.dat");
    file.close();
}
void exportResults() {
    std::fstream file("student.dat");
    file.close();
}
int findMinimum(int arr[], int size) {
    std::fstream file("student.dat");
    file.close();
}

int main() {
    display();

    return 0;
}
