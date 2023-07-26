
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    char character;
    Node* left;
    Node* right;
    Node* up;
    Node* down;
};

void GridCreation(Node* head) {
    Node* n = head;
    Node* lower = new Node;

    lower->up = n;
    n->down = lower;

    Node* next = nullptr;
    Node* beneath = nullptr;

    for (int y = 1; y <= 50; y++) {
        Node* newLower = new Node;
        if (y != 50) {
            newLower->up = lower;
            lower->down = newLower;
        }
        
        for (int x = 1; x <= 50; x++) {

            if (x != 50) {                          //make a next node if n isn't at the right boundary
                if (y != 1)                        //if the top row is being worked on, we can't go up, so if y isn't 1, we use up->right->down
                    next = n->up->right->down;
                else
                    next = new Node;
            }
            else
                next = nullptr;

            if (x != 1 && y != 50) {                          //if x is on the first row, down is already connected vertically to n. If y is 50, that's the farthest down we need to go
                beneath = new Node;
                n->down = beneath;
                beneath->up = n;
            }

            if (next != nullptr) {                           //if the next pointer is generated 
                n->character = ' ';
                n->right = next;
                next->left = n;

                if (y == 1)
                    n->up = nullptr;
                if (x == 1)
                    n->left = nullptr;
                if (y == 50)
                    n->down = nullptr;
                if (x == 50)
                    n->right = nullptr;

                n = next;
            }
            else {                                          //if next pointer isn't generated, we're at X = 50, the rightmost node before the border

                if (y == 1)            
                    n->up = nullptr;
                if (y == 50)
                    n->down = nullptr;

                n->character = ' ';
                n->right = nullptr;
                if (y == 1)
                    n->up = nullptr;

                n = lower;
            }
            
        } //end of first loop

        lower = newLower;
    } // end of secondLoop

}

void printGrid(ofstream& outputfile, Node *head, Node *n, int rows, int option) { //head is kept constant, n is the pointer that moves around. option determines if printGrid prints to console or the file
   
    if (head == nullptr)
        return;
    
    Node* lower = new Node;
    lower = head->down;
   
    if (rows == 50) {    //grid is 50x50, when 50 rows are counted, we're done
        rows = 0;
        return;
    }
    if (option == 1)
        cout << n->character;
    if (option == 2)
        outputfile << n->character;
    
    if (n->right == nullptr) { //once the end of a line is reached, head is moved down a row, and rows increases by 1
        rows++;
        head = lower;
        if (option == 1)
            cout << endl;
        if (option == 2 && rows != 50)
            outputfile << endl;
        printGrid(outputfile, head, head, rows, option);
    }

    else {
        printGrid(outputfile, head, n->right, rows, option);  //move to the next Node along the row
    }
}

void deleteGrid(Node* head) { 
    Node* hold = nullptr;
    Node* lower = head->down;
    Node* newLower = nullptr;

    for (int y = 1; y <= 50; y++) {
        if (y != 50)
            newLower = lower->down;

        for (int x = 1; x <= 50; x++) {
            
            hold = head;

            if (head->right != nullptr)    //if we're not at the end of the row, head moves right one
                head = head->right;             
            else                           //if we're at the end of the row, head moves to start of next row
                head = lower;

            free(hold);
        }
        lower = newLower;
    }

}

void drawLeftRight(Node*& pointer, bool penStatus, int distance, string direction, bool isBold = false) {

    for (int x = 0; x < distance; x++) {
        if (direction == "E")
            pointer = pointer->right;
        if (direction == "W")
            pointer = pointer->left;

        if (penStatus) {
            if (isBold)
                pointer->character = '#';
            else {
                if (pointer->character != '#')
                    pointer->character = '*';
            }
        }
    }   
}

void drawUpDown(Node*& pointer, bool penStatus, int distance, string direction, bool isBold = false) {
    for (int x = 0; x < distance; x++) {
        if (direction == "N")
            pointer = pointer->up;
        if (direction == "S")
            pointer = pointer->down;

        if (penStatus) {
            if (isBold)
                pointer->character = '#';
            else {
                if (pointer->character != '#')
                    pointer->character = '*';
            }
        }
    }
}

bool isInBound(Node* pointer, int distance, string direction) {
    bool bounded = true;
    for (int x = 1; x <= distance; x++) {
        if (direction == "N") {
            if ((pointer->up == nullptr) && (x <= distance)) {  //if the next pointer in the direction is null, and the loop isn't at its end [meaning there's still more line which occurs when x < distance], the line goes out of bounds.
                bounded = false;                                //or, if the line ends exactly at a null border, it's also out. This happens when x == distance                                  
                return bounded;                                                                    
            }
            else
                pointer = pointer->up;
        }
        if (direction == "S") {
            if ((pointer->down == nullptr) && (x <= distance)) {
                bounded = false;
                return bounded;
            }
            else
                pointer = pointer->down;
        }
        if (direction == "E") {
            if ((pointer->right == nullptr) && (x <= distance)) {
                bounded = false;
                return bounded;
            }
            else
                pointer = pointer->right;
        }
        if (direction == "W") {
            if ((pointer->left == nullptr) && (x <= distance)) {
                bounded = false;
                return bounded;
            }
            else
                pointer = pointer->left;
        }
    }
    return bounded;
}



int main()                  
{
    Node* head = new Node;
    head->up = nullptr;
    head->left = nullptr;
    Node* drawingPointer = new Node; //this is the pointer that gets moved around while drawing
    drawingPointer = head;
    GridCreation(head);
    int rowCount = 0;
   
    
    bool actualPenStatus = false, isBold = false;
    string inputfileName, line;
    
    cin >> inputfileName;
    ifstream inputFile(inputfileName, ios::in);
    ofstream outfile("paint.txt");

    if (inputFile) {
        while (!inputFile.eof()) {
            while (getline(inputFile, line)) {
                isBold = false;
                size_t firstComma = line.find(",");
                string penStatus = line.substr(0, firstComma); //parses first command penStatus
                if (penStatus == "2") //penStatus is the string containing 1 or 2, actualPenStatus is the boolean value, if not 1 or 2 then skip
                    actualPenStatus = true;
                else if (penStatus == "1")
                    actualPenStatus = false;
                else
                    continue;

                size_t secondComma = line.find(",", firstComma + 1);
                string direction = line.substr(firstComma + 1, (secondComma - (firstComma + 1))); //parses Direction, assumes only one character

                if (direction != "N" && direction != "S" && direction != "E" && direction != "W") //validates direction
                    continue;

                size_t thirdComma = line.find(",", secondComma + 1);
                string distance = "";

                string fourthCommand = "";
                string fifthCommand = "";

                if (thirdComma == string::npos) { //if no third comma is found
                    distance = line.substr(secondComma + 1); // parses length if there's nothing else
                    isBold = false;
                }

                else {//if third comma exists / if B/P or both exists
                    distance = line.substr(secondComma + 1, (thirdComma - (secondComma + 1)));

                    size_t fourthComma = line.find(",", thirdComma + 1);
                    if (fourthComma != string::npos) {                                               //if fourthComma exists
                        fourthCommand = line.substr(thirdComma + 1, (fourthComma - (thirdComma + 1)));
                        if (fourthCommand != "B" && fourthCommand != "P")                                          //fourthcommand is either B or P, if it isn't them, it's invalid
                            continue;

                        fifthCommand = line.substr(fourthComma + 1);                               //the fifthcommand can only be P
                        if (fifthCommand != "P")
                            continue;
                        if (fifthCommand == "P" && fourthCommand == "P")                          //if both the fifth and fourth commands are P, it's invalid. (It would get through the earlier logic)
                            continue;
                    }
                    else {     //if fourth comma doesn't exist. Just B or P
                        fourthCommand = line.substr(thirdComma + 1);
                        if (fourthCommand != "B" && fourthCommand != "P") {          //if it isn't B or P, it's invalid
                            continue;
                        }
                    }

                }

                if (distance.find_first_not_of("0123456789") != string::npos) //validates distance, sees if is entirely a number, otherwise something like "10eg" would work and be treated as 10 with stoi
                    continue;

                if (stoi(distance) < 0)     // if distance is a negative number, it's invalid
                    continue;

                if (line.find('B') != string::npos) {
                    isBold = true;
                    if (line.find('P') < line.find('B'))
                        continue;
                }

                if (actualPenStatus == false && isBold) //skip if pen is up and bold, contradictory
                    continue;

                if (direction == "N" || direction == "S") {
                    if (isInBound(drawingPointer, stoi(distance), direction)) {
                     //   cout << "this is in bound North South" << endl;
                        drawUpDown(drawingPointer, actualPenStatus, stoi(distance), direction, isBold);
                    }
                    else {
                      //  cout << "This isn't in bound North South" << endl;
                        continue;
                    }
                }
                if (direction == "E" || direction == "W") {
                    if (isInBound(drawingPointer, stoi(distance), direction)) {
                    //    cout << "this is in bound East West" << endl;
                        drawLeftRight(drawingPointer, actualPenStatus, stoi(distance), direction, isBold);
                    }
                    else {
                  //      cout << "this isn't in bound East west" << endl;
                        continue;
                    }
                }

                if (line.find('P') != string::npos) { //checks if p is in the line
                    printGrid(outfile, head, head, rowCount, 1);
                    cout << endl << endl;
                }
               
            }//end of getline stuff
            printGrid(outfile, head, head, rowCount, 2);
        }
    } //end of if inputfile  
    outfile.close();
    deleteGrid(head);
}
