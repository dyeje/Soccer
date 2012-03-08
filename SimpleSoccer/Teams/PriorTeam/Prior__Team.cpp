#include "Prior__Team.h"
#include "../../SoccerPitch.h"
#include "../../Goal.h"
#include "../../PlayerBase.h"
#include "Prior__Goalkeeper.h"
#include "Prior__FieldPlayer.h"
#include "GameStateLoader.h"
#include "misc/utils.h"
#include "../../SteeringBehaviors.h"
#include "Prior__FieldPlayerStates.h"
#include "Prior__GoalKeeperStates.h"
#include "../../ParamLoader.h"
#include "2D/geometry.h"
#include "Game/EntityManager.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "Prior__States.h"
#include "Debug/DebugConsole.h"
#include <windows.h>

using std::vector;

Prior__Team::Prior__Team(Goal*        home_goal,
                       Goal*        opponents_goal,
                       SoccerPitch* pitch,
					   team_color   color): AbstSoccerTeam(home_goal, opponents_goal, pitch, color)
{
	InitStateMachine();
	CreatePlayers();
	RegisterPlayers();
	InitPlayers();

  //create the sweet spot calculator
  m_pPrior__SupportSpotCalc = new Prior__SupportSpotCalculator(Prm.NumSupportSpotsX,
                                                        Prm.NumSupportSpotsY,
                                                        this);
  // AbstSoccerTeam may still use the default SupportSpotCalculator, so keep it.
  m_pSupportSpotCalc  = new SupportSpotCalculator(Prm.NumSupportSpotsX,
                                                 Prm.NumSupportSpotsY,
                                                 this);

  ballRadius = Pitch()->Ball()->BRadius();
  goalYMin = ((pitch->cyClient()/2) - Prm.GoalWidth/2) + ballRadius;
  goalYMax = (pitch->cyClient()/2) + Prm.GoalWidth/2 - ballRadius;
  opponentGoalie = NULL;
  pitchMinY = 0;
  pitchMaxY = m_pPitch->cyClient();
}


//----------------------- dtor -------------------------------------------
//
//------------------------------------------------------------------------
Prior__Team::~Prior__Team()
{

  delete m_pSupportSpotCalc;
}

void Prior__Team::InitStateMachine()
{
  m_pStateMachine = new StateMachine<AbstSoccerTeam>(this);

  m_pStateMachine->SetCurrentState(Prior__Defending::Instance());
  m_pStateMachine->SetPreviousState(Prior__Defending::Instance());
  m_pStateMachine->SetGlobalState(NULL);
}

void Prior__Team::InitPlayers()
{
  //set default steering behaviors
  std::vector<PlayerBase*>::iterator it = m_Players.begin();
  pair<double,double> coord;
  string state;

  for (it; it != m_Players.end(); ++it)
  {
    (*it)->Steering()->SeparationOn();
    
    // Get/Set Red Player positions, if players are specified in state file
    Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(*it);
    coord = GameState.PlayerCoord(plyr->HomeRegion());
    if(coord.first != 0 && coord.second != 0) {
      plyr->ForcePosition(coord.first,coord.second);
      state = GameState.PlayerState(plyr->HomeRegion());
      if(state=="ChaseBall")
          plyr->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
      else if(state=="SupportAttacker")
          plyr->GetFSM()->ChangeState(Prior__SupportAttacker::Instance());
      else if(state=="KickBall")
          plyr->GetFSM()->ChangeState(Prior__KickBall::Instance());
      else if(state=="Dribble")
         plyr->GetFSM()->ChangeState(Prior__Dribble::Instance());
      else if(state=="ReceiveBall")
          plyr->GetFSM()->ChangeState(Prior__ReceiveBall::Instance());
      else if(state=="Wait")
          plyr->GetFSM()->ChangeState(Prior__Wait::Instance());
      else if(state=="TendGoal") {
          Prior__GoalKeeper* plyr_g = static_cast<Prior__GoalKeeper*>(*it);
          plyr_g->GetFSM()->ChangeState(Prior__TendGoal::Instance());
      }
    }
  }

  // Get/Set Ball position, if specified in state file
  coord = GameState.PlayerCoord(0);
  if(coord.first != 0 && coord.second != 0)
    Pitch()->Ball()->PlaceAtPosition(Vector2D(coord.first,coord.second));

  // Get/Set team state, if specified in state file
  state = GameState.TeamState(Color());
  if(state=="Attacking") {
    m_pStateMachine->SetCurrentState(Prior__Attacking::Instance());
    m_pStateMachine->SetPreviousState(Prior__Attacking::Instance());
  }
  else if(state=="Defending") {
    m_pStateMachine->SetCurrentState(Prior__Defending::Instance());
    m_pStateMachine->SetPreviousState(Prior__Defending::Instance());
  }

 }

void Prior__Team::SetPrior__Goalie(PlayerBase* player) {
  Prior__Goalie = player;
}  
PlayerBase* Prior__Team::GetPrior__Goalie() {
  return Prior__Goalie;
}

//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void Prior__Team::CreatePlayers()
{
  Prior__GoalKeeper* goalie;

  if (Color() == blue) {
    
	  // Create Blue players
    goalie = new Prior__GoalKeeper(this,
                               1,
                               Prior__TendGoal::Instance(),
                               Prior__GlobalKeeperState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale);

    m_Players.push_back(goalie);
    SetPrior__Goalie(goalie);
 
    m_Players.push_back(new Prior__FieldPlayer(this,
                               6,
                               Prior__Wait::Instance(),
							   Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));



        m_Players.push_back(new Prior__FieldPlayer(this,
                               8,
                               Prior__Wait::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 


        m_Players.push_back(new Prior__FieldPlayer(this,
                               3,
                               Prior__Wait::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


        m_Players.push_back(new Prior__FieldPlayer(this,
                               5,
                               Prior__DefenseMeister::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                              PlayerBase::defender));

  }

  else
  {
	  // Create Red players
    goalie = new Prior__GoalKeeper(this,
                               16,
                               Prior__TendGoal::Instance(),
                               Prior__GlobalKeeperState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale);

    m_Players.push_back(goalie);
    SetPrior__Goalie(goalie);

    m_Players.push_back(new Prior__FieldPlayer(this,
                               9,
                               Prior__Wait::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));

    m_Players.push_back(new Prior__FieldPlayer(this,
                               11,
                               Prior__Wait::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 
    m_Players.push_back(new Prior__FieldPlayer(this,
                               12,
                               Prior__Wait::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


    m_Players.push_back(new Prior__FieldPlayer(this,
                               14,
                               Prior__DefenseMeister::Instance(),
                               Prior__GlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));
                      
  }
}

void Prior__Team::prepareForKickoff ()
{
	GetFSM()->ChangeState(Prior__PrepareForKickOff::Instance());
}

//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void Prior__Team::UpdateTargetsOfWaitingPlayers()const
{
  std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {  
    if ( (*it)->Role() != PlayerBase::goal_keeper )
    {
      //cast to a field player
      FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);
      
      if ( plyr->GetFSM()->isInState(*Prior__Wait::Instance()) ||
           plyr->GetFSM()->isInState(*Prior__ReturnToHomeRegion::Instance()) )
      {
        plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
      }
    }
  }
}



//------------------------ CanShoot --------------------------------------
//
//  Given a ball position, a kicking power and a reference to a vector2D
//  this function will sample random positions along the opponent's goal-
//  mouth and check to see if a goal can be scored if the ball was to be
//  kicked in that direction with the given power. If a possible shot is 
//  found, the function will immediately return true, with the target 
//  position stored in the vector ShotTarget.
//------------------------------------------------------------------------
bool Prior__Team::CanShoot(Vector2D  BallPos,
                          double     power, 
                          Vector2D& ShotTarget)const
{
  if(!opponentGoalie) {
    // Find and cache Opponent's Goalie
	  std::vector<PlayerBase*>::const_iterator opp = Opponents()->Members().begin();
	  for (opp; opp != Opponents()->Members().end(); ++opp)
	  {
		  if ((*opp)->Role() == PlayerBase::goal_keeper)
		  {
        (const_cast<PlayerBase*>(opponentGoalie)) = const_cast<PlayerBase*>(*opp);
		  }
	  }
	}

	//the number of randomly created shot targets this method will test 
	//int NumAttempts = Prm.NumAttemptsToFindValidStrike;

  //while (NumAttempts--)
  //{
    //choose a random position along the opponent's goal mouth. (making
    //sure the ball's radius is taken into account)
    //ShotTarget = OpponentsGoal()->Center();
    ShotTarget.x = OpponentsGoal()->Center().x;

    double target; 
    (fabs(goalYMin - opponentGoalie->Pos().y) > fabs(goalYMax - opponentGoalie->Pos().y)) 
      ? target = goalYMin+2 
      : target = goalYMax-2;  
    ShotTarget.y = target;

    //make sure striking the ball with the given power is enough to drive
    //the ball over the goal line.
    double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
                                                      ShotTarget,
                                                      power);
    
    //if it is, this shot is then tested to see if any of the opponents
    //can intercept it.
    if (time >= 0)
    {
      if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
      {
        return true;
      }
    }
  //}
  
  return false;
}

//------------- DetermineBestSupportingAttacker ------------------------
//
// calculate the closest player to the SupportSpot
//------------------------------------------------------------------------
PlayerBase* Prior__Team::DetermineBestSupportingAttacker()
{
  double ClosestSoFar = MaxFloat;

  PlayerBase* BestPlayer = NULL;

  std::vector<PlayerBase*>::iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {
    //only attackers utilize the BestSupportingSpot
    if ( ((*it)->Role() == PlayerBase::attacker) && ((*it) != m_pControllingPlayer) )
    {
      //calculate the dist. Use the squared value to avoid sqrt
      double dist = Vec2DDistanceSq((*it)->Pos(), m_pPrior__SupportSpotCalc->GetBestSupportingSpot());
    
      //if the distance is the closest so far and the player is not a
      //goalkeeper and the player is not the one currently controlling
      //the ball, keep a record of this player
      if ((dist < ClosestSoFar) )
      {
        ClosestSoFar = dist;

        BestPlayer = (*it);
      }
    }
  }

  return BestPlayer;
}

Vector2D Prior__Team::GetSupportSpot()const
{
  return m_pPrior__SupportSpotCalc->GetBestSupportingSpot();
}

void Prior__Team::DetermineBestSupportingPosition()const{ 
  m_pPrior__SupportSpotCalc->DetermineBestSupportingPosition();
}




// Determines if an upfield pass off the boards can be made to another
// team member in order to advance the ball towards the enemy goal
bool Prior__Team::FindPassOffBoards(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                 power,
                      double                 MinPassingDistance)const
{
  double ClosestToGoalSoFar = MaxFloat;
  //double goalX = OpponentsGoal()->Center().x;
  double goalX = m_pHomeGoal->Center().x;
  double passerX = passer->Pos().x;
  double passerY = passer->Pos().y;
  double receiverX = 0.0;
  double receiverY = 0.0;
  double dist2Goal;
  Vector2D wallTarget;

  // Iterate through all this player's and calculate which one is both 
  // in a position to be passed the ball off the boards, and if multiple
  // players are, chose the one closest to the goal.  No backward passes
  std::vector<PlayerBase*>::const_iterator curPlyr = Members().begin();
  for (curPlyr; curPlyr != Members().end(); ++curPlyr)
  {   
    // Player must not be the one with the ball
    if ((*curPlyr) != passer)                  
    {
      // Receiver must be closer to goal than passer
      receiverX = (*curPlyr)->Pos().x;
      receiverY = (*curPlyr)->Pos().y;
      int absolutePasserX = fabs(goalX - passerX);
      int absoluteReceiverX = fabs(goalX - receiverX);
      if (absolutePasserX < absoluteReceiverX &&
          fabs(passerY - receiverY) < 50 &&
          fabs(passerX - receiverX) < 200
        )
      {           
        // Determine if it's safe to pass and if the receiver can get
        // to the ball.

        // Determine wallTarget (place on boards to shoot at)
        if(passer->Pos().y < (m_pPitch->cyClient()/2))
          wallTarget.y = 1;                     // Bank off top
        else
          wallTarget.y = m_pPitch->cyClient();  // Bank off bottom

        double diff = fabs(passerX - receiverX)/2;
        if(passerX > receiverX)
          wallTarget.x = passerX - diff;
        else
          wallTarget.x = passerX + diff;

        // Ensure it's good/safe to pass off wallTarget
        if (GetBestPassToReceiverOffBoards(passer, *curPlyr, wallTarget, power))
        {
          // It's safe to pass to this receiver
          dist2Goal = fabs(receiverX - goalX);
          if (dist2Goal < ClosestToGoalSoFar)
          {
            // Best receiver thus far
            ClosestToGoalSoFar = dist2Goal;
            PassTarget = wallTarget;
            receiver = *curPlyr;
          }     
        }
      }
    }
  }//next team member

  if (receiver) {
    return true;
  }
  else {
    return false;
  }
}



bool Prior__Team::GetBestPassToReceiverOffBoards(const PlayerBase* const passer,
                                                 const PlayerBase* const receiver,
                                                 Vector2D&         PassTarget,
                                                 double            power)const
{  
  // Calculate how much time it will take for the ball to reach receiver, time
  // to wall (Target) and time from there to receiver
  double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
                                                    PassTarget,
                                                    power);
  time += Pitch()->Ball()->TimeToCoverDistance(PassTarget,
                                              receiver->Pos(),
                                              power);

  // Return false if ball cannot reach the receiver 
  if (time < 0) return false;

  return true;  // Until logic below is constructed for a pass off boards

  ////the maximum distance the receiver can cover in this time
  //double InterceptRange = time * receiver->MaxSpeed();
  //
  ////Scale the intercept range
  //const double ScalingFactor = 0.3;
  //InterceptRange *= ScalingFactor;

  ////now calculate the pass targets which are positioned at the intercepts
  ////of the tangents from the ball to the receiver's range circle.
  //Vector2D ip1, ip2;

  //GetTangentPoints(receiver->Pos(),
  //                 InterceptRange,
  //                 Pitch()->Ball()->Pos(),
  //                 ip1,
  //                 ip2);
 
  //const int NumPassesToTry = 3;
  //Vector2D Passes[NumPassesToTry] = {ip1, receiver->Pos(), ip2};
  //
  //
  //// this pass is the best found so far if it is:
  ////
  ////  1. Further upfield than the closest valid pass for this receiver
  ////     found so far
  ////  2. Within the playing area
  ////  3. Cannot be intercepted by any opponents

  //double ClosestSoFar = MaxFloat;
  //bool  bResult      = false;

  //for (int pass=0; pass<NumPassesToTry; ++pass)
  //{    
  //  double dist = fabs(Passes[pass].x - OpponentsGoal()->Center().x);

  //  if (( dist < ClosestSoFar) &&
  //      Pitch()->PlayingArea()->Inside(Passes[pass]) &&
  //      isPassSafeFromAllOpponents(Pitch()->Ball()->Pos(),
  //                                 Passes[pass],
  //                                 receiver,
  //                                 power))
  //      
  //  {
  //    ClosestSoFar = dist;
  //    PassTarget   = Passes[pass];
  //    bResult      = true;
  //  }
  //}
  //
  //return bResult;
}

