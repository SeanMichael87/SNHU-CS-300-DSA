//============================================================================
// Name        : CoursePlanner.cpp
// Author      : Sean Bruyere
// Version     : 1.0
// Description : Class Planner with Vector Data structure
//============================================================================

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

using namespace std;

struct Course {
    string courseNumber; // unique identifier
    string courseName;
    vector<string> prerequisites;
};

//Opens and reads each line of file - stores in a string vector
vector<string> courseFile (string file) {
    ifstream newFile (file, ifstream::in);
    vector<string> readLine;
    if (newFile.is_open()) {

        for (string line; getline(newFile, line);) {
            readLine.push_back(line);
        }

    } else {
        cout << "File is not open" << endl;
    }

    newFile.close();

    return readLine;
}

//Reads each line from vector and sets each value to necessary variable to create the course
vector<Course> createCourse(const vector<string>& readLine) {
    vector<Course> courses;
    Course course;
    string token;
    int count = 0;

    for (const string& line: readLine) {
        stringstream ss(line);
        count = 0;

        while (ss.good()) {
            getline(ss, token, ',');

            if (count == 0) {
                course.courseNumber = token;
                count++;
            } else if (count == 1) {
                course.courseName = token;
                count++;
            } else {
                course.prerequisites.push_back(token);
                count++;
            }
        }

        if (count < 2) {
            cout << "Error Found: Need Course Name and Number" << endl;
        }

        ss.clear();
        ss.str(string());

        courses.push_back(course);
        course.prerequisites.clear();
    }

    return courses;
}

int partition(vector<Course>& course, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;

    // adjusts middle of partition accordingly
    int pivot = (low + (high-low) / 2);
    bool finished = false;

    while(!finished) {

        // keep incrementing low index while bids[low] < bids[pivot]
        while(course.at(low).courseNumber < course.at(pivot).courseNumber){
            ++low;
        }
        // keep decrementing high index while bids[pivot] < bids[high]
        while(course.at(pivot).courseNumber < course.at(high).courseNumber){
            --high;
        }

        if (low >= high) {
            //Zero or one element remains exit while loop
            finished = true;
        } else {
            //swap values to sort
            swap(course.at(low), course.at(high));

            //update low index and high index
            ++low;
            --high;
        }

    }
    //returns high index for quickSort recursion
    return high;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 */
void quickSort(vector<Course>& course, int begin, int end) {
    unsigned mid;

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }

    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    mid = partition(course, begin, end);
    // recursively sort lower partition (begin to pivot value returned by partition)
    quickSort(course, begin, mid);
    // recursively sort higher partition (pivot value + 1 to end)
    quickSort(course, mid + 1, end);
}

//Display course method to limit redundancies
void displayCourses(Course course) {
    cout << course.courseNumber << ": " << course.courseName << endl;
}

//Overloaded display course to include a for loop for each available course
void displayCourses(const vector<Course>& courses) {
    cout << "           Sample Schedule" << endl;
    cout << "----------------------------------------" << endl;
    for (const Course& course: courses) {
        displayCourses(course);
    }
}

//Search and print course file
void searchCourse(const vector<Course>& courses, string userCourseNumber) {
    //flag is used for validation - switched to true if item is found
    bool flag = false;
    for (Course course: courses) {
        if (course.courseNumber == userCourseNumber) {
            displayCourses(course);
            cout << "Prerequisites: ";
            for (string prereq: course.prerequisites) {
                cout << " " << prereq << " |";
            }
            cout << endl;
            flag = true;
            break;
        }
    }

    if (!flag) {
        cout << "Course number was not found" << endl;
    }
    cout << endl;
}

int main() {
    vector<Course> courses;
    vector<string> course = courseFile("/Users/seanbruyere/CLionProjects/ProjectTwo/classList.txt");
    string userInput;

    int choice = 0;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        //Should create mismatch exception for cin
        //when strings entered, infinite loop is the result.
        cin >> choice;

        switch (choice) {

            case 1:
               courses = createCourse(course);
               break;

            case 2:
                quickSort(courses, 0, courses.size() - 1);
                displayCourses(courses);
                break;

            case 3:
                cout << "What course do you want to know about?";
                cin >> userInput;
                transform(userInput.begin(), userInput.end(), userInput.begin(), ::toupper);
                searchCourse(courses, userInput);
                break;

            default:
                if (choice != 4) {
                    cout << choice << " is invalid try again" << endl;
                }
                break;
        }
    }

    cout << "Thank you for using the Course Planner!" << endl;

    return 0;
}
