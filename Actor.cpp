#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp



Actor:: Actor(StudentWorld* w, double startX, double startY, int imageID,
          int imageDir, double size, int depth)
: GraphObject(imageID, startX, startY, imageDir, size, depth)
{
    m_world=w;
    m_isDead=false;
}

    // Is this actor dead?
bool Actor:: isDead() const
{
    return m_isDead;
}
    
    // Can this actor collide with a player-fired projectile?
    // (Note:  We originally called this isAlien, but it's not so much
    // _what_ it is that's important, but what you can _do_ to it.)
bool Actor:: collidableWithPlayerFiredProjectile() const
{
    return false;
}
    
    // Get this actor's world
StudentWorld* Actor:: world() const
{
    return m_world;
}
    
    // Mark this actor as dead.
void Actor:: setDead()
{
    m_isDead=true;
}
    
    // Move this actor to x,y if onscreen; otherwise, don't move and mark
    // this actor as dead.
void Actor:: moveTo(double x, double y)
{
    if (!isDead())
        GraphObject::moveTo(x, y);
}

//////////////////////////////////////////////




//////////////////////////////////////////////

Star:: Star(StudentWorld* w,double startX, double startY)
:Actor(w, startX, startY, IID_STAR,
       0, 0, 3)
{
    double starSize= (randInt(5, 50))/100.0;
    setSize(starSize);//A star has a randomly chosen size in the range .05 and .50.
}

  void  Star:: doSomething()
{
    moveTo(getX()-1, getY());
    if (getX()<0)
        setDead();
}


//////////////////////////////////////////////




//////////////////////////////////////////////

Explosion:: Explosion(StudentWorld* w, double startX, double startY)
:Actor(w, startX, startY, IID_EXPLOSION,
       0, 1, 0)
{
    m_count=4;
}
void Explosion:: doSomething()
{
    if (m_count==0)
        setDead();
    else
    {
    setSize(1.5*getSize());
    m_count--;
    }
}




//////////////////////////////////////////////




//////////////////////////////////////////////



DamageableObject:: DamageableObject(StudentWorld* w, double startX, double startY, int imageID,
                     int startDir, double size, int depth, double hitPoints)
:Actor(w, startX, startY, imageID, startDir, size, depth)
{
    m_hitpoints=hitPoints;
}
    
    // How many hit points does this actor have left?
    double DamageableObject:: hitPoints() const
{
    return m_hitpoints;
}
    
    // Increase this actor's hit points by amt.
void DamageableObject:: increaseHitPoints(double amt)
{
    m_hitpoints+= amt;
}
    
    // This actor suffers an amount of damage caused by being hit by either
    // a ship or a projectile (see constants above).
  void DamageableObject:: sufferDamage(double amt, int cause)
{
    
}

NachenBlaster:: NachenBlaster(StudentWorld* w)
:DamageableObject(w, 0, 128, IID_NACHENBLASTER,
                  0, 1, 0, 50)
{
    m_torpedoCount=0;
    m_cabbageCount=30;
}


void NachenBlaster:: doSomething()
{
    if (isDead())
        return;
    
    int value;
    if (world()->getKey(value))
    {
        switch(value)
        {
            case KEY_PRESS_SPACE:
                if (m_cabbageCount >= 5)
                {
                    world()->addActor(new Cabbage(world(), getX()+12, getY()));
                    m_cabbageCount-=5;
                    world()->playSound(SOUND_PLAYER_SHOOT);
                }
                break;
                
            case KEY_PRESS_TAB:
                    if (m_torpedoCount > 0)
                    {
                        world()->addActor(new PlayerLaunchedTorpedo(world(), getX()+12, getY()));
                        m_torpedoCount-=1;
                        world()->playSound(SOUND_TORPEDO);
                    }
                break;
                
            case KEY_PRESS_UP:
                if (getY() < VIEW_HEIGHT-6)
                    moveTo(getX(), getY()+6);
                    break;
                
            case KEY_PRESS_DOWN:
                if (getY() >= 6)
                    moveTo(getX(), getY()-6);
                    break;
                
            case KEY_PRESS_RIGHT:
                if (getX() < VIEW_WIDTH-6)
                    moveTo(getX()+6, getY());
                    break;
                
            case KEY_PRESS_LEFT:
                if (getX() >= 6)
                    moveTo(getX()-6, getY());
                    break;
                
                // case KEY_PRESS_ESCAPE:
                
        }
    }
    
//    if ( hitPoints() <=0 )
//        setDead();
   
    if (m_cabbageCount<30)
      m_cabbageCount++;
    
    
}


     void NachenBlaster:: increaseHitPoints(double amt)
{
    if (amt+hitPoints() <= 50)
        DamageableObject::increaseHitPoints(amt);
   
    if ( hitPoints() <=0 )
        setDead();
}

    void NachenBlaster:: sufferDamage(double amt, int cause)
{
   
    switch (cause)
    {
        case HIT_BY_SHIP:
            increaseHitPoints( ( amt * -1) );
            //std:: cout<<"NachenBlaster hit points: "<< hitPoints()<< std::endl;
            break;
       
        case HIT_BY_PROJECTILE:
            increaseHitPoints( ( amt * -1) );
            world()->playSound(SOUND_BLAST);
            break;
    }
}

    // Incease the number of torpedoes the player has by amt.
    void NachenBlaster:: increaseTorpedoes(int amt)
{
    m_torpedoCount+=amt;
}
    
    // Get the player's health percentage.
    int NachenBlaster:: healthPct() const
{
    return ((hitPoints()/50)*100);
}
    
    // Get the player's cabbage energy percentage.
    int NachenBlaster:: cabbagePct() const
{
    return ((m_cabbageCount/30)*100);
}
    // Get the number of torpedoes the player has.
    int NachenBlaster:: numTorpedoes() const
    {
        return m_torpedoCount;
    }


    Alien:: Alien(StudentWorld* w, double startX, double startY, int imageID,
          double hitPoints, double damageAmt, double deltaX,
          double deltaY, double speed, unsigned int scoreValue)
:DamageableObject(w, startX, startY, imageID, 0, 1.5, 1, hitPoints)
    {
        m_hitPoints=hitPoints;
        m_damageAmt=damageAmt;
        m_deltaX=deltaX;
        m_deltaY=deltaY;
        m_speed=speed;
        m_scoreValue=scoreValue;
    }
    
    bool Alien:: collidableWithPlayerFiredProjectile() const
    {
        return true;
    }
    
    void Alien:: sufferDamage(double amt, int cause)
    {
        if (cause==HIT_BY_SHIP)
        {
            setDead();
            world()->addActor(new Explosion(world(), getX(), getY()));
           world()->playSound(SOUND_DEATH);
            world()-> recordAlienDestroyed();
        }
        
        else
        {
            increaseHitPoints( ( amt * -1) );
            if (hitPoints()<=0)
        {
                world()->increaseScore(250);
                setDead();
                world()->addActor(new Explosion(world(), getX(), getY()));
                world()->playSound(SOUND_DEATH);
                world()-> recordAlienDestroyed();
            }
           
            else
                world()->playSound(SOUND_BLAST);
        }
    }
    
    // Move the player by the current speed in the direction indicated
    // by the x and y deltas.
    void Alien:: move()
    {
        switch (m_direction)
        {
            case LEFT:
                moveTo(getX()-m_speed, getY());
                break;
                
            case UP_AND_LEFT:
                moveTo(getX()-m_speed, getY()+m_deltaY);
                break;
                
            case DOWN_AND_LEFT:
                moveTo(getX()-m_speed, getY()-m_deltaY);
                break;
        }
    }
    
    // Set the player's y direction.
    void  Alien:: setDeltaY(double dy)
    {
        m_deltaY=dy;
    }
    
    // Set the player's speed.
    void Alien:: setSpeed(double speed)
    {
        m_speed=speed;
    }
    
    // If this alien collided with the player, damage the player and return
    // true; otherwise, return false.
   bool Alien:: damageCollidingPlayer(double amt)
    {
        return true;
    }
    
    // If this alien drops goodies, drop one with the appropriate probability.
  void Alien:: possiblyDropGoodie()
    {
        
    }


void Alien:: setDirection(int dir)
{
    m_direction=dir;
}



Smallgon:: Smallgon(StudentWorld* w, double startX, double startY, int FlightPlanLength)
   :Alien(w, startX, startY, IID_SMALLGON, 5*(1+((w->getLevel())-1) * 0.1), 5.0, 2, 2, 2.0, 250) /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    {
          m_FlightPlanLength=FlightPlanLength;
    }

    void Smallgon:: doSomething()
    {
        if (isDead())
            return;
        
        if (getX()<0)
        {
            setDead();
            return;
        }
        
        NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
            collidingPlayer->sufferDamage(5, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(250);
        }
        
        if (m_FlightPlanLength==0 || getY()>=VIEW_HEIGHT-1 || getY()<=0)
        {
            if (getY()>=VIEW_HEIGHT-1)
            {
                setDirection(DOWN_AND_LEFT);
            }
            else if (getY()<=0)
            {
                setDirection(UP_AND_LEFT);
            }
            else if (m_FlightPlanLength==0)
            {
                int r= randInt(1, 3);
                switch (r)
                {
                    case 1:
                        setDirection(LEFT);
                        break;
                    case 2:
                        setDirection(DOWN_AND_LEFT);
                        break;
                    case 3:
                        setDirection(UP_AND_LEFT);
                        break;
                }
                
            }
            m_FlightPlanLength= randInt(1, 32);
        }
        
        if ( world()->getPlayer()->getX() < getX() && std::abs(getY()-(world()->getPlayer()->getY())) <= 4 )
        {
            int rand= randInt(1, ((20/(world()->getLevel()))+5));
            if (rand == 1 )
            {
                world()->addActor(new Turnip(world(), getX()-14, getY()));
                world()->playSound(SOUND_ALIEN_SHOOT);
                return;
            }
        }
        
        move();
        m_FlightPlanLength--;
    
        collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
            collidingPlayer->sufferDamage(5, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(250);
        }
    }

    Smoregon:: Smoregon(StudentWorld* w, double startX, double startY, int FlightPlanLength)
: Alien(w, startX, startY,IID_SMOREGON,
        5*(1+((w->getLevel())-1) * 0.1), 5.0 , 2 , 2 , 2.0 , 250)
    {
        m_FlightPlanLength=FlightPlanLength;
    }
    void Smoregon:: doSomething()
    {
        if (isDead())
            return;
        
        if (getX()<0)
        {
            setDead();
            return;
        }
        
        NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
       
            collidingPlayer->sufferDamage(5, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(250);
        }
        
        if (m_FlightPlanLength==0 || getY()>=VIEW_HEIGHT-1 || getY()<=0)
        {
            if (getY()>=VIEW_HEIGHT-1)
            {
                setDirection(DOWN_AND_LEFT);
            }
            else if (getY()<=0)
            {
                setDirection(UP_AND_LEFT);
            }
            else if (m_FlightPlanLength==0)
            {
                int r= randInt(1, 3);
                switch (r)
                {
                    case 1:
                        setDirection(LEFT);
                        break;
                    case 2:
                        setDirection(DOWN_AND_LEFT);
                        break;
                    case 3:
                        setDirection(UP_AND_LEFT);
                        break;
                }
                
            }
            m_FlightPlanLength= randInt(1, 32);
        }

        if ( world()->getPlayer()->getX() < getX() && std::abs(getY()-(world()->getPlayer()->getY())) <= 4 )
        {
            int rand= randInt(1, ((20/(world()->getLevel()))+5));
            if (rand == 1 )
            {
                world()->addActor(new Turnip(world(), getX()-14, getY()));
                world()->playSound(SOUND_ALIEN_SHOOT);
                return;
            }
            
            int rand2= randInt(1, ((20/(world()->getLevel()))+5));
            if (rand2 == 1)
            {
                setDirection(LEFT);
                m_FlightPlanLength = VIEW_WIDTH;
                setSpeed(5);
                setDeltaY(5);
            }
        }
        
        move();
        m_FlightPlanLength--;

        collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
            
            collidingPlayer->sufferDamage(5, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(250);
        }
    }

 void Smoregon:: sufferDamage(double amt, int cause)
{
    if (cause==HIT_BY_SHIP)
    {
        setDead();
        world()->addActor(new Explosion(world(), getX(), getY()));
        world()->playSound(SOUND_DEATH);
        world()-> recordAlienDestroyed();
    
        int rand= randInt(1, 3);
        if (rand == 1)
        {
            rand=randInt(1, 2);
            if (rand == 1)
            {
                world()->addActor(new RepairGoodie(world(), getX(), getY()));
            }
            else
            {
                world()->addActor(new TorpedoGoodie(world(), getX(), getY()));
            }
        }
    }
    
    else
    {
        increaseHitPoints( ( amt * -1) );
        if (hitPoints()<=0)
        {
            world()->increaseScore(250);
            setDead();
           world()->playSound(SOUND_DEATH);
            world()->addActor(new Explosion(world(), getX(), getY()));
            world()-> recordAlienDestroyed();
            
            int rand= randInt(1, 3);
            if (rand == 1)
            {
                rand=randInt(1, 2);
                if (rand == 1)
                {
                    world()->addActor(new RepairGoodie(world(), getX(), getY()));
                }
                            else
                            {
                                world()->addActor(new TorpedoGoodie(world(), getX(), getY()));
                
                            }
            }
        }
        
        else
          world()->playSound(SOUND_BLAST);
    }
}

    Snagglegon:: Snagglegon(StudentWorld* w, double startX, double startY)
: Alien(w, startX, startY, IID_SNAGGLEGON,
      10*(1+((w->getLevel())-1) * 0.1), 15, 1.75,
      1.75, 1.75, 1000)
    {
        setDirection(DOWN_AND_LEFT);
    }
    
    void Snagglegon:: doSomething()
    {
        if (isDead())
            return;
        
        if (getX()<0)
        {
            setDead();
            return;
        }
        
        NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
            collidingPlayer->sufferDamage(15, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(1000);
        }
        
        if (getY()>=VIEW_HEIGHT-1 || getY()<=0)
        {
            if (getY()>= VIEW_HEIGHT-1)
                setDirection(DOWN_AND_LEFT);
            else
                setDirection(UP_AND_LEFT);
        }
        
        
        if ( world()->getPlayer()->getX() < getX() && std::abs(getY()-(world()->getPlayer()->getY())) <= 4 )
        {
            int rand= randInt(1, ((15/(world()->getLevel()))+10));
            if (rand == 1 )
            {
                world()->addActor(new AlienLaunchedTorpedo(world(), getX()-14, getY()));
                world()->playSound(SOUND_TORPEDO);
                return;
            }
        }
        
        move();
        
        collidingPlayer= world()->getCollidingPlayer(this);
        if (collidingPlayer != nullptr)
        {
            collidingPlayer->sufferDamage(15, HIT_BY_SHIP);
            sufferDamage(0, HIT_BY_SHIP);
            world()->increaseScore(1000);
        }

    }


 void Snagglegon:: sufferDamage(double amt, int cause)
{
    if (cause==HIT_BY_SHIP)
    {
        setDead();
        world()->addActor(new Explosion(world(), getX(), getY()));
        world()->playSound(SOUND_DEATH);
        world()-> recordAlienDestroyed(); 
        
        int rand = randInt(1, 6);
        if (rand == 1)
        {
            world()->addActor(new ExtraLifeGoodie(world(), getX(), getY()));
        }
    }
    
    else
    {
        increaseHitPoints( ( amt * -1) );
        if (hitPoints()<=0)
        {
            world()->increaseScore(1000);
            setDead();
            world()->playSound(SOUND_DEATH);
            world()->addActor(new Explosion(world(), getX(), getY()));
            world()-> recordAlienDestroyed();
            
            int rand = randInt(1, 6);
            if (rand == 1)
            {
                world()->addActor(new ExtraLifeGoodie(world(), getX(), getY()));
            }
        }
        
        else
            world()->playSound(SOUND_BLAST);
    }
}
//////////////////////////////////////////////




//////////////////////////////////////////////

Projectile:: Projectile(StudentWorld* w, double startX, double startY, int imageID,
               double damageAmt, double deltaX, bool rotates, int imageDir)
:Actor(w, startX, startY, imageID,
      imageDir, 0.5 , 1)
{
    m_damageAmt= damageAmt;
    m_deltaX=deltaX;
    m_rotates=rotates;
}


Cabbage:: Cabbage(StudentWorld* w, double startX, double startY)
:Projectile(w, startX, startY, IID_CABBAGE,
            2.0 , 8, true, 0)
{
    
}

void Cabbage:: doSomething()
{
    if (isDead())
        return;
    
    if (getX()>=VIEW_WIDTH) //If it flew off the screen
    {
        setDead();
        return;
    }
    Actor* collidingAlien= world()->getOneCollidingAlien(this);
    if (collidingAlien != nullptr)
    {
     
        collidingAlien->sufferDamage(2, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
    
    moveTo(getX()+8, getY());
    
    //The cabbage must rotate its direction by 20 degrees counter-clockwise.////////////////////////////////////////////
    
    collidingAlien= world()->getOneCollidingAlien(this);
    if (collidingAlien != nullptr)
    {
        collidingAlien->sufferDamage(2, HIT_BY_PROJECTILE);
        setDead();
    }

}


Turnip::Turnip(StudentWorld* w, double startX, double startY)
:Projectile(w, startX, startY, IID_TURNIP,
            2, -6, true, 0)
{
    
}

void Turnip:: doSomething()
{
    if (isDead())
        return;
    
    if (getX()>=VIEW_WIDTH) //If it flew off the screen
    {
        setDead();
        return;
    }
    NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        collidingPlayer->sufferDamage(2, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
    
     moveTo(getX()-6, getY());
    

    //The turnip must rotate its direction by 20 degrees counter-clockwise.////////////////////////////////////////////
   
    collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        collidingPlayer->sufferDamage(2, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
}



Torpedo:: Torpedo(StudentWorld* w, double startX, double startY, double deltaX,
            int imageDir)
:Projectile(w, startX, startY, IID_TORPEDO,
             8, 8, false, 0)
{
    
}



PlayerLaunchedTorpedo:: PlayerLaunchedTorpedo(StudentWorld* w, double startX, double startY)
:Torpedo(w, startX, startY, 8, 0)

{
    
}

 void PlayerLaunchedTorpedo:: doSomething()
{
    if (isDead())
        return;
    
    if (getX() >= VIEW_WIDTH)
    {
        setDead();
        return;
    }
    
    Actor* collidingAlien= world()->getOneCollidingAlien(this);
    if (collidingAlien != nullptr)
    {
        collidingAlien->sufferDamage(8, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
    
    moveTo(getX()+8, getY());
    
    collidingAlien= world()->getOneCollidingAlien(this);
    if (collidingAlien != nullptr)
    {
        collidingAlien->sufferDamage(8, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
}


AlienLaunchedTorpedo:: AlienLaunchedTorpedo(StudentWorld* w, double startX, double startY)
:Torpedo(w, startX, startY, -8 , 180)

{
    
}

 void AlienLaunchedTorpedo ::doSomething()
{
    if (isDead())
        return;
    
    if (getX() < 0)
    {
        setDead();
        return;
    }
    
    NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        collidingPlayer->sufferDamage(8, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
    
    moveTo(getX()-8, getY());
 
    collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        collidingPlayer->sufferDamage(8, HIT_BY_PROJECTILE);
        setDead();
        return;
    }
}


//////////////////////////////////////////////




//////////////////////////////////////////////


Goodie:: Goodie(StudentWorld* w, double startX, double startY, int imageID)
:Actor(w, startX, startY, imageID, 0, 0.5, 1)
{
    
}


ExtraLifeGoodie::  ExtraLifeGoodie(StudentWorld* w, double startX, double startY)
:Goodie (w, startX, startY, IID_LIFE_GOODIE)
{
}

void ExtraLifeGoodie:: doSomething()
{
    if (isDead())
        return;
    
    if (getX() < 0 || getX()>= VIEW_WIDTH)
    {
        setDead();
        return;
    }
    
    NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
        world()->playSound(SOUND_GOODIE);
        world()->incLives();
        return;
    }
    
    moveTo(getX()-0.75, getY()-0.75);
    
    collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
       world()->playSound(SOUND_GOODIE);
        world()->incLives();
        return;
    }
}


RepairGoodie:: RepairGoodie(StudentWorld* w, double startX, double startY)
:Goodie (w, startX, startY, IID_REPAIR_GOODIE)
{
    
}

void RepairGoodie:: doSomething()
{
    
    if (isDead())
        return;
    
    if (getX() < 0 || getX()>= VIEW_WIDTH)
    {
        setDead();
        return;
    }

    NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
        world()->playSound(SOUND_GOODIE);
        world()->getPlayer()->increaseHitPoints(10);
        return;
    }
    
     moveTo(getX()-0.75, getY()-0.75);
    
    collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
        world()->playSound(SOUND_GOODIE);
        world()->getPlayer()->increaseHitPoints(10);
        return;
    }
}


TorpedoGoodie:: TorpedoGoodie(StudentWorld* w, double startX, double startY)
:Goodie (w, startX, startY, IID_TORPEDO_GOODIE)
{
    
}

void TorpedoGoodie:: doSomething()
{
    if (isDead())
        return;
    
    if (getX() < 0 || getX()>= VIEW_WIDTH)
    {
        setDead();
        return;
    }
    
    NachenBlaster* collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
       world()->playSound(SOUND_GOODIE);
        world()->getPlayer()->increaseTorpedoes(5);
        return;
    }
    
    moveTo( getX()-0.75, getY()-0.75);

    collidingPlayer= world()->getCollidingPlayer(this);
    if (collidingPlayer != nullptr)
    {
        world()->increaseScore(100);
        setDead();
       world()->playSound(SOUND_GOODIE);
        world()->getPlayer()->increaseTorpedoes(5);
        return;
    }
};


















