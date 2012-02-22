#include "BlazersTeam.h"
#include "../../SoccerPitch.h"
#include "../../Goal.h"
#include "../../PlayerBase.h"
// #include "../../Goalkeeper.h"
#include "BlazersGoalkeeper.h"
// #include "../../FieldPlayer.h"
#include "BlazersFieldPlayer.h"
#include "misc/utils.h"
#include "../../SteeringBehaviors.h"
#include "FieldPlayerStates.h"
#include "GoalKeeperStates.h"
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

  for (it; it != m_Players.end(); ++it)
  {
    (*it)->Steering()->SeparationOn();
    
    //cast to a field player
    // BlazersFieldPlayer* plyr = static_cast<BlazersFieldPlayer*>(*it);
    // if(plyr->HomeRegion()==9) {
    //   plyr->ForcePosition(50,450);
    // }
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
                               TendGoal::Instance(),
							   GlobalKeeperState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));
 
    m_Players.push_back(new BlazersFieldPlayer(this,
                               6,
                               Wait::Instance(),
							   GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               TendGoal::Instance(),
                               GlobalKeeperState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));

    m_Players.push_back(new BlazersFieldPlayer(this,
                               9,
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
                               Wait::Instance(),
                               GlobalPlayerState::Instance(),
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
      
      if ( plyr->GetFSM()->isInState(*Wait::Instance()) ||
           plyr->GetFSM()->isInState(*ReturnToHomeRegion::Instance()) )
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
  //the number of randomly created shot targets this method will test 
  int NumAttempts = Prm.NumAttemptsToFindValidStrike;

  while (NumAttempts--)
  {
    //choose a random position along the opponent's goal mouth. (making
    //sure the ball's radius is taken into account)
    ShotTarget = OpponentsGoal()->Center();

    //the y value of the shot position should lay somewhere between two
    //goalposts (taking into consideration the ball diameter)
    int MinYVal = OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius();
    int MaxYVal = OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius();

	Vector2D goalyPos;
	std::vector<PlayerBase*>::const_iterator opp = Opponents()->Members().begin();

	for (opp; opp != Opponents()->Members().end(); ++opp)
	{
		if ((*opp)->Role() == PlayerBase::goal_keeper)
		{
			goalyPos = (*opp)->Pos();
		}
	}

	int target = max(fabs(MinYVal - goalyPos.x), fabs(MaxYVal - goalyPos.x));
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
  }
  
  return false;
}



