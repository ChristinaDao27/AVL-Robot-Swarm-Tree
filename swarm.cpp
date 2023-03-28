//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2
//Title: swarm.cpp
//Author: Christina Dao
//Date: 10/26/2021
//Description: This file Holds the bodies of my functions for Proj2
//             and contains most of the program
//E-mail: cdao1@umbc.edu
//Course Section: CMSC 341 Section 03

#include "swarm.h"
// Name: Swarm::Swarm()
// Desc: default constructor set root to nullptr
Swarm::Swarm(){
    m_root = nullptr;
}

// Name: Swarm::~Swarm()
// Desc: deconstructor deallocates all memory
// PostCondition: all Memory is deallocated
Swarm::~Swarm(){
    clear();
}

// Name: Swarm::insert(const Robot& robot)
// Desc: calls the recursive insert and passes in the desired robot a
//       and the root of the swarm
// Postcondition: Robot is inserted in the correct position
void Swarm::insert(const Robot& robot) {
    m_root = insertRecursively(robot, m_root);
}

// Name: Swarm::insertRecursively(const Robot& insertBot, Robot *currNode)
// Desc: recursively finds the correct location to insert the desired node
//       and updates the heights of the nodes and calls rebalance to keep AVL tree structure
Robot* Swarm::insertRecursively(const Robot& insertBot, Robot *currNode){
    // location on tree is nullptr and available for the new robot to be placed
    if (!currNode){
        // created new bot
        Robot *newRobot = new Robot(insertBot.getID(),insertBot.getType(),insertBot.getState());
        return newRobot;
    }
    // InsertBot is smaller than Node
    if (insertBot.getID() < currNode->getID()) {
        // recursive call
        currNode->setLeft(insertRecursively(insertBot, currNode->getLeft()));
        // update height and rebalance
        updateHeight(currNode);
        return rebalance(currNode);
    }
    // InsertBot is greater than Node
    else if (insertBot.getID() > currNode->getID()){
        // Recursive call
        currNode->setRight(insertRecursively(insertBot, currNode->getRight()));
        // update height and rebalance
        updateHeight(currNode);
        return rebalance(currNode);
    }
    // new robot is equal ro the node it is at.
    else {
        return currNode;
    }
}


// Name: Swarm::clear()
// Desc: aids in deallocating memory by passing the root
void Swarm::clear(){
    clearHelper(m_root);
}

//Name: Swarm::clearHelper(Robot* aBot)
// Desc: recursively deallocates memory
// Postcondition: memory is deallocated
void Swarm::clearHelper(Robot* aBot){
    if (aBot) {
        // recursively calls
        clearHelper(aBot->getLeft());
        clearHelper(aBot->getRight());
        delete aBot;
    }
}

// Name: Swarm::remove(int id)
// Desc: passes in root and desired robot id to removeHelper to aid in bot removal.
void Swarm::remove(int id){
    removeHelper(m_root, id);
}

// Name: Swarm::removeHelper(Robot* aBot, int ID)
// Desc: recursive remove helper function
// PostCondition: desired bot is removed recursively and tree is rebalanced
//                and heights are updated to satisfy AVL conditions
Robot* Swarm::removeHelper(Robot* aBot, int ID){
    Robot* tempBot;
    // Robot does not exist
    if (aBot == nullptr){
        return nullptr;
    }
    // Check if the desired ID is greater than current Robot Node
    else if (ID > aBot->getID()){
        // recursive call to continue searching for Robot wit ID
        aBot->setRight(removeHelper(aBot->getRight(), ID));
    }
    // Check if the desired ID is less than the current Robot Node
    else if (ID < aBot->getID()){
        // recursive call to continue searching for Robot wit ID
        aBot->setLeft(removeHelper(aBot->getLeft(), ID));
    }
    // current robot has two children
    else if (aBot->getRight() && aBot->getLeft()){
        // find the smallest value on the right side of aBot and change current Robot's
        // ID to that value, and then remove
        tempBot = findSmallestRight(aBot->getRight());
        aBot->setID(tempBot->getID());
        aBot->setRight(removeHelper(aBot->getRight(), ID));
    }
    // current robot has zero or one child
    else{
        tempBot = aBot;
        if (aBot->getLeft() == nullptr) {
            aBot = aBot->getRight();
        }
        else if (aBot->getRight() == nullptr){
            aBot = aBot->getLeft();
        }
        delete tempBot;
    }
    // maintain AVL conditions
    updateHeight(aBot);
    aBot = rebalance(aBot);
    return aBot;
}

// Name: findSmallestRight(Robot* aBot)
// Desc: finds smallest Robot on the right side of aBot
// postcondition: smallest right robot is returned
Robot* Swarm::findSmallestRight(Robot* aBot){
    if (!aBot || !aBot->getLeft()){
        return aBot;
    }
    else{
        return maxBot(aBot->getRight());
    }
}

// Name: Swarm::maxBot(Robot* aBot)
// Desc: finds the maxrobot of tree
Robot* Swarm::maxBot(Robot* aBot){
    if (!aBot || !aBot->getRight()){
        return aBot;
    }
    else{
        return maxBot(aBot->getRight());
    }
}

// Name: Swarm::updateHeight(Robot* aBot)
// Desc: updates heights of robot that was passed
// Postcondition: robot height is up to date
void Swarm::updateHeight(Robot* aBot){
    // a leaf node with no left or right
    if (!aBot->getRight() && !aBot->getLeft()) {
        aBot->setHeight(0);
    }
    // Node only has a right node
    if (aBot->getRight() && !aBot->getLeft()){
        aBot->setHeight(aBot->getRight()->getHeight() + 1);
    }
    // Node only has a left node
    if (aBot->getLeft() && !aBot->getRight()){
        aBot->setHeight(aBot->getLeft()->getHeight() + 1);
    }
    // Node has both left and right node so the node's height is 1+ it's tallest child
    if (aBot->getRight() && aBot->getLeft()){
        int right = aBot->getRight()->getHeight();
        int left = aBot->getLeft()->getHeight();
        aBot->setHeight((right > left) ? (right + 1) : (left + 1));
    }
}

// Name: Swarm::checkImbalance(Robot* aBot)
// Desc: Checks if there is an imbalance in a tree
// postcondition: returns the different in heights
int Swarm::checkImbalance(Robot* aBot){
    // Check if the Robot/Node being checked is a nullptr with height -1
    if (!aBot){
        return -1;
    }
    else{
        // Find heights of left and right subtrees
        int rHeight = -1;
        int lHeight = -1;
        if (aBot->getRight()){
            rHeight = aBot->getRight()->getHeight();
        }
        if (aBot->getLeft()){
            lHeight = aBot->getLeft()->getHeight();
        }
        // return the difference in height of subtrees
        return (lHeight - rHeight);
    }
}

// Name:  Swarm::rebalance(Robot* aBot)
// Desc: rebalances the tree by checking conditions
// Postcondition: tree is rebalanced at the node that was passed
Robot* Swarm::rebalance(Robot* aBot) {
    // Left rotation at z (aBot) needed
    if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->getRight()) <= 0)) {
        return rotateL(aBot);
    }
        // Right rotation at z (aBot) needed
    else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->getLeft()) >= 0)) {
        return rotateR(aBot);
    }
        // right rotation at aBot->getRight(), then left rotation at aBot
    else if ((checkImbalance(aBot) < -1) && (checkImbalance(aBot->getRight()) >= 0)) {
        aBot->setRight(rotateR(aBot->getRight()));
        return rotateL(aBot);
    }
        // left rotation at aBot->getleft, then right rotation at aBot
    else if ((checkImbalance(aBot) > 1) && (checkImbalance(aBot->getLeft()) <= 0)) {
        aBot->setLeft(rotateL(aBot->getLeft()));
        return rotateR(aBot);
    } else {
        return aBot;
    }
}

// Name: Swarm::rotateL(Robot* aBot)
// Desc: rotates the nodes leftward
// Postcondition: nodes are rotated to the left
Robot* Swarm::rotateL(Robot* aBot){
    Robot* z = aBot;
    Robot* y = z->getRight();
    z->setRight(y->getLeft());
    y->setLeft(z);

    // heights must be updated after rotation
    updateHeight(z);
    updateHeight(y);
    return y;
}

//  Name:  Swarm::rotateR(Robot* aBot)
// Desc: rotates the nodes rightward
// Postcondition: nodes are rotated to the right
Robot* Swarm::rotateR(Robot* aBot){
    Robot* z = aBot;
    Robot* y = z->getLeft();
    z->setLeft(y->getRight());
    y->setRight(z);

    // heights must be updated after rotation
    updateHeight(z);
    updateHeight(y);
    return y;
}

// Name: Swarm::listRobots() const
// Name: Swarm::listRobots() const
// Postcondition: robots are displayed with their information
void Swarm::listRobots() const {
    listHelper(m_root);
}

// Name: Swarm::listHelper(Robot* aBot) const
// Desc: function recursively displays the robots and their information
// Postcondition: robots are displayed with their information
void Swarm::listHelper(Robot* aBot) const{
    if (aBot){
        listHelper(aBot->getLeft());
        cout << aBot->getID() << ":" << aBot->getStateStr() << ":" << aBot->getTypeStr() << endl;
        listHelper(aBot->getRight());
    }
}

// Name: Swarm::setState(int id, STATE state)
// Desc: changes the state of the bot
// Postcondition: true or false based on if the robots state was changed
bool Swarm::setState(int id, STATE state){
    // Robot must exist
    if (findBot(id)){
        Robot* currBot = findHelper(m_root, id);
        currBot->setState(state);
        return true;
    }
    else{
        cout << "Robot with this ID does not exist" << endl;
        return false;
    }
}

// Name:  Swarm::removeDead()
// Desc: passes in the root to a helper function
// Dead robots are removed from tree
void Swarm::removeDead(){
    deadHelper(m_root);
}

// Name: Swarm::deadHelper(Robot* aBot)
// Desc: helper function to remove dead bots recursively
// postconditions: dead bots are removed
void Swarm::deadHelper(Robot* aBot){
    if (aBot){
        deadHelper(aBot->getLeft());
        if (aBot->getState() == DEAD){
            remove(aBot->getID());
        }
        deadHelper(aBot->getRight());
    }
}

// Name: Swarm::findBot(int id) const
// Desc: search for a bot with the desired ID and returns
//       true if it exists, or false if not
// Postcondition: bot is either found or doesnt exist
bool Swarm::findBot(int id) const {
    Robot* tempBot = findHelper(m_root, id);
    if (tempBot == nullptr){
        return false;
    }
    else{
        return true;
    }
}

// Name: Swarm::findHelper(Robot* aBot, int id) const
// Desc: helper function to recursively find a robot
// Description: desired robot is returned
Robot* Swarm::findHelper(Robot* aBot, int id) const{
    // The bot has been found
    if (!aBot || aBot->getID() == id){
        return aBot;
    }
    // go left
    else if (aBot->getID() > id){
        return findHelper(aBot->getLeft(), id);
    }
    // go right
    else{
        return findHelper(aBot->getRight(), id);
    }
}

// Given
void Swarm::dumpTree() const {
    dump(m_root);
}

// Given
void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}