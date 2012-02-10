#include "SoccerTeam.h"
#include "../../SoccerPitch.h"
#include "../../Goal.h"
#include "../../PlayerBase.h"
#include "../../GoalKeeper.h"
#include "../../FieldPlayer.h"
#include "misc/utils.h"
#include "../../SteeringBehaviors.h"
#include "GoalKeeperStates.h"
#include "../../ParamLoader.h"
#include "2D/geometry.h"
#include "Game/EntityManager.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "TeamStates.h"
#include "Debug/DebugConsole.h"
#include <windows.h>

using std::vector;

//----------------------------- ctor -------------------------------------
//
//------------------------------------------------------------------------
SoccerTeam::SoccerTeam(Goal*        home_goal,
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
SoccerTeam::~SoccerTeam()
{

  delete m_pSupportSpotCalc;
}

void SoccerTeam::InitStateMachine()
{
  m_pStateMachine = new StateMachine<AbstSoccerTeam>(this);

  m_pStateMachine->SetCurrentState(Defending::Instance());
  m_pStateMachine->SetPreviousState(Defending::Instance());
  m_pStateMachine->SetGlobalState(NULL);
}

void SoccerTeam::InitPlayers()
{
  //set default steering behaviors
  std::vector<PlayerBase*>::iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {
    (*it)->Steering()->SeparationOn();   
  }
}



//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void SoccerTeam::CreatePlayers()
{
  if (Color() == blue)
  {
    //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
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
 
    //create the players
    m_Players.push_back(new FieldPlayer(this,
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



        m_Players.push_back(new FieldPlayer(this,
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


 


        m_Players.push_back(new FieldPlayer(this,
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


        m_Players.push_back(new FieldPlayer(this,
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

     //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
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


    //create the players
    m_Players.push_back(new FieldPlayer(this,
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

    m_Players.push_back(new FieldPlayer(this,
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


 
    m_Players.push_back(new FieldPlayer(this,
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


    m_Players.push_back(new FieldPlayer(this,
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

void SoccerTeam::prepareForKickoff ()
{
	GetFSM()->ChangeState(PrepareForKickOff::Instance());
}

//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void SoccerTeam::UpdateTargetsOfWaitingPlayers()const
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

