//Mack Foggia - CS 202 - Program 3
#include "header.h"
using namespace std;



//Main allows the user to pick between three ways to work with/test the program.
int main(){
  story aStory;

  cout << "Type 1 to read from the external file and start the story" << endl
       << "Type 2 to read from the external file and start the testing menu" << endl
       << "Type 3 to start the testing menu with an empty tree" << endl;
  int mode = 0;
  cin >> mode;
  cin.ignore(100, '\n');
  
  switch(mode){
    case 1:
      aStory.readFromFile("script.txt");
      aStory.runStory();
      break;
    case 2:
      aStory.readFromFile("script.txt");
      aStory.testingMenu();
      break;
    case 3:
      aStory.testingMenu();
      break;
  }

  return 0;
}
