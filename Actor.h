#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include<math.h>

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;
const int LEFT=2;
const int UP_AND_LEFT=3;
const int DOWN_AND_LEFT=4;

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* w, double startX, double startY, int imageID,
          int imageDir, double size, int depth);
    
    // Action to perform for each tick.
    virtual void doSomething() = 0;
    
    // Is this actor dead?
    bool isDead() const;
    
    // Can this actor collide with a player-fired projectile?
    // (Note:  We originally called this isAlien, but it's not so much
    // _what_ it is that's important, but what you can _do_ to it.)
    virtual bool collidableWithPlayerFiredProjectile() const;
    
    // Get this actor's world
    StudentWorld* world() const;
    
    // Mark this actor as dead.
    void setDead();
    
    // Move this actor to x,y if onscreen; otherwise, don't move and mark
    // this actor as dead.
    virtual void moveTo(double x, double y);
    
    virtual void sufferDamage(double amt, int cause) = 0;
    
private:
    StudentWorld* m_world;
    bool m_isDead;
};

//////////////////////////////////////////////




//////////////////////////////////////////////

class Star : public Actor
{
public:
    Star(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};


//////////////////////////////////////////////




//////////////////////////////////////////////
class Explosion : public Actor
{
public:
    Explosion(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
    
private:
    int m_count;
};



//////////////////////////////////////////////




//////////////////////////////////////////////


class DamageableObject : public Actor
{
public:
    DamageableObject(StudentWorld* w, double startX, double startY, int imageID,
                     int startDir, double size, int depth, double hitPoints);
    
    // How many hit points does this actor have left?
    double hitPoints() const;
    
    // Increase this actor's hit points by amt.
    virtual void increaseHitPoints(double amt);
    
    // This actor suffers an amount of damage caused by being hit by either
    // a ship or a projectile (see constants above).
    virtual void sufferDamage(double amt, int cause);
    
private:
    
    double m_hitpoints;
};

class NachenBlaster : public DamageableObject
{
public:
    NachenBlaster(StudentWorld* w);
    
    virtual void doSomething();

    virtual void increaseHitPoints(double amt);
    
    virtual void sufferDamage(double amt, int cause);
    
    // Incease the number of torpedoes the player has by amt.
    void increaseTorpedoes(int amt);
    
    // Get the player's health percentage.
    int healthPct() const;
    
    // Get the player's cabbage energy percentage.
    int cabbagePct() const;
    
    // Get the number of torpedoes the player has.
    int numTorpedoes() const;
    
private:
    int m_torpedoCount;
    int m_cabbageCount;
};

class Alien : public DamageableObject
{
public:
    Alien(StudentWorld* w, double startX, double startY, int imageID,
          double hitPoints, double damageAmt, double deltaX,
          double deltaY, double speed, unsigned int scoreValue);
    
    virtual bool collidableWithPlayerFiredProjectile() const;
    
    virtual void sufferDamage(double amt, int cause);
    
    // Move the player by the current speed in the direction indicated
    // by the x and y deltas.
    void move();
    
    // Set the player's y direction.
    void setDeltaY(double dy);
    
    // Set the player's speed.
    void setSpeed(double speed);
    
    // If this alien collided with the player, damage the player and return
    // true; otherwise, return false.
    virtual bool damageCollidingPlayer(double amt);
    
    // If this alien drops goodies, drop one with the appropriate probability.
    virtual void possiblyDropGoodie();
    
    void setDirection(int dir);
    
private:
    double m_hitPoints;
    double m_damageAmt;
    double m_deltaX;
    double m_deltaY;
    double m_speed;
    unsigned int m_scoreValue;
    int m_direction;
};

class Smallgon : public Alien
{
public:
    Smallgon(StudentWorld* w, double startX, double startY, int FlightPlanLength);
    virtual void doSomething();
    
private:
    int m_FlightPlanLength;

};

class Smoregon : public Alien
{
public:
    Smoregon(StudentWorld* w, double startX, double startY, int FlightPlanLength);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause);
    
private:
    int m_FlightPlanLength;

};

class Snagglegon : public Alien
{
public:
    Snagglegon(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause);
};

//////////////////////////////////////////////




//////////////////////////////////////////////

class Projectile : public Actor
{
public:
    Projectile(StudentWorld* w, double startX, double startY, int imageID,
               double damageAmt, double deltaX, bool rotates, int imageDir);
    
private:
    double m_damageAmt;
    double m_deltaX;
    bool m_rotates;
};

class Cabbage : public Projectile
{
public:
    Cabbage(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

class Turnip : public Projectile
{
public:
    Turnip(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

class Torpedo : public Projectile
{
public:
    Torpedo(StudentWorld* w, double startX, double startY, double deltaX,
            int imageDir);
};

class PlayerLaunchedTorpedo : public Torpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

class AlienLaunchedTorpedo : public Torpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

//////////////////////////////////////////////




//////////////////////////////////////////////

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* w, double startX, double startY, int imageID);
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

class RepairGoodie : public Goodie
{
public:
    RepairGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};

class TorpedoGoodie : public Goodie
{
public:
    TorpedoGoodie(StudentWorld* w, double startX, double startY);
    virtual void doSomething();
    virtual void sufferDamage(double amt, int cause)
    {
        
    }
};




//////////////////////////////////////////////




//////////////////////////////////////////////


#endif // ACTOR_H_
