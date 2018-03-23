#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir), m_NachenBlaster(nullptr)
{
    
}

int StudentWorld::init()
{
    m_aliensDestroyed=0;
    m_alienShipsOnScreen=0;

    m_NachenBlaster= new NachenBlaster(this);
   
    for (int i=0; i<30; i++)
    {
        m_actors.push_back(new Star(this, randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1)));
    }
    
    //std::cout<<"Level: "<<getLevel()<<endl;
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
   
    m_NachenBlaster->doSomething();
    
    list<Actor*>::iterator  it= m_actors.begin();
    //std::cout<<"level: "<<getLevel()<<std::endl;
    while (it!= m_actors.end())
    {
        (*it)->doSomething();
        it++;
        
        if (m_NachenBlaster->isDead())
        {
            decLives();/////////////////////////////////////////////////////////////////////////////////////////////////
            return GWSTATUS_PLAYER_DIED;
        }
        
        if (remainingAlienShipstoDestroyOnThisLevel()==0)
        {
            //increaseScoreAppropriately();
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    introduceAnyNewActors();
    deleteAnyDeadActors();
    
    setGameStatText(updateDisplayText());/////////////////////////////////////////////////////////////////////////////////
    
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::cleanUp()
{
        while (!m_actors.empty())
        {
            delete m_actors.back();
            m_actors.pop_back();
        }

            delete m_NachenBlaster;
    
}

StudentWorld::~StudentWorld()
{
    
}

// If there's at least one alien that's collided with a, return
// a pointer to one of them; otherwise, return a null pointer.
Actor* StudentWorld:: getOneCollidingAlien(const Actor* a) const
{
    list<Actor*>::const_iterator it= m_actors.begin();
    it= m_actors.begin();
    while (it!= m_actors.end())
    {
        if ((*it)!=a && (*it)->collidableWithPlayerFiredProjectile())
        {
            if (euclideanDistance(a->getX(), a->getY(),(*it)->getX(), (*it)->getY()) < 0.75 * (a->getRadius() + (*it)->getRadius()))
            return (*it);
        }
        it++;
    }
    return nullptr;
}

// If the player has collided with a, return a pointer to the player;
// otherwise, return a null pointer.
NachenBlaster* StudentWorld:: getCollidingPlayer(const Actor* a) const
{
        if (euclideanDistance(a->getX(), a->getY(),m_NachenBlaster->getX(), m_NachenBlaster->getY()) < 0.75 * (a->getRadius() + m_NachenBlaster->getRadius()))
            return m_NachenBlaster;
    return nullptr;
}

double StudentWorld:: euclideanDistance(int x1, int y1, int x2, int y2) const
{
    double x = x2 - x1;
    double y = y2 - y1;
    double distance= pow(x, 2) + pow(y, 2);
    distance = sqrt(distance);
    return distance;
}

// Is the player in the line of fire of a, which might cause a to attack?
bool StudentWorld:: playerInLineOfFire(const Actor* a) const
{
    return false;////////////////////////////////////////////////////////////////////////////////////
}

// Add an actor to the world.
void StudentWorld:: addActor(Actor* a)
{
    m_actors.push_back(a);
}

// Record that one more alien on the current level has been destroyed.
void StudentWorld:: recordAlienDestroyed()
{
    m_aliensDestroyed++;
}

int StudentWorld:: remainingAlienShipstoDestroyOnThisLevel()
{
    int t = 6+(4*getLevel());
    int result= t-m_aliensDestroyed;
    //std::cout<<m_aliensDestroyed<<endl;
    return result;
}

int StudentWorld:: maxAlienShipsOnScreen()
{
   return (4 + (.5 * getLevel()));
}

std::string StudentWorld:: whichAlienShipShouldiAdd()
{
    int S1 = 60;
    int S2 = 20 + getLevel() * 5;
    int S3 = 5 + getLevel() * 10;
    int S = S1 + S2 + S3;
    
    int arr[3];
    if (S1<S2 && S1<S3)
    {
        arr[0]=S1;
        if (S2<S3)
        {
            arr[1]=S2;
            arr[2]=S3;
        }
        else
        {
            arr[1]=S3;
            arr[2]=S2;
        }
    }
    else if (S2<S1 && S2<S3)
    {
        arr[0]=S2;
        if (S1<S3)
        {
            arr[1]=S1;
            arr[2]=S3;
        }
        else
        {
            arr[1]=S3;
            arr[2]=S1;
        }
    }
    else
    {
        arr[0]=S3;
        if (S1<S2)
        {
            arr[1]=S1;
            arr[2]=S2;
        }
        else
        {
            arr[1]=S2;
            arr[2]=S1;
        }
    }
    
    int r = randInt(1, S);
    
    if (r <= arr[0])
    {
        
         if (arr[0]== S1)
             return "smallgon";
         else if (arr[0]==S2)
             return "smoregon";
         else if (arr[0]==S3)
             return "snagglegon";
         else
             cerr<<"Error in whichAlienShipShouldiAdd()"<<endl;
    }
        
    else if ( r>15 && r<= arr[0]+arr[1])
    {
        if (arr[1]== S1)
            return "smallgon";
        else if (arr[1]==S2)
            return "smoregon";
        else if (arr[1]==S3)
            return "snagglegon";
        else
            cerr<<"Error in whichAlienShipShouldiAdd()"<<endl;
    }
    
    else
    {
        if (arr[2]== S1)
            return "smallgon";
        else if (arr[2]==S2)
            return "smoregon";
        else if (arr[2]==S3)
            return "snagglegon";
        else
          cerr<<"Error in whichAlienShipShouldiAdd()"<<endl;
    }
    
    return "Error in whichAlienShipShouldiAdd()";
}

void StudentWorld:: introduceAnyNewActors()
{
    int addStar = randInt(1, 15);
    if (addStar==1)
        addActor(new Star(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
    
   /* You must use the following formula to decide whether to add a new alien ship during a given tick:
    o First, determine how many alien ships have been destroyed during the current level (not including any ships destroyed before the player last lost a ship). Call this number D.
    o Second, compute the number of Remaining alien ships that must be destroyed before the level is completed, R. Remember that the total number of ships that must be destroyed per level is T = 6+(4*n), so the number of remaining ships R is equal to T minus D (the number of destroyed ships thus far).
    o Third, compute the maximum number of alien ships that should be on the screen at a time: M = 4 + (.5 * current_level_number)
    o If the current number of alien ships on the screen is less than min(M, R), then you must introduce one new alien ship at the far right side of the screen during the current tick.

    Should you need to add a new alien ship to the screen during a tick, you must use the following formula to decide which type of ship to add:
    o S1 = 60
    o S2 = 20 + n * 5, where n is the current level number. o S3 = 5 + n * 10, where n is the current level number.
    7
    o S = S1 + S2 + S3
    o With probability S1/S, the new ship you add must be a Smallgon.
    o With probability S2/S, it must be a Smoregon.
    o With probability S3/S, it must be a Snagglegon.
    o All new alien ships must start at x=VIEW_WIDTH-1, and a random y
    between 0 and VIEW_HEIGHT-1, inclusive. */
    
    int m=remainingAlienShipstoDestroyOnThisLevel();
    //std::cout<<"remainingAlienShipstoDestroyOnThisLevel: "<<m<<std::endl;
    int r=maxAlienShipsOnScreen();
    //std::cout<<"maxAlienShipsOnScreen: "<<r<<std::endl;
    //std::cout<<m_alienShipsOnScreen<<endl;
    
    if (m_alienShipsOnScreen<m || m_alienShipsOnScreen<r)
    {
        //std::cout<<"On screen: "<<m_alienShipsOnScreen<<" Remaining to destroy: "<<m<< "Max on screen: "<<r<<endl;
        string ship=whichAlienShipShouldiAdd();
        
           if (ship=="smallgon")
            {
               addActor(new Smallgon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1),0));
               m_alienShipsOnScreen++;
            }
        
           else if (ship=="smoregon")
            {
               addActor(new Smoregon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1),0));
               m_alienShipsOnScreen++;
            }

    
           else if (ship=="snagglegon")
            {
                addActor(new Snagglegon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
                m_alienShipsOnScreen++;
            }
           else
                    cerr<< "error in introduceAnyNewActors()";
    }
}

void StudentWorld:: deleteAnyDeadActors()
{
    list<Actor*>::iterator  it= m_actors.begin();
    it= m_actors.begin();
    while (it!= m_actors.end())
    {
        if ((*it)->isDead())
        {
            list<Actor*>::iterator  temp= it;
            it++;
            if ((*temp)->collidableWithPlayerFiredProjectile())
                m_alienShipsOnScreen--;
            delete (*temp);
            m_actors.remove(*temp);
        }
        
        it++;
    }
}


NachenBlaster* StudentWorld:: getPlayer()
{
    return m_NachenBlaster;
}

std::string StudentWorld:: updateDisplayText()
{
    
    ostringstream oss;
    //oss << "Lives: ";
    int lives= getLives();
    int health= getPlayer()->healthPct();
    int score=getScore();
    int level=getLevel();
    int cabbages=getPlayer()->cabbagePct();
    int torpedoes=getPlayer()->numTorpedoes();
    
    oss << "Lives: " << lives<<" Health: "<<health<<"%"<<" Score: "<<score<<" Level: "<<level<<" Cabbages: "<<cabbages<<"%"<<" Torpedoes: "<<torpedoes<<endl;
    std:: string s = oss.str();
    return s;
    
    
    //Lives: 3 Health: 100% Score: 24530 Level: 3 Cabbages: 80% Torpedoes: 4
    
}












