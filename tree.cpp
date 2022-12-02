//Mack Foggia - CS 202 - Program 3
#include "header.h"
using namespace std;

/* This file holds the implementations for member functions
in the story and node classes. */



//Story class functions

//Default constructor
story::story(): root(NULL), start(NULL){
}


//Copy constructor
story::story(const story & source){
  if(source.root) source.root->copyNodes(root);
  if(start){
    start = new char[strlen(source.start) + 1];
    strcpy(start, source.start);
  }
}


//Destructor
story::~story(){
  delete root;
  delete [] start;
}


//Starts the story from the player's perspective
void story::runStory(){
  if(!root){
    cout << "There aren't any events yet!" << endl;
    return;
  }

  event * current = root->find(start);
  if(!current){
    cout << "The starting event hasn't been created yet.";
    return;
  }

  int choice = 0;

  do{
    current->display();
    do{
      cout << "Which option do you choose? (type 1 or 2): ";
      cin >> choice;
      cin.ignore(100, '\n');
    }while(choice != 1 && choice != 2);
    current = root->find(current->getOption(choice));
  }while(current && strcmp(current->getOption(1), "end") != 0);

  if(!current){
    cout << "This branch of the story seems unfinished..." << endl
         << "Your choice didn't lead anywhere." << endl;
  }

  else{
    current->display();
    cout << endl << "That's the end of the story, thanks for playing!" << endl;
  }
}


//Function to read from a file given the file name
void story::readFromFile(char * fileName){
  ofstream outfile;
  outfile.open(fileName, ios::app);
  outfile.close();

  ifstream infile;
  infile.open(fileName);
  char * temp = new char[500];
  infile.get(temp, 500, '\n');
  if(infile.eof()){
    cout << "Empty script file!" << endl;
    delete [] temp;
    return;
  }

  infile.ignore(1, '\n');
  start = new char[strlen(temp) + 1];
  strcpy(start, temp);

  event toAdd;
 
  toAdd.readFromFile(infile);
  while(!infile.eof()){
    insert(toAdd);
    toAdd.readFromFile(infile);
  }

  delete [] temp;
}


//Wrapper for insertion given an event
void story::insert(event & toAdd){
  if(!root){ //empty tree 
    root = new node(toAdd);
    if(!start) changeStart();
    return;
  }

  event * storage = NULL;
  node * temp = root->insert(toAdd, storage);

  //If something gets pushed up past the root
  if(temp){
    node * nodeTemp = new node(*storage);
    delete storage;
    nodeTemp->setChild(0, root);
    nodeTemp->setChild(2, temp);
    root = nodeTemp;
  }

  if(!start) changeStart();

  return;
}


//Changes the name of the event where the story starts
void story::changeStart(){
  if(start) delete [] start;
  char * temp = new char[50];
  cout << "What is the name of the event where the story starts? ";
  cin.get(temp, 50, '\n');
  cin.ignore(100, '\n');
  start = new char[strlen(temp) + 1];
  strcpy(start, temp);
  delete [] temp;
}


//Runs the testing menu, allowing the client to insert,
//display, retrieve, etc.
void story::testingMenu(){
  char c;
  event newEvent;

  do{
    cout << endl << "Enter a command or type 'help': ";
    cin >> c;
    cin.ignore(100, '\n');
    c = toupper(c);

    switch(c){
      case 'H':
        printHelp();
        break;

      case 'I':
        newEvent.input();
        insert(newEvent);
        break;

      case 'D':
        display();
        break;

      case 'E':
        cout << "Thanks for using the program!" << endl;
        break;

      case 'R':
        find();
        break;

      case 'S':
        runStory();
        break;

      default:
        cout << "I didn't understand that." << endl;
        break;
    }
  }while(c != 'E');
}


//Wrapper for recursive retrieve
void story::find(){
  char * temp = new char[50];
  cout << "Enter name to search for: ";
  cin.get(temp, 50, '\n');
  cin.ignore(100, '\n');
  event * eventTemp = root->find(temp);
  if(!eventTemp)
    cout << "Couldn't find a match." << endl;
  else{
    cout << "Found: ";
    eventTemp->display();
    cout << endl;
  }
  delete [] temp;
}
  

//Displays info about available commands
void story::printHelp(){
  cout << "'help' - Displays this screen" << endl
       << "'display' - Prints the full tree using preorder traversal" << endl
       << "'insert' - Prompts for info for a new event and adds it to the tree" << endl
       << "'exit' - Exits the program" << endl
       << "'retrieve' - Prompts for a name and searches the tree for it" << endl
       << "'story' - Runs the story function, allowing you to step through it" << endl
       << endl;
}


//Wrapper for recursive display (displays using preorder)
void story::display(){
  if(!root) return;
  root->display();
}







//Node class functions

//Default constructor
node::node(){
  data[0] = data[1] = NULL;
  for(int i = 0; i < 3; ++i){
    children[i] = NULL;
  }
}


//Copy constructor
node::node(const node & source){
  data[0] = new event(*source.data[0]);
  if(source.data[1])
    data[1] = new event(*source.data[1]);
  else data[1] = NULL;
  for(int i = 0; i < 3; ++i){
    children[i] = NULL;
  }
}


//Constructor to initialize first piece of data to
//given value, leaving everything else null
node::node(event & toAdd){
  data[0] = new event(toAdd);
  data[1] = NULL;
  for(int i = 0; i < 3; ++i){
    children[i] = NULL;
  }
}


//Destructor
node::~node(){
  delete data[0];
  if(data[1]) delete data[1];
  if(children[0]) delete children[0];
  if(children[1]) delete children[1];
  if(children[2]) delete children[2];
}


//Recursive copy for copy constructors
void node::copyNodes(node * & dest){
  dest = new node(*this);
  if(children[0]) children[0]->copyNodes(dest->children[0]);
  if(children[1]) children[1]->copyNodes(dest->children[1]);
  if(children[2]) children[2]->copyNodes(dest->children[2]);
}


//Setter for children, rarely used
void node::setChild(int i, node * set){
  children[i] = set;
}


//Inserts a given event into the tree recursively
node * node::insert(event & toAdd, event *& storage){
  if(!children[0]){ //We are at a leaf

    //Only one data stored currently
    if(!data[1]){
      if(toAdd >= *data[0])//Check which order they should be in
        data[1] = new event(toAdd);
      else{
        data[1] = data[0];
        data[0] = new event(toAdd);
      }
      node * temp = NULL;
      return temp;
    }

    //If two data stored, now check which should be pushed up
    if(toAdd < *data[0]){
      storage = data[0];
      data[0] = new event(toAdd);
    }

    else if(toAdd > *data[1]){
      storage = data[1];
      data[1] = new event(toAdd);
    }

    else storage = new event(toAdd);

    //Split the node and return the new node's pointer
    node * temp = new node(*data[1]);
    data[1] = NULL;

    return temp;
  }

  node * temp = (travel(toAdd))->insert(toAdd, storage);

  if(temp){//A lower node split, and has sent up data
    
    //Only one data in this node
    if(!data[1]){

      //The split came from the right
      if(*storage >= *data[0]){
        data[1] = storage;
        children[1] = children[2];
        children[2] = temp;
      }

      //The split came form the left
      else{
        data[1] = data[0];
        data[0] = storage;
        children[1] = temp;
      }

      storage = NULL;
      temp = NULL;
      return temp;
    }

    //There are two data items in this node
    else{
      
      //The split came from the right
      if(*storage >= *data[1]){

        //Send up the middle data
        event * eventTemp = storage;
        storage = data[1];
        data[1] = eventTemp;

        //Split the node
        node * nodeTemp = new node(*data[1]);
        delete data[1];
        data[1] = NULL;
        nodeTemp->children[0] = children[2];
        children[2] = children[1];
        children[1] = NULL;
        nodeTemp->children[2] = temp;
        return nodeTemp;
      }

      //The split came from the left
      if(*storage < *data[0]){
        
        //Send up the middle data
        event * eventTemp = storage;
        storage = data[0];
        data[0] = eventTemp;

        //Split the node
        node * nodeTemp = new node(*data[1]);
        delete data[1];
        data[1] = NULL;
        nodeTemp->children[0] = children[1];
        nodeTemp->children[2] = children[2];
        children[1] = NULL;
        children[2] = temp;
        return nodeTemp;
      }


      //The split came from the middle
      else{
        
        //Don't need to send up data because it is already stored

        //Split the node
        node * nodeTemp = new node(*data[1]);
        delete data[1];
        data[1] = NULL;
        nodeTemp->children[2] = children[2];
        children[2] = children[1];
        children[1] = NULL;
        nodeTemp->children[0] = temp;
        return nodeTemp;
      }
    }
  }
  temp = NULL;
  return temp;
}


//Recursive retrieval function
event * node::find(char * name){
  if(*data[0] == name) return data[0];
  if(data[1]){
    if(*data[1] == name) return data[1];
  }

  if(!children[0]){
    event * temp = NULL;
    return temp;
  }
  
  if(name < *data[0]) 
    return children[0]->find(name);

  if(data[1] && name < *data[1])
    return children[1]->find(name);

  return children[2]->find(name);
}


//Displays what data a node contains (for testing)
void node::display(){
  if(!data[1]){
    cout << "One piece of data: ";
    data[0]->displayName();
    cout << endl;
  }
  else{ 
    cout << "Two pieces of data: "; 
    data[0]->displayName();
    cout << ", "; 
    data[1]->displayName(); 
    cout << endl;
  }
  if(children[0]) children[0]->display();
  if(children[1]) children[1]->display();
  if(children[2]) children[2]->display();
}


//Used in insertion, takes an event and returns 
//the child that it is relevant to
node *& node::travel(event & toAdd){
  if(!data[1]){//If we only have two choices
    if(toAdd >= *data[0])
      return children[2];
    return children[0];
  }

  //If we have three choices
  if(toAdd >= *data[1])
    return children[2];

  if(toAdd < *data[0])
    return children[0];

  return children[1];
}
