//Hanre WPJ Delport (50605216)
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>

using namespace std;
//Definition of the course class used to make different course objects
class Course
{
//Defining the course class's attributes
public:
    string name;
    string description;
    string courseID;
    double weights[6];

    //Constructer method used to create an object with no parameters
    Course()
    {
        name = "";
        description = "";
    }

    //Constructer method used to create an object with some parameters
    Course (string courseName, string courseDescription, double newWeights[])
    {
        name = courseName;
        description = courseDescription;
        for (int i = 0; i<6; i++)
        {
            weights[i] = newWeights[i];
        }
    }
};

//Defining a user class that will be used to create student and lecturer objects
class User
{
//Declaring user attributes
public :
    string name;
    string surname;
    int userID;
    Course studentsCourse;
    double marks[6];
    bool finished;

    //Construccter method with no parameters used to create arrays of user objects
    User ()
    {
        name = "";
        surname = "";
    }

    //Constructer method used to save user's name and surname
    User(string newName, string newSurname)
    {
        name = newName;
        surname = newSurname;
    }

    //Method to generate a random, unique 6-digit ID for the user
    int GenerateID()
    {
        srand(time(NULL));
        return (100000 + rand() % 900000);
    }

    //Method to calculate a student's average mark and to return it to main program
    double CalcAverage()
    {
        double avg = 0;

        //Run through the array of marks and multiply it with the corresponding array  of weights
        for (int i = 0; i<6; i++)
        {
            avg += (marks[i]/100)*(studentsCourse.weights[i]/100)*100;
        }

        return avg;
    }

    //Method used by lecturers to add new courses to textfiles. Uses a reference variable to update num of courses in the main program
    void AddCourse(Course arrCourses[], int &numCourses)
{
    //Receive the course's name and descriptionj from lecturer and save in temporary course object
    Course tempCourse;
    cout << "Enter a name for the new course: ";
    cin.ignore();
    getline(cin,tempCourse.name);
    cout << "Enter a short description of the course: ";
    cin.ignore();
    getline(cin, tempCourse.description);

    //Checks if the course ID entered by user is unique
    bool uniqueID;
    do
    {
        uniqueID = true;
        cout << "Enter a 3- or 4- letter, unique ID for the course: ";
        cin >> tempCourse.courseID;
        for (int i=0; i<numCourses; i++)
        {
            if (tempCourse.courseID == arrCourses[i].courseID)
            {
                uniqueID = false;
                break;
            }
        }
    }
    while(!uniqueID);

    //Receive the weights of each assessment, saves it to the course object's array of weights and checks that it adds up to 100
    cout << "Please enter the weights that each assessment will contribute towards the final 100% of the course (Enter 0 to stop): "<<endl;
    double total;
    do
    {
        total = 0;
        cout <<"Make sure that the weights count up to 100."<<endl;
        for (int i =0; i<6; i++)
        {
            cout <<"Enter weight of assessment "<<i+1<<" : ";
            cin >> tempCourse.weights[i];
            total += tempCourse.weights[i];
            if (tempCourse.weights[i]==0)
            {
                for (int j= (i!=5) ? (i+1):(i); j<6; j++)
                    tempCourse.weights[j]=0;
                break;
            }
        }
    }while(total!=100);

    //Create an ofstream file variable and open the file in append mode, then checks if file could be opened.
    ofstream outfile;
    outfile.open("Courses.txt", ios::app);
    if (outfile.is_open())
    {
        //Add the new course's info to the txtfile in correct format
        outfile<<endl;
        outfile<<tempCourse.name<<"#"<<tempCourse.description<<"#"<<tempCourse.courseID<<endl;
        for (int i=0; i<6; i++)
        {
            outfile<<tempCourse.weights[i]<<" ";
        }
        outfile.close();
    }
    //If file couldnt open
    else
    {
        cout<< "Ooops!! File couldn't be found, please try again!"<<endl;
        exit(1);
    }

    //Add the new course to the array of existing courses
    arrCourses[numCourses] = tempCourse;
    numCourses++;
    cout<<"New course successfully added!!"<<endl;
}

    //Method to calculate and display statistics of a specific course
    void ShowStats(User arrStudents[], Course course, int numStudents)
{
    //Initialize an array with enough space for all students and a counter variable
    size_t maxSize = numStudents;
    double averages[maxSize];
    int studentsWithCourse=0;

    //Loop counts all the students that have this course, calculate their average and saves it to the array of averages
    for (int i = 0; i<numStudents; i++)
    {
        if (arrStudents[i].studentsCourse.courseID == course.courseID)
        {
            averages[studentsWithCourse] = arrStudents[i].CalcAverage();
            studentsWithCourse++;
        }
    }

    //Sort the array of averages from smallest to largest average
    double temp;
    for (int i = 0; i<studentsWithCourse; i++)
    {
        for (int j=0; j<studentsWithCourse-i-1; j++)
        {
            if(averages[j]>averages[j+1])
            {
                temp = averages[j];
                averages[j] = averages[j+1];
                averages[j+1] = temp;
            }
        }
    }

    //Display all the averages from smallest to largest
    int studentPos=0;
    double total=0, totalSquared;
    cout << "Averages for """<<course.name<<""" course from lowest to highest." <<endl;
    for(int i=0; i<studentsWithCourse; i++)
    {
        total += averages[i];
        totalSquared += pow(averages[i],2);
        cout <<i+1<<". "<<fixed<<setprecision(2)<<averages[i]<<"%"<<endl;
    }
    //Calculate and display different statistics for a lecturer to interperet
    double courseAverage = total/studentsWithCourse;
    double sDeviation = sqrt((1.00/(studentsWithCourse-1))*(totalSquared-(studentsWithCourse*(pow(courseAverage,2.00)))));
    double minimum = averages[0];
    int pos = trunc((studentsWithCourse+1)*1/4);
    double Q1 =averages[pos];
    pos = trunc((studentsWithCourse+1)*2/4);
    double Q2 = averages[pos];
    pos=trunc((studentsWithCourse+1)*3/4);
    double Q3 = averages[pos];
    double maximum = averages[studentsWithCourse-1];
    cout << "Course average: "<<fixed<<setprecision(2)<<courseAverage<<"% (with a standard deviation of "<<fixed<<setprecision(2)<<sDeviation<<")"<<endl;
    cout << "\nThe 5-point summary of the averages for the course is as follows:"<<endl;
    cout << left<<setw(10)<<"MIN:"<<fixed<<setprecision(2)<< minimum<<endl;
    cout << left<<setw(10)<<"Q1:"<<fixed<<setprecision(2)<< Q1<<endl;
    cout << left<<setw(10)<<"Q2:"<<fixed<<setprecision(2)<< Q2<<endl;
    cout << left<<setw(10)<<"Q3:"<<fixed<<setprecision(2)<< Q3<<endl;
    cout << left<<setw(10)<<"MAX:"<<fixed<<setprecision(2)<< maximum<<endl;
}

//Function to save the data of the users' arrays to the textfiles when any data changes.
void SaveUsers(User arrLecturers[], User arrStudents[], int numLecturers, int numStudents)
{
    //Open the users txtfile and cllear the data
    ofstream outfile;
    outfile.open("Users.txt");
    //Checks if file opened
    if (outfile.is_open())
    {
        //Loop runs through array of lecturers and save their data to the file
        for (int i =0; i<numLecturers; i++)
        {
            outfile<< arrLecturers[i].name<<" "<<arrLecturers[i].surname<<" "<<arrLecturers[i].userID<<endl;
        }
        //Loop runs throuh array of students and write their data to the file
        for (int i =0; i<numStudents; i++)
        {
            outfile<< arrStudents[i].name<<" "<<arrStudents[i].surname<<" "<<arrStudents[i].userID<<" "<<arrStudents[i].studentsCourse.courseID<<" ";
            for (int j=0; j<6; j++)
            {
                outfile << arrStudents[i].marks[j]<<" ";
            }
            //If loop reaches the last student, the last element should be written to the file without any new-line characters
            if (i != (numStudents-1))
            {
                outfile<<(arrStudents[i].finished? (1):(0))<<endl;
            }
            else
            {
                outfile<<(arrStudents[i].finished? (1):(0));
            }

        }
        //Close txtfile
        outfile.close();
        cout << "Changes successfully saved!"<<endl;
    }
    //If file couldnt open
    else
    {
        cout << "Oooops!!Users file couldn't be found."<<endl;
        exit(1);
    }
}

//Function that lecturers will use to change a student's marks
void EditMarks(User &student)
{
    //Displays student's current marks
    cout<<student.name<<"'s current marks:\n";
    int i;
    for (i=0; i<6; i++)
    {
        if (student.studentsCourse.weights[i] == 0)
        {
            break;
        }
        else
        {
            cout << "Assesssment "<<i+1<<" : "<<fixed<<setprecision(2)<<student.marks[i]<<"%"<<endl;
        }
    }

    //Ask user which assessment needs to be marked, then asks for the new mark and checks if the mark is valid. Keeps on processing until user enters a 0
    double tempMark;
    int pos;
    do
    {
        cout << "What assessment's mark do you want to edit (between 1 and "<<i<<" - Enter 0 to exit): ";
        cin >> pos;

        if (pos>=1 && pos<=i)
        {
            do
            {
                cout<<"Enter the student's mark for assessment "<<pos<<" in % : ";
                cin >> tempMark ;
            }
            while (tempMark<=0 || tempMark>=100);
            student.marks[pos-1] = tempMark;
        }
        else
        {
            cout << "Please enter a valid number between 1 and "<<i<<endl;
        }
    }
    while(pos!=0);
    //Displays confirmaation message to the user
    cout <<"Marks updated!!"<<endl;

    //Displays the student's new marks
    cout<<student.name<<"'s new marks:\n";
    for (i=0; i<6; i++)
    {
        if (student.studentsCourse.weights[i] == 0)
        {
            break;
        }
        else
        {
            cout << "Assesssment "<<i+1<<" : "<<fixed<<setprecision(2)<<student.marks[i]<<"%"<<endl;
        }
    }

    //Checks if student has marks for all the assessments that count according to the weights attribute of the course
    bool finished =true;
    for (i=0; i<6; i++)
    {
        if (student.studentsCourse.weights[i] == 0)
        {
            break;
        }
        else
        {
            if(student.marks[i]==0)
            {
                finished = false;
                break;
            }
        }
    }
    //Update the finished attribute of the student in the student's array
    if (finished)
    {
        student.finished = true;
    }

}
};

//Function that reads the users textfile and saves the data to the corresponding arrays
void populateUserArrays(User arrStudents[], User arrLecturers[],Course arrCourses[],int numCourses,int &numStudents, int &numLecturers)
{
    //Crestes file variable, opens file and check if file could open
    ifstream infile("Users.txt");
    if(infile.is_open())
    {
        //Initialize variables
        string tempName, tempSurname ;
        int tempUserID;

        //Loop runs through the users textfile
        numStudents = 0, numLecturers = 0;
        while (!infile.eof())
        {
            infile >>tempName>>tempSurname>>tempUserID;
            //Lecturers' user IDs are uneven, as a way to distinguish them from students
            if (tempUserID%2 != 0)
            {
                //Save data from txtfile to array
                numLecturers++;
                arrLecturers[numLecturers-1].name = tempName;
                arrLecturers[numLecturers-1].surname = tempSurname;
                arrLecturers[numLecturers-1].userID = tempUserID;
            }
            else
            {
                //Save data from txtfile to array
                numStudents++;
                arrStudents[numStudents-1].name = tempName;
                arrStudents[numStudents-1].surname = tempSurname;
                arrStudents[numStudents-1].userID = tempUserID;
                //Get the other data that only applies to students from the file and saves tit to the students array
                infile >> arrStudents[numStudents-1].studentsCourse.courseID>>arrStudents[numStudents-1].marks[0]>>arrStudents[numStudents-1].marks[1]>>arrStudents[numStudents-1].marks[2]>>arrStudents[numStudents-1].marks[3]>>arrStudents[numStudents-1].marks[4]>>arrStudents[numStudents-1].marks[5]>>arrStudents[numStudents-1].finished;

                for (int i=0; i<numCourses; i++)
                {
                    if (arrCourses[i].courseID == arrStudents[numStudents-1].studentsCourse.courseID)
                    {
                        arrStudents[numStudents-1].studentsCourse = arrCourses[i];
                        break;
                    }
                }
            }
        }
        //Close txtfile
        infile.close();

    }
    else
    {
        cout << "User file could'nt be found. Please try again!"<<endl;
        exit(1);
    }

}

//Function that reads the courses textfile and saves the data to the corresponding arrays
void populateCourseArrays(Course arrCourses[],int &numCourses)
{
    //Open txtfile and checks if it is opened successfully
    ifstream infile("Courses.txt");
    if(infile.is_open())
    {
        string line;
        int posSpace,pos1,pos2;
        //Loop runs to end of the file
        numCourses = 0;
        while (!infile.eof())
        {
            //Save the name, description and ID of the course by using the position of the "#"-symbols
            infile >> arrCourses[numCourses].name;
            getline(infile,line);
            posSpace = line.find(" ");
            pos1 = line.find("#");
            arrCourses[numCourses].name += line.substr(posSpace,pos1);
            pos2 = line.find("#",pos1+1);
            arrCourses[numCourses].description = line.substr(pos1+1,pos2-pos1-1);
            arrCourses[numCourses].courseID = line.substr(pos2+1);
            //Use loop to get the weights of the assessments and save it to the array of courses
            for (int i=0; i<6; i++)
            {
                infile >> arrCourses[numCourses].weights[i];
            }
            numCourses+=1;
        }
        infile.close();

    }
    else
    {
        cout << "Courses file could'nt be found. Please try again!"<<endl;
        exit(1);
    }

}

//Function to search for a specific user and returns the position of the user in the array
int searchUser(User arrUsers[], int numUsers)
{
    //Receive ID
    int userID;
    cout << "Please enter a unique user ID: ";
    cin >> userID ;
    //Runs through array of users, searching for ID and saving position of ID when its found
    for (int i=0; i<numUsers; i++)
    {
        if (arrUsers[i].userID == userID)
        {
                return i;
        }
    }
    //Return -1 when id not found
    return -1;
}

//Function that runs through all the courses in courses array and display the data of each course
void DisplayCourses(Course arrCourses[], int numCourses)
{
    cout<<"All the courses Skill-Rise offer: "<<endl;
    for(int i = 0; i<numCourses; i++)
    {
        cout <<i+1<<". "<<arrCourses[i].name<<" ("<<arrCourses[i].courseID<<")"<<endl;
        cout <<arrCourses[i].description<<endl;
        cout<<endl;
    }
}

//Function that asks user to choose a course and return an object of the course to main program
Course ChooseCourse(Course arrCourses[], int numCourses)
{
    //Loop runs while user enters invalid numbers
    bool validCourse;
    int CourseNum;
    DisplayCourses(arrCourses, numCourses);
    do
    {
        validCourse = true;
        cout <<"Please choose a course (1- "<< numCourses <<"): ";
        cin >> CourseNum;
    }
    while (!(CourseNum>=1 && CourseNum<=numCourses));


    return (arrCourses[CourseNum-1]);
}

//Function used to register a new student and save their info to the users txtfile
void RegisterStudent(User arrUsers[], Course arrCourses[],int &numUsers, int numCourses)
{
    //Receive name and surname of user, then creates temporary user object
    string name, surname;
    cout << "Please enter your name: ";
    cin >> name;

    cout << "Please enter your surname: ";
    cin >> surname;

    User tempStudent(name, surname);

    //do while loop generating user id until it's a unique, even ID
    bool uniqueID;
    int tempID;
    do
    {
        uniqueID = true;
        tempID = tempStudent.GenerateID();
        for (int i =0; i<numUsers; i++)
        {
            if (arrUsers[i].userID == tempID)
            {
                uniqueID = false;
                break;
            }
        }
    }
    while (!uniqueID || tempID%2 != 0);
    //Save the users ID and  chosen course to temporary object
    tempStudent.userID = tempID;
    tempStudent.studentsCourse = ChooseCourse(arrCourses,numCourses);
    tempStudent.finished = false;

    //Initialize the marks of student to 0
    for (int i = 0; i<6; i++)
    {
        tempStudent.marks[i]=0.00;
    }

    //Save the temp object to array of users
    arrUsers[numUsers] = tempStudent;

    //Save the new user's data to textfile
    ofstream outfile;
    outfile.open("Users.txt", ios::app);
    if (outfile.is_open())
    {
        outfile<<"\n"<<arrUsers[numUsers].name<<" "<<arrUsers[numUsers].surname<<" "<<arrUsers[numUsers].userID<<" "<<arrUsers[numUsers].studentsCourse.courseID<<" " ;
        for (int i = 0; i<6; i++)
        {
            outfile<<arrUsers[numUsers].marks[i]<<" ";
        }
        outfile<<arrUsers[numUsers].finished;
        outfile.close();
    }
    //Increment number of users
    numUsers++;
}

//Function that neatly displays the student's marks
void DisplayMarks(Course myCourse, User student)
{
    cout << "You are currently enrolled for "<<myCourse.name<<endl;
    cout << "Assessments:"<<endl;
    //For loop calculates how many assessments the course has and displays how much each assessment count towards final mark
    int numAssessments;
    for (int i = 0; i<6; i++)
    {
        if (myCourse.weights[i]!=0)
        {
            cout <<"Assessment "<<i+1 <<" counts "<<myCourse.weights[i]<<"% towards the final mark"<<endl;
            numAssessments = i+1;
        }
        else
        {
            break;
        }
    }

    if (student.finished)
    {
        cout << "\nCongrats, you have finished the course!!"<<endl;
    }
    else
    {
        cout << "\nYou have not completed all the assessments."<<endl;
    }

    //Display student's marks for all the assessments
    cout<<"\nYour marks for each assessment: "<<endl;
    cout << "num assessments "<<numAssessments<<endl;
    for (int i = 0; i<numAssessments; i++)
    {
        cout <<"Assessment "<<i+1 <<" : "<<student.marks[i]<<"%"<<endl;
    }
    //Displays average
    cout <<"Your average for "<<myCourse.courseID<<" is: "<<fixed<<setprecision(2)<<student.CalcAverage()<<"%"<<endl;
}

//Function displays the statistics of a student
void ShowStats(User arrStudents[], User Student, int numStudents)
{
    //Initialize array of averages
    size_t maxSize = numStudents;
    double averages[maxSize];
    int studentsWithCourse=0;

    //Calcs how many students have the course and fills the array
    for (int i = 0; i<numStudents; i++)
    {
        if (arrStudents[i].studentsCourse.courseID == Student.studentsCourse.courseID)
        {
            averages[studentsWithCourse] = arrStudents[i].CalcAverage();
            studentsWithCourse++;
        }
    }

    //Sorts the array from lowest to highest average
    double temp;
    for (int i = 0; i<studentsWithCourse; i++)
    {
        for (int j=0; j<studentsWithCourse-i-1; j++)
        {
            if(averages[j]>averages[j+1])
            {
                temp = averages[j];
                averages[j] = averages[j+1];
                averages[j+1] = temp;
            }
        }
    }

    //Display the sorted array of averages and gets position of students average
    int studentPos=0;
    double total=0, totalSquared=0;
    cout << "Averages for """<<Student.studentsCourse.name<<""" course from lowest to highest." <<endl;
    for(int i=0; i<studentsWithCourse; i++)
    {
        total += averages[i];
        totalSquared += pow(averages[i],2);
        cout <<i+1<<". "<<fixed<<setprecision(2)<<averages[i]<<"%"<<endl;
        if (Student.CalcAverage() == averages[i])
        {
            studentPos = i+1;
        }
    }

    //Calculate and display different statistics
    double courseAverage = total/studentsWithCourse;

    double sDeviation = sqrt((1.00/(studentsWithCourse-1))*(totalSquared-(studentsWithCourse*(pow(courseAverage,2.00)))));
    double minimum = averages[0];
    int pos = trunc((studentsWithCourse+1)*1/4);
    double Q1 =averages[pos];
    pos = trunc((studentsWithCourse+1)*2/4);
    double Q2 = averages[pos];
    pos=trunc((studentsWithCourse+1)*3/4);
    double Q3 = averages[pos];
    double maximum = averages[studentsWithCourse-1];
    cout << "Course average: "<<fixed<<setprecision(2)<<courseAverage<<"% (with a standard deviation of "<<fixed<<setprecision(2)<<sDeviation<<")"<<endl;
    cout << "\nThe 5-point summary of the averages for the course is as follows:"<<endl;
    cout << left<<setw(10)<<"MIN:"<<fixed<<setprecision(2)<< minimum<<endl;
    cout << left<<setw(10)<<"Q1:"<<fixed<<setprecision(2)<< Q1<<endl;
    cout << left<<setw(10)<<"Q2:"<<fixed<<setprecision(2)<< Q2<<endl;
    cout << left<<setw(10)<<"Q3:"<<fixed<<setprecision(2)<< Q3<<endl;
    cout << left<<setw(10)<<"MAX:"<<fixed<<setprecision(2)<< maximum<<endl;
    cout << "Your average: "<<fixed<<setprecision(2)<<Student.CalcAverage()<<"%"<<endl;
    cout << "You rank "<<studentsWithCourse-studentPos+1<<" out of "<<studentsWithCourse<<" students."<<endl;
}

int main()
{
    //Create and fill arrays of users and courses
    const int MAX_STUDENTS = 60;
    const int MAX_LECTURERS = 10;
    const int MAX_COURSES = 10;

    Course arrCourses[MAX_COURSES];
    int numCourses=0;
    populateCourseArrays(arrCourses, numCourses);

    User arrStudents[MAX_STUDENTS];
    User arrLecturers[MAX_LECTURERS];
    int numStudents=0, numLecturers =0;
    populateUserArrays(arrStudents, arrLecturers,arrCourses,numCourses,numStudents,numLecturers);

    int choiceMain;
    //Loop runs while user doesnt exit
    do
    {
        //Display menu
        cout << "WELCOME TO SKILL-RISE"<<endl;
        cout << "1. Login/Register as a Student"<<endl;
        cout << "2. Login as a Lecturer"<<endl;
        cout << "3. Exit"<<endl;
        cout << "Please choose an option above (1-3): ";
        cin >> choiceMain ;

        int userID=0;
        switch(choiceMain)
        {
            case 1:
            {
                //Tests if user is registered or not
                int studentPos = searchUser(arrStudents, numStudents);
                //asks user to register if not yet registered
                if (studentPos == -1)
                {
                    string reg;
                    do
                    {
                        cout << "You are currently not regisered, would you like to register (Y/N) : ";
                        cin >>reg;
                    }
                    while(!( toupper(reg[0])=='Y'  || toupper(reg[0])=='N'));

                    if (toupper(reg[0])=='Y')
                    {
                        RegisterStudent(arrStudents,arrCourses,numStudents,numCourses);
                        cout << "\nCongratulations "<<arrStudents[numStudents-1].name<<"!!You are now a registered student at SkillRise!!"<<endl;
                        cout <<"Your unique student ID that you will need to login is: "<<arrStudents[numStudents-1].userID<<endl;
                    }
                }
                //If student is registered
                else
                {
                    User currentStudent;
                    currentStudent = arrStudents[studentPos];
                    cout<<"\nWelcome back "<<currentStudent.name<<"!!\n"<<endl;
                    //Display student's menu
                    int choiceStudent;
                    //loop runs until user exits
                    do
                    {
                        cout << "1. View your marks"<<endl;
                        cout << "2. View statistics"<<endl;
                        cout << "3. Return to main menu"<<endl;
                        cout << "Please choose an option above (1-3): ";
                        cin >> choiceStudent ;

                        switch(choiceStudent)
                        {
                        case 1:
                            cout <<endl;
                            DisplayMarks(currentStudent.studentsCourse, currentStudent);
                            cout<<endl;
                            break;

                        case 2:
                            cout << endl;
                            ShowStats(arrStudents,currentStudent,numStudents);
                            cout<<endl;
                            break;

                        case 3:
                            cout<<"Returning...\n"<<endl;
                            break;

                        default:
                            cout << "Please enter a valid number between 1 and 3\n"<<endl;
                            break;
                        }

                    }
                    while (choiceStudent != 3);
                }
                break;
            }

            case 2:
            {
                //Checks if user is registered as lecturer
                int lectPos = searchUser(arrLecturers, numLecturers);
                if (lectPos == -1)
                {
                    cout << "You are unfortunately not registered as a lecturer, please go see the IT consultant to get yourself registered."<<endl;
                }
                else
                {
                    //creates temp lecturer object
                    User currentLect;
                    currentLect = arrLecturers[lectPos];
                    cout<<"\nWelcome back "<<currentLect.name<<"!!"<<endl;

                    int choiceLect;
                    //Loop runs until user returns to main
                    do
                    {
                        //display lecturer menu
                        cout << "1. Add course"<<endl;
                        cout << "2. Edit student marks"<<endl;
                        cout << "3. View statistics"<<endl;
                        cout << "4. Return to main menu"<<endl;
                        cout << "Please choose an option above (1-4): ";
                        cin >> choiceLect ;

                        switch(choiceLect)
                        {
                        case 1:
                            cout <<endl;
                            currentLect.AddCourse(arrCourses, numCourses);
                            cout<<endl;
                            break;

                        case 2:
                        {
                            cout << endl;
                            cout << "Who's marks do you need to change?" <<endl;
                            //Call different functions to get pos of student, change student's marks and to save the new data to files
                            int pos = searchUser(arrStudents,numStudents);
                            currentLect.EditMarks(arrStudents[pos]);
                            currentLect.SaveUsers(arrLecturers, arrStudents,numLecturers,numStudents);
                            cout<<endl;
                            break;
                        }
                        case 3:
                        {
                            cout <<endl;
                            cout<<"Which course's statistics do you want to see?\n"<<endl;
                            //Create temp course object and shows stats for the course
                            Course chosenCourse = ChooseCourse(arrCourses, numCourses);
                            currentLect.ShowStats(arrStudents,chosenCourse,numStudents);
                            break;
                        }
                        case 4:
                            cout<<"Returning...\n"<<endl;
                            break;

                        default:
                            cout << "Please enter a valid number between 1 and 4\n"<<endl;
                            break;
                        }

                    }
                    while (choiceLect != 4);
                }
                break;

            }
            case 3:
                //Exits progam
                cout << "Thank you for using SkillRise!\n"<<endl;
                break;

            default:
                cout << "Please enter a valid number between 1 and 3\n"<<endl;
                break;
        }

    }
    while (choiceMain != 3);


    return 0;

}

 /*
    for (int i = 0; i<numLecturers; i++)
    {
        cout << arrLecturers[i].name<<"  "<<arrLecturers[i].surname<<"  "<<arrLecturers[i].userID<<endl;
    }


    for (int i = 0; i<numStudents; i++)
    {
        cout << arrStudents[i].name<<"  "<<arrStudents[i].surname<<"  "<<arrStudents[i].userID<<"  "<<arrStudents[i].studentsCourse.courseID<<"  "<<arrStudents[i].marks[0]<<"  "<<arrStudents[i].marks[1]<<"  "<<arrStudents[i].marks[2]<<"  "<<arrStudents[i].marks[5]<<"  "<<arrStudents[i].finished<<endl;
    }
*/
