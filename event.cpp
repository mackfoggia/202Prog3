//Mack Foggia - CS 202 - Program 3
#include "header.h"
using namespace std;

/* This file holds implementations for member functions
of the event, lnode, and name classes. */




//Event member functions

//Default constructor
event::event(): name(NULL), text(NULL), option1(NULL), option2(NULL), head(NULL){}


//Copy constructor
event::event(const event & source){
  name = new char[strlen(source.name) + 1];
  strcpy(name, source.name);

  text = new char[strlen(source.text) + 1];
  strcpy(text, source.text);

  option1 = new char[strlen(source.option1) + 1];
  strcpy(option1, source.option1);

  option2 = new char[strlen(source.option2) + 1];
  strcpy(option2, source.option2);

  copyNodes(head, source.head);
}


//Destructor
event::~event(){
  clearData();
}


//Input prompts the user for each field, used only for testing
void event::input(){
  clearData();

  int size = 500;

  char * temp = new char[size];

  cout << "Enter name of event: ";
  cin.get(temp, size, '\n');
  cin.ignore(100, '\n');
  name = new char[strlen(temp) + 1];
  strcpy(name, temp);

  cout << "Enter event text (up to " << size << " characters, don't press enter until done): ";
  cin.get(temp, size, '\n');
  cin.ignore(100, '\n');
  text = new char[strlen(temp) + 1];
  strcpy(text, temp);

  cout << "Enter the event name that option 1 leads to." << endl
       << "If this is the end of this branch of the story, type 'end' (case-sensitive): ";
  cin.get(temp, size, '\n');
  cin.ignore(100, '\n');
  option1 = new char[strlen(temp) + 1];
  strcpy(option1, temp);

  cout << "Enter the event name that option 2 leads to (case-sensitive): ";
  cin.get(temp, size, '\n');
  cin.ignore(100, '\n');
  option2 = new char[strlen(temp) + 1];
  strcpy(option2, temp);

  //Prompts for characters that are affected by this choice
  inputCharacters();

  delete [] temp;
}


//Prompts user for character info when creating event
void event::inputCharacters(){
  char * temp = new char[50];
  char again;

  cout << "Does this choice affect any characters? (y/n) ";
  cin >> again;
  cin.ignore(100, '\n');
  again = toupper(again);
  while(again == 'Y'){
    cout << "Enter the name of the character: ";
    cin.get(temp, 50, '\n');
    cin.ignore(100, '\n');
    addCharacter(temp);
    cout << "Another? (y/n) ";
    cin >> again;
    again = toupper(again);
    cin.ignore(100, '\n');
  }
}


//Displays the event's name and text
void event::display(){
  cout << name << endl << endl << text << endl;
  if(head){ 
    cout << "Affected characters: ";
    display(head);
  }
  else cout << "This won't affect any characters." << endl;
}


//Recursive display function
void event::display(lnode * head){
  if(!head){
    cout << endl;
    return;
  }
  head->display();
  if(head->get()) cout << ", ";
  display(head->get());
}



//Recursive function for copy constructor
void event::copyNodes(lnode *& head, lnode * source){
  if(!source){
    head = NULL;
    return;
  }
  head = new lnode(*source);
  copyNodes(head->get(), source->get());
}


//Insert a new character into the list of characters given their name
void event::addCharacter(char * newName){
  lnode * temp = head;
  head = new lnode(newName);
  head->set(temp);
}


//Displays the name of the event
void event::displayName(){
  cout << name;
}

//Overloaded << to dislay event's name
ostream & operator<<(ostream & out, event & anEvent){
  out << anEvent.name;
  return out;
}


//Returns a requested option
char * event::getOption(int i){
  if(i == 1) return option1;
  else return option2;
}


//Deletes the data in an event if it needs to be reused
void event::clearData(){
  if(name){
    delete [] name;
    name = NULL;
  }
  if(text){
    delete [] text;
    text = NULL;
  }
  if(option1){
    delete [] option1;
    option1 = NULL;
  }
  if(option2){
    delete [] option2;
    option2 = NULL;
  }
  if(head) deleteNodes(head);
  head = NULL;
}
  

//Reads in an event from a given istream object
void event::readFromFile(istream & infile){
  clearData();

  int size = 500;
  char * temp = new char[size];
  int num = 0;
  
  infile.get(temp, size, '|');
  if(infile.eof()){
    delete [] temp;
    return;
  }

  infile.ignore(1, '|');
  name = new char[strlen(temp) + 1];
  strcpy(name, temp);

  infile.get(temp, size, '|');
  infile.ignore(1, '|');
  text = new char[strlen(temp) + 1];
  strcpy(text, temp);

  infile.get(temp, size, '|');
  infile.ignore(1, '|');
  option1 = new char[strlen(temp) + 1];
  strcpy(option1, temp);

  infile.get(temp, size, '|');
  infile.ignore(1, '|');
  option2 = new char[strlen(temp) + 1];
  strcpy(option2, temp);

  infile >> num;
  infile.ignore(1, '|');

  for(int i = 0; i < num; ++i){
    infile.get(temp, size, '|');
    addCharacter(temp);
    infile.ignore(1, '|');
  }
  
  infile.ignore(1, '\n');

  delete [] temp;
  return;
}


//Recursive function to delete nodes
void event::deleteNodes(lnode *& head){
  if(!head) return;
  deleteNodes(head->get());
  delete head;
}



//Event relationship operators used for insertion into the tree
bool operator<(const event & a, const event & b){
  if(strcmp(a.name, b.name) < 0) return true;
  else return false;
}

bool operator<(const event & a, const char * b){
  if(strcmp(a.name, b) < 0) return true;
  else return false;
}

bool operator<(const char * a, const event & b){
  if(strcmp(a, b.name) < 0) return true;
  else return false;
}

bool operator>(const event & a, const event & b){
  if(strcmp(a.name, b.name) > 0) return true;
  else return false;
}

bool operator>(const event & a, const char * b){
  if(strcmp(a.name, b) > 0) return true;
  else return false;
}

bool operator>(const char * a, const event & b){
  if(strcmp(a, b.name) > 0) return true;
  else return false;
}

bool operator>=(const event & a, const event & b){
  if(strcmp(a.name, b.name) >= 0) return true;
  else return false;
}

bool operator>=(const event & a, const char * b){
  if(strcmp(a.name, b) >= 0) return true;
  else return false;
}

bool operator>=(const char * a, const event & b){
  if(strcmp(a, b.name) >= 0) return true;
  else return false;
}

bool operator<=(const event & a, const event & b){
  if(strcmp(a.name, b.name) <= 0) return true;
  else return false;
}

bool operator<=(const event & a, const char * b){
  if(strcmp(a.name, b) <= 0) return true;
  else return false;
}

bool operator<=(const char * a, const event & b){
  if(strcmp(a, b.name) <= 0) return true;
  else return false;
}

bool operator==(const event & a, const event & b){
  if(strcmp(a.name, b.name) == 0) return true;
  else return false;
}

bool operator==(const event & a, const char * b){
  if(strcmp(a.name, b) == 0) return true;
  else return false;
}

bool operator==(const char * a, const event & b){
  if(strcmp(a, b.name) == 0) return true;
  else return false;
}

bool operator!=(const event & a, const event & b){
  if(strcmp(a.name, b.name) != 0) return true;
  else return false;
}

bool operator!=(const event & a, const char * b){
  if(strcmp(a.name, b) != 0) return true;
  else return false;
}

bool operator!=(const char * a, const event & b){
  if(strcmp(a, b.name) != 0) return true;
  else return false;
}






//Name member functions


//Default constructor
name::name(): Name(NULL){}


//Copy constructor
name::name(const name & source){
  Name = new char[strlen(source.Name) + 1];
  strcpy(Name, source.Name);
}


//Constructor from char *
name::name(const char * s){
  Name = new char[strlen(s)+1];
  strcpy(Name, s);
}


//This was a good place to practice more operators
name & name::operator=(const name & source){
  *this = source.Name;
  return *this;
}

name & name::operator=(const char * source){
  delete [] Name;
  Name = new char[strlen(source) + 1];
  strcpy(Name, source);
  return *this;
}

name & operator>>(istream & in, name & nameObj){
  if(nameObj.Name) delete [] nameObj.Name;
  char * temp = new char[50];
  in.get(temp, 50, '\n');
  in.ignore(100, '\n');
  nameObj.Name = new char[strlen(temp) + 1];
  strcpy(nameObj.Name, temp);
  delete [] temp;
  return nameObj;
}

ostream & operator<<(ostream & out, name & aName){
  out << aName.Name;
  return out;
}

char & name::operator[](const int i){
  return Name[i];
}

name name::operator+(const char * toAdd){
  char * temp = new char[strlen(Name) + strlen(toAdd) + 1];
  strcpy(temp, Name);
  strcat(temp, toAdd);
  name nameTemp(temp);
  return nameTemp;
}

name name::operator+(const name & toAdd){
  return *this + toAdd.Name;
}

name & name::operator+=(const char * toAdd){
  char * temp = new char[strlen(Name) + strlen(toAdd) + 1];
  strcpy(temp, Name);
  strcat(temp, toAdd);
  delete [] Name;
  Name = temp;
  return *this;
}

name & name::operator+=(const name & toAdd){
  return *this += toAdd.Name;
}


//Destructor
name::~name(){
  if(Name) delete [] Name;
}


//Outputs the name (mostly irrelevant due to << overload)
void name::display(){
  cout << Name;
}






//Lnode member functions

//Default constructor
lnode::lnode(): next(NULL){}


//Copy constructor
lnode::lnode(const lnode & source): name(source), next(NULL){}


//Constructor from char * name
lnode::lnode(const char * s):name(s), next(NULL){}


//Destructor currently does nothing, if it is still here
//I probably forgot to remove it
lnode::~lnode(){
}


//Setter and getter
void lnode::set(lnode * source){
  next = source;
}

lnode *& lnode::get(){
  return next;
}
