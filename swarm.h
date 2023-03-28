//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
//Title: swarm.h
//Author: Christina Dao
//Date: 10/26/2021
//Description: This file is the header file for my project 2
//E-mail: cdao1@umbc.edu
//Course Section: CMSC 341 Section 03

#ifndef SWARM_H
#define SWARM_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
enum STATE {ALIVE, DEAD};
enum ROBOTTYPE {BIRD, DRONE, REPTILE, SUB, QUADRUPED};
const int MINID = 10000;
const int MAXID = 99999;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE DRONE
#define DEFAULT_STATE ALIVE
class Robot{
public:
    friend class Swarm;
    friend class Grader;
    friend class Tester;
    Robot(int id, ROBOTTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
            :m_id(id),m_type(type), m_state(state) {
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    Robot(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
            case DEAD:text = "DEAD";break;
            case ALIVE:text = "ALIVE";break;
            default:text = "UNKNOWN";break;
        }
        return text;
    }
    ROBOTTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
            case BIRD:text = "BIRD";break;
            case DRONE:text = "DRONE";break;
            case REPTILE:text = "REPTILE";break;
            case SUB:text = "SUB";break;
            case QUADRUPED:text = "QUADRUPED";break;
            default:text = "UNKNOWN";break;
        }
        return text
                ;
    }
    int getHeight() const {return m_height;}
    Robot* getLeft() const {return m_left;}
    Robot* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(ROBOTTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Robot* left){m_left=left;}
    void setRight(Robot* right){m_right=right;}

private:
    int m_id;
    ROBOTTYPE m_type;
    STATE m_state;
    Robot* m_left;//the pointer to the left child in the BST
    Robot* m_right;//the pointer to the right child in the BST
    int m_height;//the height of node in the BST
};

class Swarm {
public:
    friend class Grader;

    friend class Tester;

    Swarm();

    ~Swarm();

    void insert(const Robot &robot);

    void clear();

    void remove(int id);

    void dumpTree() const;

    void listRobots() const;

    bool setState(int id, STATE state);

    void removeDead();//removes all dead robots from the tree
    bool findBot(int id) const;//returns true if the bot is in tree

private:
    Robot *m_root;//the root of the BST

    void dump(Robot *aBot) const;

    void updateHeight(Robot *aBot);

    int checkImbalance(Robot *aBot);

    Robot *rebalance(Robot *aBot);

    // ***************************************************
    // Any private helper functions must be declared here!
    // ***************************************************

    Robot* insertRecursively(const Robot &insertBot, Robot *currNode); // recursive helper function for insert
    Robot *removeHelper(Robot *aBot, int ID); // remove helper func
    Robot *findSmallestRight(Robot *aBot); // function to find smallest robot on right
    Robot *maxBot(Robot *aBot); // helper func to fin max noe on right of aBot
    Robot* findHelper(Robot *aBot, int id) const; // helper function to find a robot
    Robot* rotateL(Robot* aBot); // rotating left function
    Robot* rotateR(Robot* aBot); // rotation right function
    void listHelper(Robot* aBot) const; // helps display all the robots and their information
    void deadHelper(Robot* aBot); // helps to remove dead bots
    void clearHelper(Robot* aBot); // helps to deallocate memory
};

#endif