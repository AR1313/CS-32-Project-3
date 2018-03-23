#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
#include<math.h>
#include <iostream>

#include <sstream>  // defines the type std::ostringstream
#include <iomanip>  // defines the manipulator setw


class Actor;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    ~StudentWorld();

    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(const Actor* a) const;
    
    // If the player has collided with a, return a pointer to the player;
    // otherwise, return a null pointer.
    NachenBlaster* getCollidingPlayer(const Actor* a) const;
    
    // Is the player in the line of fire of a, which might cause a to attack?
    bool playerInLineOfFire(const Actor* a) const;
    
    // Add an actor to the world.
    void addActor(Actor* a);
    
    // Record that one more alien on the current level has been destroyed.
    void recordAlienDestroyed();
    
    void introduceAnyNewActors();
    
    void deleteAnyDeadActors();
    
    int remainingAlienShipstoDestroyOnThisLevel();

    int maxAlienShipsOnScreen();
    
    std::string whichAlienShipShouldiAdd();
    
    double euclideanDistance(int x1, int y1, int x2, int y2) const;
    
    NachenBlaster* getPlayer();
    
    std::string updateDisplayText();
    
    
private:
    std::list<Actor*> m_actors;
    NachenBlaster* m_NachenBlaster;
    int m_aliensDestroyed;
    int m_alienShipsOnScreen;
    int m_score;
};

#endif // STUDENTWORLD_H_
