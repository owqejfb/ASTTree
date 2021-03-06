// COP3530Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <ctype.h>
#include <stack>
#include <cstdlib>
#include<algorithm> 
#include <vector>
using namespace std;

class Student {

public:
    //name
    string studentName;
    
    //GatorID
    string gatorID; 
    //Using string instead of integer to display zeros. (We want 00000030, not 30)
    
    //unique ID
    int uniqueID = 0;
    //using an int to make it easier to compare later on

    int height = 0;

    //leftPointer
    Student* leftChildPointer = nullptr;
    
    //rightPointer
    Student* rightChildPointer = nullptr;

public:
    //constructor
    Student* createStudent(string studentName, string gatorID);
    
    //insertion
    Student* insertStudentNode(Student* rootPointer, string studentName, string gatorID);
    
    //deletion
    Student* removeID(Student* rootPointer, string gatorID);
    Student* removeInorder(Student* rootPointer, int NthStudent);

    //search
    void searchID(Student* rootPointer, string gatorID);
    void searchNAME(Student* rootPointer, string studentName);

    //print functions
    void printInOrder(Student* rootPointer);
    void printPreOrder(Student* rootPointer);
    void printPostOrder(Student* rootPointer);
    void printLevelCount(Student* rootPointer);

    //helper functions
    int heightOfTree(Student* rootPointer);
    
    int getBalanceFactor(Student* rootPointer);

    Student* leftRotation(Student* rotatingNode);
    Student* rightRotation(Student* rotatingNode);

    Student* minValueOfTree(Student* rootPointer);

    int howManyStudentsInTree(Student* rootPointer);
};

Student* Student::createStudent(string studentName, string gatorID) {

    //make new student

    Student* newStudent = new Student();
    
    newStudent->studentName = studentName;
    newStudent->gatorID = gatorID;
    newStudent->uniqueID = stoi(gatorID);

    newStudent->leftChildPointer = nullptr;
    newStudent->rightChildPointer = nullptr;

    return newStudent;
}

Student* Student::insertStudentNode(Student* rootPointer, string studentName, string gatorID) {

    //these statements check to see if parameters are valid
    for (unsigned int i = 0; i < studentName.length(); i++) {
        if (!isalpha(studentName.at(i)) && !isblank(studentName.at(i))) {
            cout << "unsuccesful\n";
            return nullptr;
        }
    }

    if (gatorID.length() != 8) {
        cout << "unsuccessful\n";
        return nullptr;
    }

    for (unsigned int i = 0; i < gatorID.length(); i++) {
        if (!isdigit(gatorID.at(i))) { 
            cout << "unsuccessful\n";
            return nullptr;
        }
    }

    int tempUniqueID = stoi(gatorID);

    if (rootPointer == nullptr) {
        //creates first student
        rootPointer = createStudent(studentName, gatorID);
        cout << "successful\n";
    }
    //else if statements used to find the place to put new entry
    else if (tempUniqueID < rootPointer->uniqueID) {
        rootPointer->leftChildPointer = insertStudentNode(rootPointer->leftChildPointer, studentName, gatorID);

    }
    else if (tempUniqueID > rootPointer->uniqueID) {
        rootPointer->rightChildPointer = insertStudentNode(rootPointer->rightChildPointer, studentName, gatorID);

    }
    else {
        //this activates if there is a repeated ID
        return rootPointer;
    }

    //declaring the height of the newest addition
    rootPointer->height = 1 + max(heightOfTree(rootPointer->leftChildPointer),heightOfTree(rootPointer->rightChildPointer));

    //get balanceFactor of each student to determine if it needs to be rotated
    int balanceFactor = getBalanceFactor(rootPointer);

    //Rotations
    
    //LL = right rotation
    if (balanceFactor > 1 && tempUniqueID < rootPointer->leftChildPointer->uniqueID){
        
        return rightRotation(rootPointer);
    }
    
    //RR = left rotation
    if (balanceFactor < -1 && tempUniqueID > rootPointer->rightChildPointer->uniqueID){
        
        return leftRotation(rootPointer);
    }
    
    //LR = left right rotation
    if (balanceFactor > 1 && tempUniqueID > rootPointer->leftChildPointer->uniqueID){
        
        rootPointer->leftChildPointer = leftRotation(rootPointer->leftChildPointer);
        return rightRotation(rootPointer);
    }
    
    //RL = right left rotation
    if (balanceFactor < -1 && tempUniqueID < rootPointer->rightChildPointer->uniqueID) {

        rootPointer->rightChildPointer = rightRotation(rootPointer->rightChildPointer);
        return leftRotation(rootPointer);
    }

    return rootPointer;
}

Student* Student::removeID(Student* rootPointer, string gatorID) {

    if (rootPointer == nullptr) {
        return rootPointer;
    }

    int tempUniqueID = stoi(gatorID);

    if (tempUniqueID < rootPointer->uniqueID) {
        rootPointer->leftChildPointer = removeID(rootPointer->leftChildPointer, gatorID);
    }
    else if (tempUniqueID > rootPointer->uniqueID) {
        rootPointer->rightChildPointer = removeID(rootPointer->rightChildPointer, gatorID);
    }
    else {

        //no child
        if (rootPointer->leftChildPointer == nullptr && rootPointer->rightChildPointer == nullptr) {
            //catch dangling pointer
            delete rootPointer;
            rootPointer = nullptr;
            cout << "successful\n";
        }
        //one child
        else if (rootPointer->leftChildPointer == nullptr) {
            Student* tempPointer = nullptr;
            tempPointer = rootPointer;
            rootPointer = rootPointer->rightChildPointer;
            delete tempPointer;
            tempPointer = nullptr;
            cout << "successful\n";
        }
        else if (rootPointer->rightChildPointer == nullptr) {
            Student* tempPointer = nullptr;
            tempPointer = rootPointer;
            rootPointer = rootPointer->leftChildPointer;
            delete tempPointer;
            tempPointer = nullptr;
            cout << "successful\n";
        }
        else {
            Student* tempPointer = minValueOfTree(rootPointer->rightChildPointer);
            rootPointer->studentName = tempPointer->studentName;
            rootPointer->gatorID = tempPointer->gatorID;
            rootPointer->uniqueID = tempPointer->uniqueID;
            rootPointer->rightChildPointer = removeID(rootPointer->rightChildPointer, tempPointer->gatorID);
            //may need a print statement in here for "successful"
        }
    }
    return rootPointer;
}

Student* Student::removeInorder(Student* rootPointer, int NthStudent) {

    if (rootPointer == nullptr) {
        cout << "unsuccessful\n";
        return rootPointer;
    }
    
    if (NthStudent < 0) {
        cout << "unsuccessful\n";
        return rootPointer;
    }
    
    //count number of nodes in the tree to make sure the Nth submission is valid
    int numberOfNodes = howManyStudentsInTree(rootPointer);
    
    //checks to see if the entry is possible given ammount of nodes
    if (numberOfNodes < NthStudent + 1) {
        cout << "unsuccessful\n";
        return rootPointer;
    }
    
    
    stack<Student*> StudentStack;

    Student* tempPointer = rootPointer;
    int index = 0;

    while ((StudentStack.empty() == false || tempPointer != nullptr) && index < NthStudent)
    {

        while (tempPointer != nullptr)
        {
            StudentStack.push(tempPointer);
            tempPointer = tempPointer->leftChildPointer;
        }

        tempPointer = StudentStack.top();
        StudentStack.pop();

        tempPointer = tempPointer->rightChildPointer;
   
        index++;
    }

    removeID(rootPointer, tempPointer->gatorID);
    return rootPointer;
}

void Student::printInOrder(Student* rootPointer) {

    //source: https://www.educative.io/edpresso/how-to-perform-an-iterative-inorder-traversal-of-a-binary-tree

    if (rootPointer == nullptr) {
        return;
    }

    stack<Student*> StudentStack;

    Student* tempPointer = rootPointer;

    //when the student node is null and stack is empty we have nothing more to add so we break out
    while (StudentStack.empty() == false || tempPointer != nullptr)
    {
        
        while (tempPointer != nullptr)
        {
            StudentStack.push(tempPointer);
            tempPointer = tempPointer->leftChildPointer;
        }

        tempPointer = StudentStack.top();
        StudentStack.pop();

        cout << tempPointer->studentName;
       
        tempPointer = tempPointer->rightChildPointer;
        
        //does not print the ", " on the last iteration
        if (!(StudentStack.empty() == true && tempPointer == nullptr)) {
            cout << ", ";
        }
    }
    cout << endl;
}

void Student::printPreOrder(Student* rootPointer) {
    
    //source: https://www.geeksforgeeks.org/iterative-preorder-traversal/

    if (rootPointer == nullptr) {
        return;
    }

    stack<Student*> StudentStack;
    StudentStack.push(rootPointer);

    Student* tempPointer = nullptr;
   
    while (StudentStack.empty() == false)
    {
        tempPointer = StudentStack.top();
        cout << tempPointer->studentName;
        
        StudentStack.pop();

        if (tempPointer->rightChildPointer) {
            
            StudentStack.push(tempPointer->rightChildPointer);
        }
        if (tempPointer->leftChildPointer) {
            
            StudentStack.push(tempPointer->leftChildPointer);
        }
        
        //does not print on the last iteration of this while loop
        if (StudentStack.empty() == false) {
            cout << ", ";
        }
    }

    cout << endl;
}

void Student::printPostOrder(Student* rootPointer) {

    //source : GeekforGeeks

    if (rootPointer == nullptr) {
        return;
    }

    //two stacks
    stack<Student*> StudentStack1;
    stack<Student*> StudentStack2;

    
    StudentStack1.push(rootPointer);
    Student* tempPointer;

    
    while (!StudentStack1.empty()) {
        
        //point to node then pop and push into next stack 
        tempPointer = StudentStack1.top();
        StudentStack1.pop();
        //everything gets pushed into stack 2
        StudentStack2.push(tempPointer);

        //if node exist push it into stack one
        if (tempPointer->leftChildPointer)
            StudentStack1.push(tempPointer->leftChildPointer);
        if (tempPointer->rightChildPointer)
            StudentStack1.push(tempPointer->rightChildPointer);
    }

    //go through stack 2 one by one and print everything
    while (!StudentStack2.empty()) {
        tempPointer = StudentStack2.top();
        StudentStack2.pop();
        cout << tempPointer->studentName;

        if (!StudentStack2.empty()) {
            cout << ", ";
        }
    }
    cout << endl;
}

void Student::printLevelCount(Student* rootPointer) {
    
    cout << heightOfTree(rootPointer) << endl;
}

void Student::searchNAME(Student* rootPointer, string studentName) {
    
    //if empty jump out
    if (rootPointer == nullptr) {
        cout << "unsuccessful\n";
        return;
    }

    stack<Student*> StudentStack;
    StudentStack.push(rootPointer);

    Student* tempPointer = nullptr;

    //bool to determine what to print
    bool wasAMatchFound = false;

    //while loop searches each node to see if a name is found since there can be multiple of same name
    while (StudentStack.empty() == false)
    {
        tempPointer = StudentStack.top();
        if (studentName.compare(tempPointer->studentName) == 0) {
            cout << tempPointer->gatorID << endl;
            wasAMatchFound = true;
        }

        StudentStack.pop();

        if (tempPointer->rightChildPointer) {

            StudentStack.push(tempPointer->rightChildPointer);
        }
        if (tempPointer->leftChildPointer) {

            StudentStack.push(tempPointer->leftChildPointer);
        }

    }

    //used to print if we go through entire tree and do not find a node
    if (wasAMatchFound == false) {
        cout << "unsuccessful\n";
    }
    
}
void Student::searchID(Student* rootPointer, string gatorID) {
    
    
    if (gatorID.length() != 8) {
        cout << "unsuccessful\n";
        return;
    }

    for (unsigned int i = 0; i < gatorID.length(); i++) {
        if (!isdigit(gatorID.at(i))) {
            cout << "unsuccessful\n";
            return;
        }
    }
    
    bool wasAMatchFound = false;    //used to break out of else statement in loop to prevent infinite loop
    Student* tempPointer = rootPointer;

    //if tempPointer is null loop will not be entered and goes straight to next if statement
    while (tempPointer != nullptr)
    {
        if (stoi(gatorID) > tempPointer->uniqueID) {
            tempPointer = tempPointer->rightChildPointer;
        }
        else if (stoi(gatorID) < tempPointer->uniqueID) {
            tempPointer = tempPointer->leftChildPointer;
        }
        else {
            cout << tempPointer->studentName << endl;
            wasAMatchFound = true;
            break;
        }
    }
    if (wasAMatchFound == false) {
        cout << "unsuccessful\n";
    }
}


int Student::heightOfTree(Student* rootPointer) {

    if (rootPointer == nullptr) {
        return 0;
    }
    return rootPointer->height;
}

int Student::getBalanceFactor(Student* rootPointer) {
    
    //0 if tree empty
    if (rootPointer == nullptr) {
        return 0;
    }
    //gets balance factor
    return heightOfTree(rootPointer->leftChildPointer) - heightOfTree(rootPointer->rightChildPointer);
}

Student* Student::minValueOfTree(Student* rootPointer) {

    Student* tempPointer = rootPointer;

    while (tempPointer && tempPointer->leftChildPointer != nullptr) {
        tempPointer = tempPointer->leftChildPointer;
    }

    return tempPointer;
}

Student* Student::leftRotation(Student* x) {

    //source : GeekForGeeks

    Student* y = x->rightChildPointer;
    Student* T2 = y->leftChildPointer;

    y->leftChildPointer = x;
    x->rightChildPointer = T2;

    x->height = max(heightOfTree(x->leftChildPointer),
        heightOfTree(x->rightChildPointer)) + 1;
    y->height = max(heightOfTree(y->leftChildPointer),
        heightOfTree(y->rightChildPointer)) + 1;

    return y;
}

Student* Student::rightRotation(Student* y) {

    //source : GeekForGeeks

    Student* x = y->leftChildPointer;
    Student* T2 = x->rightChildPointer;

    x->rightChildPointer = y;
    y->leftChildPointer = T2;

    y->height = max(heightOfTree(y->leftChildPointer), heightOfTree(y->rightChildPointer)) + 1;
    x->height = max(heightOfTree(x->leftChildPointer), heightOfTree(x->rightChildPointer)) + 1;

    return x;
}

int Student::howManyStudentsInTree(Student* rootPointer) {

    //goes through tree and counts the number of nodes present
    if (rootPointer == nullptr)
        return 0;
    return 1 + howManyStudentsInTree(rootPointer->leftChildPointer) + howManyStudentsInTree(rootPointer->rightChildPointer);

}

int main()
{
    Student studentTree;
    Student* rootPointer = nullptr;
    string name;
    string gatorID;
    
    int numberOfCommands = 0;
    string functionCall;

    cin >> numberOfCommands;

    for (int i = 0; i < numberOfCommands; i++) {

        cin >> functionCall;

        if (functionCall.compare("insert") == 0) {
            cin >> name; 
            //removes " " from the name
            name = name.substr(1, name.length() - 2);
            cin >> gatorID;
            rootPointer = studentTree.insertStudentNode(rootPointer, name, gatorID);
        }

        else if (functionCall.compare("remove") == 0) {
            //gets ID to remove
            cin >> gatorID;
            rootPointer = studentTree.removeID(rootPointer, gatorID);
        }
        
        else if (functionCall.compare("search") == 0) {
            //check if string is only digits to decide where to send it to
            string nameOrID;
            cin >> nameOrID;
            bool isDigit = true;

            for (unsigned int i = 0; i < nameOrID.length(); i++) {
                
                //checks if input is all digits to determine which function to call via isDigit boolean
                if ((!isdigit(nameOrID.at(i))))
                {
                    isDigit = false;
                    break;
                }
            }

            if (isDigit == true) {
               
                studentTree.searchID(rootPointer, nameOrID); 
            }
            else {
                //remove quoation marks from name
                nameOrID = nameOrID.substr(1, nameOrID.length() - 2);
                studentTree.searchNAME(rootPointer, nameOrID);
            }
        }

        //Print functions
        else if (functionCall.compare("printLevelCount") == 0) {
            studentTree.printLevelCount(rootPointer);
        }

        else if (functionCall.compare("printInorder") == 0) {
            studentTree.printInOrder(rootPointer);
        }

        else if (functionCall.compare("printPreorder") == 0) {
            studentTree.printPreOrder(rootPointer);
        }

        else if (functionCall.compare("printPostorder") == 0) {
            studentTree.printPostOrder(rootPointer);
        }
        
        //Remove Inorder
        else if (functionCall.compare("removeInorder") == 0) {
            //gets Nth removal from user than calls function
            int Nth = 0;
            cin >> Nth;

            rootPointer = studentTree.removeInorder(rootPointer, Nth);
        }
    }
    return 0;
}
