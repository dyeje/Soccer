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

//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void Prior__Team::CreatePlayers()
{
  if (Color() == blue) {
    
	  // Create Blue players
    m_Players.push_back(new Prior__GoalKeeper(this,
                               1,
                               Prior__TendGoal::Instance(),
							   Prior__GlobalKeeperState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));
 
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
                               PlayerBase::defender));


        m_Players.push_back(new Prior__FieldPlayer(this,
                               5,
                               Prior__Wait::Instance(),
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
    m_Players.push_back(new Prior__GoalKeeper(this,
                               16,
                               Prior__TendGoal::Instance(),
                               Prior__GlobalKeeperState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));

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
                               PlayerBase::defender));


    m_Players.push_back(new Prior__FieldPlayer(this,
                               14,
                               Prior__Wait::Instance(),
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


bool Prior__Team::FindPassOffBoards(const PlayerBase*const passer,
                      PlayerBase*&           receiver,
                      Vector2D&              PassTarget,
                      double                 power,
                      double                 MinPassingDistance)const
{
  return false;
}


