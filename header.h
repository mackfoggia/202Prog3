//Mack Foggia - CS 202 - Program 3
#include <iostream>
#include <cstring>
#include <fstream>


/* This program is a tool for creating very simple text adventure
games. It functions around a class called event, which holds the name
and information about one specific choice that the player will make.
As the player steps through the story, they will be able to choose
from two options at each event. All of the events are stored in a 
2-3 tree, and when the player makes a choice the program searches 
the tree for an event with that name and presents it to the player.
This program supports writing a script in an external file. In order
to do this, the format for one line in the file should look like this:

Name of event|Event text|Choice 1|Choice 2|Number of characters affected|First character's name|Second character's name|

If you want an event to be the end of a branch of the story, you must
make choice 1 and choice 2 'end' without the quotes. There is already
a basic script written by me included with this package. The program
also has a full testing suite, which allows for inserting new events
into the tree, displaying the full tree, retrieving an event from the
tree by name, and exiting the program, deleting the tree.

This file is the header and it holds the definitions for all of the
classes involved and all of the function prototypes.*/



//Event is the primary class for this program. It holds the name
//of the event, its text (what tells the player what has happened
//and what their choices are), two options (which should be names
//of other events), and the head of a linear linked list of
//characters that will be affected by this choice.
class event{
  public:
    event();
    event(const event & source);
    ~event();

    //Input prompts the user for each field, used only for testing
    void input();

    //Displays the event's name and text
    void display();

    //Displays the name of the event, mostly used for overloaded << below
    void displayName(); 
    friend std::ostream & operator<<(std::ostream & out, event & anEvent);

    //Insert a new character into the list of characters given their name
    void addCharacter(char * name);

    //Returns an option
    char * getOption(int i);

    //Reads in an event from a given istream object
    void readFromFile(std::istream & infile);

    //Recursive function to delete nodes
    void deleteNodes(class lnode * & head);
    
    //Relationship operators used for inserting into the tree
    friend bool operator<(const event &, const event &);
    friend bool operator<(const event &, const char *);
    friend bool operator<(const char *, const event &);
    friend bool operator>(const event &, const event &);
    friend bool operator>(const event &, const char *);
    friend bool operator>(const char *, const event &);
    friend bool operator<=(const event &, const event &);
    friend bool operator<=(const event &, const char *);
    friend bool operator<=(const char *, const event &);
    friend bool operator>=(const event &, const event &);
    friend bool operator>=(const event &, const char *);
    friend bool operator>=(const char *, const event &);
    friend bool operator==(const event &, const event &);
    friend bool operator==(const char *, const event &);
    friend bool operator==(const event &, const char *);
    friend bool operator!=(const event &, const event &);
    friend bool operator!=(const char *, const event &);
    friend bool operator!=(const event &, const char *);

  protected:
    char * name;
    char * text;
    char * option1;
    char * option2;
    class lnode * head;

    //Recursive display
    void display(lnode *);

    //Recursive function for copy constructor
    void copyNodes(lnode *& head, lnode * source);

    //Prompts user for character info when creating event
    void inputCharacters();

    //Deletes the data in an event if it needs to be reused
    void clearData();
};



//I'll be honest, this class fulfills the inheritance
//requirement for this assignment
class name{
  public:
    name();
    name(const name & source);
    name(const char *);

    //This was a good place to practice a bunch of overloading
    name & operator=(const name & source);
    name & operator=(const char * source);
    friend name & operator>>(std::istream &, name &);
    friend std::ostream & operator<<(std::ostream &, name &);
    char & operator[](const int i);
    name operator+(const name & toAdd);
    name operator+(const char * toAdd);
    name & operator+=(const name & toAdd);
    name & operator+=(const char * toAdd);
    ~name();

    //Prints the name
    void display();

  protected:
    char * Name;
};



//Node for the linear linked list of characters in each event.
//'IS A' name
class lnode: public name{
  public:
    lnode();
    lnode(const lnode & source);
    lnode(const char *);
    ~lnode();

    //Setter and getter
    void set(lnode * n);
    lnode *& get();

  protected:
    lnode * next;
};
    



//Tree classes

//Main tree manager called story 
class story{
  public:
    story();
    story(const story & source);
    ~story();

    //Wrapper for insertion given an event
    void insert(event & toAdd);

    //Displays the tree using preorder traversal for testing
    void display();

    //Runs the testing menu, allowing the client to insert,
    //display, retrieve, etc.
    void testingMenu();

    //Displays info about available commands
    void printHelp();

    //Changes the name of the event where the story starts
    void changeStart();
    
    //Starts the story from the player's perspective
    void runStory();

    //Wrapper for recursive retrieve
    void find();

    //Function to read from a file given the file name
    void readFromFile(char * fileName);

  protected:
    class node * root;
    char * start;
};



//Node for the 2-3 tree
class node{
  public:
    node();
    node(const node & source);
    node(event & toAdd);
    ~node();

    //Used in insertion, takes an event and returns 
    //the child that it is relevant to
    node *& travel(event & toAdd);

    //Inserts a given event into the tree recursively
    node * insert(event & toAdd, event *& storage);

    //Recursive retrieval function
    event * find(char * name);

    //Setter for children, rarely used
    void setChild(int i, node * set);

    //Displays what data a node contains (for testing)
    void display();

    //Recursive copy for copy constructors
    void copyNodes(node * & dest);

  protected:
    event * data[2];
    node * children[3];
};
