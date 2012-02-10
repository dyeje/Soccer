#ifndef ABSTSOCCERTEAM_H
#define ABSTSOCCERTEAM_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   AbstSoccerTeam.h
//
//  Desc:   class to define a team of soccer playing agents. A SoccerTeam
//          contains several field players and one goalkeeper. A SoccerTeam
//          is implemented as a finite state machine and has states for
//          attacking, defending, and KickOff.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//  9/13/2006 RB Abstract class to support multiple team competition
//
//------------------------------------------------------------------------

#include <vector>

#include "Game/Region.h"
#include "SupportSpotCalculator.h"
#include "FSM/StateMachine.h"
#include "constants.h"

class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;




                
class AbstSoccerTeam 
{
public:
  
  enum team_color {blue, red};

  // RB 12/17/2005 Changed from private so that I can create subclasses
  // implementing different teams.
protected:

   //an instance of the state machine class
  StateMachine<AbstSoccerTeam>*  m_pStateMachine;

  //the team must know its own color!
  team_color                m_Color;

  //pointers to the team members
  std::vector<PlayerBase*>  m_Players;

  //a pointer to the soccer pitch
  SoccerPitch*              m_pPitch;

  //pointers to the goals
  Goal*                     m_pOpponentsGoal;
  Goal*                     m_pHomeGoal;
  
  //a pointer to the opposing team
  AbstSoccerTeam*               m_pOpponents;
   
  //pointers to 'key' players
  PlayerBase*               m_pControllingPlayer;
  PlayerBase*               m_pSupportingPlayer;
  PlayerBase*               m_pReceivingPlayer;
  PlayerBase*               m_pPlayerClosestToBall;

  //the squared distance the closest player is from the ball
  double                     m_dDistSqToBallOfClosestPlayer;

  //players use this to determine strategic positions on the playing field
  SupportSpotCalculator*    m_pSupportSpotCalc;

  //called each frame. Sets m_pClosestPlayerToBall to point to the player
  //closest to the ball. 
  void CalculateClosestPlayerToBall();

public:

  AbstSoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~AbstSoccerTeam();

  // Virtual functions can be customized by subclasses

  //calling this changes the state of all field players to that of 
  //ReturnToHomeRegion. Mainly used when a goal keeper has
  //possession
  virtual void        ReturnAllFieldPlayersToHome()const;

  //returns true if player has a clean shot at the goal and sets ShotTarget
  //to a normalized vector pointing in the direction the shot should be
  //made. Else returns false and sets heading to a zero vector
  virtual bool        CanShoot(Vector2D  BallPos,
                       double     power, 
                       Vector2D& ShotTarget = Vector2D())const;

  //The best pass is considered to be the pass that cannot be intercepted 
  //by an opponent and that is as far forward of the receiver as possible  
  //If a pass is found, the receiver's address is returned in the 
  //reference, 'receiver' and the position the pass will be made to is 
  //returned in the  reference 'PassTarget'
  virtual bool        FindPass(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                  power,
                      double                  MinPassingDistance)const;

  //Three potential passes are calculated. One directly toward the receiver's
  //current position and two that are the tangents from the ball position
  //to the circle of radius 'range' from the receiver.
  //These passes are then tested to see if they can be intercepted by an
  //opponent and to make sure they terminate within the playing area. If
  //all the passes are invalidated the function returns false. Otherwise
  //the function returns the pass that takes the ball closest to the 
  //opponent's goal area.
  virtual bool        GetBestPassToReceiver(const PlayerBase* const passer,
                                    const PlayerBase* const receiver,
                                    Vector2D& PassTarget,
                                    const double power)const;

  //test if a pass from positions 'from' to 'target' kicked with force 
  //'PassingForce'can be intercepted by an opposing player
  virtual bool        isPassSafeFromOpponent(Vector2D    from,
                                     Vector2D    target,
                                     const PlayerBase* const receiver,
                                     const PlayerBase* const opp,
                                     double       PassingForce)const;

  //tests a pass from position 'from' to position 'target' against each member
  //of the opposing team. Returns true if the pass can be made without
  //getting intercepted
  virtual bool        isPassSafeFromAllOpponents(Vector2D from,
                                         Vector2D target,
                                         const PlayerBase* const receiver,
                                         double     PassingForce)const;



  //this tests to see if a pass is possible between the requester and
  //the controlling player. If it is possible a message is sent to the
  //controlling player to pass the ball asap.
  virtual void        RequestPass(FieldPlayer* requester)const;

  //calculates the best supporting position and finds the most appropriate
  //attacker to travel to the spot
  virtual PlayerBase* DetermineBestSupportingAttacker();
  


	// pure virtual functions that subclasses must implement
	 //creates all the players for this team
	virtual void CreatePlayers() = 0;

	virtual void InitStateMachine () = 0;

	virtual void InitPlayers() = 0;

	// RB 12/17/2005 Need to reify this so that teams can have different
	// state machines
	virtual void prepareForKickoff() = 0;

	virtual std::string Name()const = 0;

	// Non-virtual functions

	  //the usual suspects
  void        Render()const;
  void        Update();

    //returns true if there is an opponent within radius of position
  bool        isOpponentWithinRadius(Vector2D pos, double rad);

	void	RegisterPlayers();

  const std::vector<PlayerBase*>& Members()const{return m_Players;}  

  StateMachine<AbstSoccerTeam>* GetFSM()const{return m_pStateMachine;}
  
  Goal*const           HomeGoal()const{return m_pHomeGoal;}
  Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}

  SoccerPitch*const    Pitch()const{return m_pPitch;}           

  AbstSoccerTeam*const     Opponents()const{return m_pOpponents;}
  void                 SetOpponents(AbstSoccerTeam* opps){m_pOpponents = opps;}

  team_color           Color()const{return m_Color;}

  void                 SetPlayerClosestToBall(PlayerBase* plyr){m_pPlayerClosestToBall=plyr;}
  PlayerBase*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
  
  double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

  Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

  PlayerBase*          SupportingPlayer()const{return m_pSupportingPlayer;}
  void                 SetSupportingPlayer(PlayerBase* plyr){m_pSupportingPlayer = plyr;}

  PlayerBase*          Receiver()const{return m_pReceivingPlayer;}
  void                 SetReceiver(PlayerBase* plyr){m_pReceivingPlayer = plyr;}

  PlayerBase*          ControllingPlayer()const{return m_pControllingPlayer;}
  void                 SetControllingPlayer(PlayerBase* plyr)
  {
    m_pControllingPlayer = plyr;

    //rub it in the opponents faces!
    Opponents()->LostControl();
  }


  bool  InControl()const{if(m_pControllingPlayer)return true; else return false;}
  void  LostControl(){m_pControllingPlayer = NULL;}

  PlayerBase*  GetPlayerFromID(int id)const;
  

  void SetPlayerHomeRegion(int plyr, int region)const;

  void DetermineBestSupportingPosition()const{m_pSupportSpotCalc->DetermineBestSupportingPosition();}

  virtual void UpdateTargetsOfWaitingPlayers()const = 0;

  //returns false if any of the team are not located within their home region
  bool AllPlayersAtHome()const;


	void ChangePlayerHomeRegions (const int NewRegions[TeamSize]);


};


#endif