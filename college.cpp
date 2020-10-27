/*****************************************************************************************
Citlally Gomez
CS211
college.cpp
This file contains all the client functions.
This program will display a menu that the user can enter  
as a student or someone from admissions. 
Before accessing the second menu both the addmissions person /student must 
enter their password. However, the student must enter their id before entering 
their password. After entering the password a second meny will be diaplayed 
that will conatin various choices.Once the user is done they can enter the 
the choice to quit. After they enter that choice the user will be taken back to 
the main menu, where they can decide to end the program be entering choice 3. 
******************************************************************************************/
#include <iostream>
#include <cstdlib> //for atoi() <-- you might not use this
using namespace std;
#include "/cs/slott/cs211/checkInput.h"
#include"student.h"
#include "array.h"
#include<fstream>
const int SIZE = 100; //size of the array
const int START = 100000;//student id starts at 100000
const int PASSWORD = 123; //password for admission's office 
//used to calculate a student's password
const int PRIME1 = 31; 
const int PRIME2 = 5;
int showMenu();
int showAdmMenu();
int showStuMenu();
void addStu(int &count,int& curID,student ar[]);
void showStuInfo(const student&s); 
void allStuInfo(const int& count,const student a[]);      
bool operator==(const student& s, int id);
void removeStu(student ar[],int &count);
//count must be decremented when a student is removed 
//count and the id need to be passed into add stu 
void addStuFromFile(student a[],int& i,int& curid);
void viewStuInfo(const student ar[],int& count);
int verifyStu(const student a[],const int&count);
bool checkPassword(int& password,int& input);
void addCourse(student&s);
void dropCourse(student&s);
void makePayment(student&stu);

int main()
{
  int count = 0; //the number of students in the array
  student ar[SIZE];
  int curID = START; //curID is the student id used for the next student you ar
  int n;//index of the student 
  int input;//the choice entered by the user
  int passAd=123;//the variable that contains the password for addmissions office
  bool entry=false;//determines whether the password was correct   
  int ans;//the user's input for the password
  int passStu;//password for the student 
  int id;//the id of the student  
  do
    {
      //the main menu is displayed and the user choice gets stored into the variable input 
     input=showMenu();
     if(input==1)//the user entered choice 1(admission office)     
	{
	  cout<<"Enter password: ";
	  cin>>ans;//the user password is stored in the in this variable
	  //check password
	  entry=checkPassword(passAd,ans);
	  if(!entry)//the password entered was incorrect
	    cout<<"The password is wrong."<<"\n"<<"\n";
	  //the loop will be executed when the user enters the correct password
	  while(input!=6&&entry)
	    {
	      //the admission menu will be displayed and the user will be asked to enter a choice
	      input=showAdmMenu();
	      //the choice entered will be stored into the variable input 
	      switch(input)
		{
		  cout<<endl;
		case 1:addStu(count,curID,ar);
		  break;
		case 2:addStuFromFile(ar,count,curID);
		  break;
		case 3: removeStu(ar,count);
		  break;
		case 4: viewStuInfo(ar, count); 
		  break;
		case 5:allStuInfo(count,ar);
		  break; 
		}

	    }
	      //once the admission quits the entry is set to false	      
	  entry=false;//this variable now can be used for the student 	
	}
     else if(input==2)//the user entered choice 2(student menu)
	{
	  //check whether the id is in the array 
	  n=verifyStu(ar,count);
	  if(n!=-1)//the id was found
	    {
	      //id is then stored into the variable id
	      id=ar[n].getStuID();
	      //calculate the student password based on the id 
	      passStu= id%(PRIME1*PRIME2);
	      cout<<"Enter password: ";
	      cin>>ans;
	      //the condition is true when the student enters the correct password
	      if(checkPassword(passStu,ans))
		{
		  //the name of the student should display 
		  string f =ar[n].getFirstName();
		  cout<<"Welcome "<<f<<endl;
		  entry=true;
		}
	      else//the password was incorrect 
		cout<<"The password is wrong."<<"\n"<<"\n";
	    } 
	  //the loop will be entered when the password is correct
	  //the while loop will end when the student enters the choice to quit 
	  while(input!=5&&entry)
	    {
	      //student menu is displayed and the student choice is stored into input
	      input=showStuMenu();
	      switch(input)
		{
		case 1:addCourse(ar[n]);
		  break;
		case 2:dropCourse(ar[n]);
		  break;
		case 3:makePayment(ar[n]);
		  break;
		case 4:showStuInfo(ar[n]);        
		  break;
		    }
	    }	 
	}
     //the program will end when the user enters choice 3 from the main menu 
    }while(input!=3);
  return 0;
}

int showMenu()
{
  int choice;
  cout << "========================================================" << endl;
  cout << "1: Admission's Office" << endl;
  cout << "2: Student" << endl;
  cout << "3: End program" << endl;
  cout << "========================================================" << endl;
  cout << "Enter your choice: ";
  choice = getData(1, 3, "\tInvalid choice. Enter only 1, 2 or 3: ");

  return choice;
}


int showStuMenu()
{
  int choice;

  cout << endl;
  cout << "***************************" << endl;
  cout << "1: Add a course" << endl;
  cout << "2: drop a course" << endl;
  cout << "3: Make a payment" << endl;
  cout << "4: View my information" << endl;
  cout << "5: Quit" << endl;
  cout << "***************************" << endl;
  cout << "Enter your choice: ";
  choice = getData(1, 5, "\tInvalid choice. Enter only 1 through 5: ");

  return choice;
}

int showAdmMenu()
{
  int choice;

  cout << endl;
  cout << "***************************" << endl;
  cout << "1: Add a new student" << endl;
  cout << "2: Add new students from a file" << endl;
  cout << "3: drop a student" << endl;
  cout << "4: View one student info" << endl;
  cout << "5: View all students' info" << endl;
  cout << "6: Quit" << endl;
  cout << "***************************" << endl;
  cout << "Enter your choice: ";
  choice = getData(1, 6, "\tInvalid choice. Enter only 1 through 6: ");

  return choice;
}

/********************************************************** 
///Purpose///
The values are in private thus not acessible by the user 
need to call the get functions inorder to see informationn                                                                                                                               
get functions are member functions of the student class
thus need a pointer to use the functions
///Parameters///
student s(s is an object that is part of the student class)
*************************************************************/            
void showStuInfo(const student & s)                                                                                       
{                                                                                                                                            
  cout << "--------------------" << endl;                                                         
 
  cout << "first name: " << s.getFirstName()  << endl;                                                                                                 
  
  cout << "last name: " << s.getLastName()  << endl;                                                                                                  
  
  cout << "gender: " << s.getGender()<< endl;                                                                                                      
  
  cout << "stu id: " << s.getStuID() << endl;                                                                                                      
 
  cout << "num classes: " << s.getNumClasses() << endl;                                                                                                  
 
  cout << "Courses: ";                                                                                                                        
  //i is the index 
  for(int i = 0; i <s.getNumClasses(); i++)  
    cout << s.getClassOf(i)<< ", ";                                                                                                                 
  
    cout << "\nAmount owed $" << s.getBalance() << endl;  
    
    cout << endl;                                                                                                                               
}
/************************************************************
//Purpose//
This function  checks the password for both the admission's 
office and student. Returns true if the correct password 
was entered
//Parameters//
both parameters are inters
This function gets passed the key(the password)and what the
user inputed(input)
**********************************************************/
bool checkPassword(int& password,int& input)
{
  //checks if the user's input was the correct password
  if(password==input)
    return true;//password was correct

  return false;//password was incorrect
}  
/**********************************************************************
//case 3 of student
This function is called when the student wants to make a payement 
The function should get the payment of the user and reduce the 
amount of money the studnet owes
//Parameter//
stu-is an obj part of the student class that is used inorder 
to use the member function reduceFee
***********************************************************************/
void makePayment(student&stu)
{
  int c;//cents 
  int d;//dolars 
  //displaying the amount the student owes
  cout << "You owe $" << stu.getBalance() << endl;  
  cout<<"Enter payment: ";
  cin>>c>>d;//the user enters the amount of dollars and cents 
  stu.reduceFee(c,d);//the balance is adjusted when the user enters a payement 
}
/***************************************
This function verifies that the student 
entered the correct id to access their 
information
//Parameters
student a[]- an array of students 
count-the amount of slotts filled
*************************************/
int verifyStu(const student a[],const int&count)
{
//When the user is a student, select 2 in the top menu. He/she enters hi/her student id.
  int id;//the id entered by the student 
  int num;//the index where the student id was found
  cout<<"Enter the student id: ";
  cin>>id;//the student input is stored in this variable
  //find function is located in the array.h file 
  num=find(a,count,id);
  //The stuent id exists in the array. 
  if(num!=-1)
    return num;
  else//showing an error message when the indez was not found. 
    {
      cout<< "The student ID doesn't exist."<<"\n\n";
     return num;//value should be -1
    }
}
/***************************************************
This function gets the course number from the student  
and displays a message that is dependent on 
what was returned in addCourse. 
This function will add the course if the following is 
true:student enters a crn that is not already in the array,
and their number of classes has not reached the max
s is an obj that is part of the student class
****************************************************/
void  addCourse(student&s)
{
  //case 1 of student
  int num;//index
  int crn;//course number entered by the user
  cout<<"Enter a crn: ";
  cin>>crn;
  //the addCourse function is in the student.cpp
  //thus need a pointer to used the function addCourse 
  num=s.addCourse(crn);
  if(num==1)//course can be added
    {
      cout<<crn<<" has been added successfully" << endl;
      s.addFee(50,14);//increasing the balance 
    }
  else if(num==0)//duplicate
    cout<<crn<<" was not added because you have " << crn << " already." <<  endl;
  else//the array is full
    cout<<crn<<" was not added because your schedule is full." <<  endl;
}
/*****************************************************************************
///Purpose///
This function drops the course of that specific student.If the course does not
exist an error message would be displayed. If course does exist then the course
will be dropped and a message will be diaplayed.
/////Parameters////
student s - s is an obj that is part of the student class
and is used to point to information in the student class 
*******************************************************************************/
void dropCourse(student&s)
{
  bool what;//T-course was removed 
  int c;//course number 
  //case 2 of admission
  cout<<"Enter crn: ";
  cin>>c;//course numbered entered by the student 
  //need to point to the function b/c it's part of the student class 
  what=s.dropCourse(c);
  //if what is true then the course was dropped
  if(!what)//course was not dropped
    {
      cout<<c<<" wasn't dropped from " << s.getFirstName() << " because " << c << " doesn't exist" << endl;
      
    }
  else//course was dropped
    {
      s.reduceFee(50,14);//reducing the amount of money   
      cout<<c<<" has been dropped successfully." << endl;
     
    }
}
/**********************************************************************
/////parameters///////
student ar[]- array of students
count- is the number of slotts
curId-is the identification for each student 
student ar is the array of students 
The array conatins a different student in each slott along 
with that students attributes.(first name, last name,gender,etc).
/////Purpose//////
This function adds the students to the array.This function is called 
when an admissionperson wants to add a student    
*********&&&&&&&&*****************************************************/
void addStu(int& count, int& curID, student ar[])
{
  string fn;//first name
  string ln;//last name
  char gen;//gender
  cout<<endl;
  cout<<"Enter the first name: ";
  cin>>fn;
  cout<<"Enter the last name: ";    
  cin>>ln;
  cout<<"Enter the gender: ";
  cin>>gen;
  cout<<endl;
  //count is the number of students
  //creating an obj with the user's input
  student s(fn,ln,gen,curID);
  //incrementing the count to 
  //add the next student into the array  
  //obj that contains the student info 
  //is being stored into an array  
  ar[count++]=s;
  curID++;//the id of the sudent is incremented
}
/***************************************************
//Case 2 of admission
///Purpose///
This function will adds students in the array 
from an input file.
///Parameters///
student a[]- array of students that conatins a 
different student in each slot along with that 
students attributes.
i- is the index 
curid- is the current id of the student  
***************************************************/
void  addStuFromFile(student a[],int& i,int& curid)
{
  string filename;//variable that contains the file name
  cout<<"Enter the input file name: ";
  cin>>filename;//the filename gets stored in this variable  
  ifstream fin;//creating an obj of the ifstream class
  //open the file name 
  fin.open(filename.c_str());//convert a c++ string(string) to c-string(char) 

  if(!fin)//the input file does not exist 
    {

      cout<<filename<<" doesn't exist"<<endl;
    }
  else
    {
      string f;//first name 
      string l;//last name;
      char g;//gender
      //read the inputs in the array
      fin>>f;
      fin>>l;
      fin>>g;
      //if the information can be read and i is less than the size of the array
      while(fin&&i<SIZE)
	{
	  //increment the id 
	  //an obj is created and it will contain the students attributes
	  student temp(f,l,g,curid);
	  a[i++]=temp;//storing the obj of info into a slot 
	  curid++;//increment the id 	  
	  //read new inputs
     	  fin>>f;
	  fin>>l;
	  fin>>g;
	}
    }
}
/***************************************************************************
///Parameters///
count- is the number of slots that are filled
student ar[]- array of students
(meaning each slot contains one student and their attributes)
///Purpose///
This function removes the student that the admissions wants to remove
If the id enterd was found then the student will be removed 
However, if the student id was not found then an error message should display 
*****************************************************************************/
void removeStu(student ar[],int &count)
{
  string f;//first name
  string l;//last name
  //Case 3 of Admission
  //2 different messages
  int id;//the id of that the user entered 
  int i;//the index of the array 
  cout<<"Student you want to be removed: ";
  cin>>id; //the id entered is being stored in this variable
  i=find(ar,count,id);//index where the id was found 
  if(i==-1)//id is not located in the array 
    {
      cout<<"student with id " << id << " doesn't exist" << endl;
    }
  else
    {     
      //f gets the first name of the student found in a certain index
      //get functions are part of the student class thus need an obj to
      //point to the functions, functions are usable when a pointer is utilized
      f=ar[i].getFirstName();
      l=ar[i].getLastName();
      //this function removes the student with id the addmissions office entered
      remove(ar,count,id);
      cout<< f << " " << l << " has been removed." << endl;
      count--;//the size must be decremented
    }
}
/******************************************
The the student has an obj s and the id (is key) 
This function compares the actual value
to the inputed value.
This operator gets called in the find function
(located in array.cpp) 
where the a[i]==key
         (student id)
******************************************/
bool operator==(const student& s, int id )
{
  //checks the id entered will the id located in the student array 
  if(s.getStuID()==id)
    return true;//id was found

  return false;//id was not found
}
/**********************************************************************
///Parameters///
student ar[]- array of students(meaning each slot contains one student 
and their attributes)
count-the number of slotts filled
///Purpose///
This function ask for the student id and then displays the information 
of that student.If the id is not found in the array then an error 
message will be displayed
***********************************************************************/
void viewStuInfo(const student ar[],int& count)
{
//Case 4 of admission
//Ask for the student id
  int index;//index of where the id was found
  int id;//the id entered 
  cout<<"Enter the student id: ";
  cin>>id;
  index=find(ar,count,id);
  if(index!=-1)//the student was found 
    //If it exists, showStuInfo is called to show the student  
    showStuInfo(ar[index]);
  else
    //If the student doesn't exist, an error message will be displayed.
    cout<<"The student ID does not exist."<<endl; 
}
/***********************************************
//Case 5 of Admission
///Parameters///
count-number of slotts filled
student ar- array of students
The number of slotts filled is not being changed 
(it's being printed out) thus it needs to be const 
The array is also is not being 
changed thus its const. 
///Purpose///
This function will display all the students in the 
array. 
************************************************/
void allStuInfo(const int& count,const student a[])
{
  //the for loop will go through the array 
  for(int i=0; i<count; i++)
    {
      //Call showStuInfo()
      showStuInfo(a[i]);
    }

}





