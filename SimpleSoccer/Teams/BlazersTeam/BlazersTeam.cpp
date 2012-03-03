#include "BlazersTeam.h"
#include "../../SoccerPitch.h"
#include "../../Goal.h"
#include "../../PlayerBase.h"
#include "BlazersGoalkeeper.h"
#include "BlazersFieldPlayer.h"
#include "GameStateLoader.h"
#include "misc/utils.h"
#include "../../SteeringBehaviors.h"
#include "BlazersFieldPlayerStates.h"
#include "BlazersGoalKeeperStates.h"
#include "../../ParamLoader.h"
#include "2D/geometry.h"
#include "Game/EntityManager.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "BlazersStates.h"
#include "Debug/DebugConsole.h"
#include <windows.h>

using std::vector;

BlazersTeam::BlazersTeam(Goal*        home_goal,
                       Goal*        opponents_goal,
                       SoccerPitch* pitch,
					   team_color   color): AbstSoccerTeam(home_goal, opponents_goal, pitch, color)
{
	InitStateMachine();
	CreatePlayers();
	RegisterPlayers();
	InitPlayers();

  //create the sweet spot calculator
  m_pSupportSpotCalc = new SupportSpotCalculator(Prm.NumSupportSpotsX,
                                                 Prm.NumSupportSpotsY,
                                                 this);

  ballRadius = Pitch()->Ball()->BRadius();
  goalYMin = ((pitch->cyClient()/2) - Prm.GoalWidth/2) + ballRadius;
  goalYMax = (pitch->cyClient()/2) + Prm.GoalWidth/2 - ballRadius;
  opponentGoalie = NULL;
}


//----------------------- dtor -------------------------------------------
//
//------------------------------------------------------------------------
BlazersTeam::~BlazersTeam()
{

  delete m_pSupportSpotCalc;
}

void BlazersTeam::InitStateMachine()
{
  m_pStateMachine = new StateMachine<AbstSoccerTeam>(this);

  m_pStateMachine->SetCurrentState(BlazersDefending::Instance());
  m_pStateMachine->SetPreviousState(BlazersDefending::Instance());
  m_pStateMachine->SetGlobalState(NULL);
}

void BlazersTeam::InitPlayers()
{
  //set default steering behaviors
  std::vector<PlayerBase*>::iterator it = m_Players.begin();
  pair<double,double> coord;
  string state;

  for (it; it != m_Players.end(); ++it)
  {
    (*it)->Steering()->SeparationOn();
    
    // Get/Set Red Player positions, if players are specified in state file
    BlazersFieldPlayer* plyr = static_cast<BlazersFieldPlayer*>(*it);
    coord = GameState.PlayerCoord(plyr->HomeRegion());
    if(coord.first != 0 && coord.second != 0) {
      plyr->ForcePosition(coord.first,coord.second);
      state = GameState.PlayerState(plyr->HomeRegion());
      if(state=="ChaseBall")
          plyr->GetFSM()->ChangeState(BlazersChaseBall::Instance());
      else if(state=="SupportAttacker")
          plyr->GetFSM()->ChangeState(BlazersSupportAttacker::Instance());
      else if(state=="KickBall")
          plyr->GetFSM()->ChangeState(BlazersKickBall::Instance());
      else if(state=="Dribble")
         plyr->GetFSM()->ChangeState(BlazersDribble::Instance());
      else if(state=="ReceiveBall")
          plyr->GetFSM()->ChangeState(BlazersReceiveBall::Instance());
      else if(state=="Wait")
          plyr->GetFSM()->ChangeState(BlazersWait::Instance());
      else if(state=="TendGoal") {
          BlazersGoalKeeper* plyr_g = static_cast<BlazersGoalKeeper*>(*it);
          plyr_g->GetFSM()->ChangeState(BlazersTendGoal::Instance());
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
    m_pStateMachine->SetCurrentState(BlazersAttacking::Instance());
    m_pStateMachine->SetPreviousState(BlazersAttacking::Instance());
  }
  else if(state=="Defending") {
    m_pStateMachine->SetCurrentState(BlazersDefending::Instance());
    m_pStateMachine->SetPreviousState(BlazersDefending::Instance());
  }

 }

//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void BlazersTeam::CreatePlayers()
{
  if (Color() == blue) {
    
	  // Create Blue players
    m_Players.push_back(new BlazersGoalKeeper(this,
                               1,
                               BlazersTendGoal::Instance(),
							   BlazersGlobalKeeperState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));
 
    m_Players.push_back(new BlazersFieldPlayer(this,
                               6,
                               BlazersWait::Instance(),
							   BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));



        m_Players.push_back(new BlazersFieldPlayer(this,
                               8,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 


        m_Players.push_back(new BlazersFieldPlayer(this,
                               3,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));


        m_Players.push_back(new BlazersFieldPlayer(this,
                               5,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
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
    m_Players.push_back(new BlazersGoalKeeper(this,
                               16,
                               BlazersTendGoal::Instance(),
                               BlazersGlobalKeeperState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));

    m_Players.push_back(new BlazersFieldPlayer(this,
                               9,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));

    m_Players.push_back(new BlazersFieldPlayer(this,
                               11,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 
    m_Players.push_back(new BlazersFieldPlayer(this,
                               12,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));


    m_Players.push_back(new BlazersFieldPlayer(this,
                               14,
                               BlazersWait::Instance(),
                               BlazersGlobalPlayerState::Instance(),
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

void BlazersTeam::prepareForKickoff ()
{
	GetFSM()->ChangeState(BlazersPrepareForKickOff::Instance());
}

//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void BlazersTeam::UpdateTargetsOfWaitingPlayers()const
{
  std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {  
    if ( (*it)->Role() != PlayerBase::goal_keeper )
    {
      //cast to a field player
      FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);
      
      if ( plyr->GetFSM()->isInState(*BlazersWait::Instance()) ||
           plyr->GetFSM()->isInState(*BlazersReturnToHomeRegion::Instance()) )
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
bool BlazersTeam::CanShoot(Vector2D  BallPos,
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


bool BlazersTeam::FindPassOffBoards(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                 power,
                      double                 MinPassingDistance)const
{
  return false;
}


