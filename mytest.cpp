//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
//Title: mytest.cpp
//Author: Christina Dao
//Date: 10/26/2021
//Description: This file is essentially my projects' driver which
//             calls for all the required tests of my program for project 2
//E-mail: cdao1@umbc.edu
//Course Section: CMSC 341 Section 03

#include "swarm.h"
#include <random>

enum RANDOM {UNIFORM, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORM) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 and standard deviation of 20
            m_normdist = std::normal_distribution<>(50,20);
        }
        else{
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
    }

    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else{
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//uniform distribution

};

class Tester{
public:
    // Name: imbalanceCheck(Swarm& tree)
    // Desc: calls imbalanceCheck with parameter of tree and the root to test
    // Postconditions: Robots heights are displayed
    void imbalanceCheck(Swarm& tree){
        imbalanceCheck(tree, tree.m_root);
    }

    // Name: imbalanceCheck(Swarm& tree, Robot* abot)
    // Desc: this functions displays each robot id and their heights
    //       to prove that the heights do not differ more than one
    // Postconditions: Robots ID and their heights are displayed.
    void imbalanceCheck(Swarm& tree, Robot* aBot){
        if (aBot){
            imbalanceCheck(tree, aBot->getLeft());
            imbalanceCheck(tree, aBot->getRight());
            cout << "ID: " << aBot->getID() << "    Balance Factor: " << tree.checkImbalance(aBot) << endl;
        }
    }

    // Name: isBST(Swarm& tree)
    // Desc: calls and passes in the swarm root into the other
    // isBST function that carries more parameters
    // Postcondition: true or false returned depending on if it satisfies BSt
    // conditions or not
    bool isBST(Swarm& tree){
        return isBST(tree, tree.m_root);
    }

    // Name: isBST(Swarm& tree, Robot* aBot)
    // Desc: checks that left nodes are smaller than parent,  and right nodes are bigger than parents
    // Postconditions: true or false returned depending on if it satisfies BSt
    // conditions or not
    bool isBST(Swarm& tree, Robot* aBot){
        //null pointers allowed
        if (!aBot){
            return true;
        }
        // Check that left ID is smaller than parent
        if (aBot->getLeft() && aBot->getLeft()->getID() > aBot->getID()){
            return false;
        }
        // Check that right ID is greater than parent
        if (aBot->getRight() && aBot->getRight()->getID() < aBot->getID()){
            return false;
        }
        // if subtrees are not BSt then entire tree is not BST
        if (!isBST(tree, aBot->getRight()) || !isBST(tree, aBot->getLeft())){
            return false;
        }
        // left is smaller and right is greater than parents, its is a bst
        return true;
    }

    // Name: makeLeftChildOfRootDead(Swarm& tree)
    // Desc: calls and passes the root into other makeLeftChildOfRootDead(Swarm& tree, Robot* aBot)
    // Postcondition:  left child of root is Dead
    void makeLeftChildOfRootDead(Swarm& tree){
        makeLeftChildOfRootDead(tree, tree.m_root);
    }

    // Name: makeLeftChildOfRootDead(Swarm& tree, Robot* aBot)
    // Desc: sets left child of root to be dead
    // Postcondition: left child of root is Dead
    void makeLeftChildOfRootDead(Swarm& tree, Robot* aBot){
        aBot->getLeft()->setState(DEAD);
    }

};

int main() {
    Tester tester;
    Random idGen(MINID, MAXID);
    Random typeGen(0, 4);
    {
        Swarm team;
        int teamSize = 10;
        int tempID = 0;
        int ID = 0;
        cout << "Testing insert function, 10 bots: " << endl;
        for (int i = 0; i < teamSize; i++) {
            ID = idGen.getRandNum();
            if (i == teamSize / 2) tempID = ID;//we store this ID for later use
            Robot robot(ID, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        cout << "\nDump after inserting " << teamSize << " nodes:\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after inserting " << teamSize << " nodes:\n";
        team.listRobots();
        cout << endl;


        cout << endl << "Testing inserting duplicate of tempID, " << tempID << endl;
        Robot robot(tempID, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        team.insert(robot);
        team.listRobots();
        cout << endl;

        cout << "Testing many insertions: " << endl;
        vector<int> idNumbers;
        for (int i = 0; i < 500; i++) {
            ID = idGen.getRandNum();
            idNumbers.push_back(ID);
            Robot robot(ID, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        cout << "\nDump after inserting 500 additional nodes:\n\n";
        team.dumpTree();
        cout << "\n\nList of Robots after inserting 500 more nodes:\n";
        team.listRobots();
        cout << endl;


        cout << "Check heights of each Robot to test if tree is balanced: " << endl;
        tester.imbalanceCheck(team);


        cout << endl << "Testing and checking that BST property is satisfied: " << endl;
        bool isBST = tester.isBST(team);
        if (isBST) {
            cout << "BST property is satisfied." << endl;
        } else {
            cout << "BST property is NOT satisfied. " << endl;
        }


        cout << endl <<"Testing removal of 10 items: " << endl << endl;
        for (int i = 0; i < 10; i++){
            team.remove(idNumbers[i]);
        }

        Swarm runTimeTeam;
        int id;
        int insertNum = 200000;
        const int a = 2;//scaling factor for input size, everytime it will double
        double T = 0.0;//to store running times
        clock_t start, stop;//stores the clock ticks while running the program
        start = clock();
        for (int k=0; k<insertNum; k++) {
            id = idGen.getRandNum();
            Robot bot(id, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            runTimeTeam.insert(bot);
        }
        stop = clock();
        T = stop - start;
        cout << "time it takes to insert " << insertNum << " robots: " << T << " clock ticks." << endl;

        insertNum *= a;
        start = clock();
        for (int k=0; k<insertNum; k++) {
            id = idGen.getRandNum();
            Robot bot(id, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            runTimeTeam.insert(bot);
        }
        stop = clock();
        T = stop - start;
        cout << "time it takes to insert " << insertNum << " robots: " << T << " clock ticks." << endl;

        insertNum *= a;
        start = clock();
        for (int k=0; k<insertNum; k++) {
            id = idGen.getRandNum();
            Robot bot(id, static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            runTimeTeam.insert(bot);
        }
        stop = clock();
        T = stop - start;
        cout << "time it takes to insert " << insertNum << " robots: " << T << " clock ticks." << endl;

        return 0;
    }
}
