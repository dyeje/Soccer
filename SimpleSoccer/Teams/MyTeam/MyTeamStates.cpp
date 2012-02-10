#include "MyTeamStates.h"
#include "../../AbstSoccerTeam.h"
#include "../../PlayerBase.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "../../constants.h"
#include "../../SoccerPitch.h"

//uncomment to send state info to debug window
//#define DEBUG_TEAM_STATES
#include "Debug/DebugConsole.h"

//************************************************************************ ATTACKING

MyAttacking* MyAttacking::Instance()
{
  static MyAttacking instance;

  return &instance;
}


void MyAttacking::Enter(AbstSoccerTeam* team)
{
//#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering MyAttacking state" << "";
//#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,9,11,7,4};
  const int RedRegions[TeamSize] = {16,8,6,10,13};

  //set up the player's home regions
  if (team->Color() == AbstSoccerTeam::blue)
  {
    team->ChangePlayerHomeRegions(BlueRegions);
  }
  else
  {
    team->ChangePlayerHomeRegions(RedRegions);
  }

  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region to enable
  //it to move into the correct position.
  team->UpdateTargetsOfWaitingPlayers();
}


void MyAttacking::Execute(AbstSoccerTeam* team)
{
  //if this team is no longer in control change states
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(MyDefending::Instance()); return;
  }

  //calculate the best position for any supporting attacker to move to
  team->DetermineBestSupportingPosition();
}

void MyAttacking::Exit(AbstSoccerTeam* team)
{
  //there is no supporting player for defense
  team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

MyDefending* MyDefending::Instance()
{
  static MyDefending instance;

  return &instance;
}

void MyDefending::Enter(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering MyDefending state" << "";
#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,6,8,7,4};
  const int RedRegions[TeamSize] = {16,9,11,10,13};

  //set up the player's home regions
  if (team->Color() == AbstSoccerTeam::blue)
  {
    team->ChangePlayerHomeRegions(BlueRegions);
  }
  else
  {
    team->ChangePlayerHomeRegions(RedRegions);
  }
  
  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region
  team->UpdateTargetsOfWaitingPlayers();
}

void MyDefending::Execute(AbstSoccerTeam* team)
{
  //if in control change states
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(MyAttacking::Instance()); return;
  }
}


void MyDefending::Exit(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " exiting MyDefending state" << "";
#endif
}


//************************************************************************ KICKOFF
MyPrepareForKickOff* MyPrepareForKickOff::Instance()
{
  static MyPrepareForKickOff instance;

  return &instance;
}

void MyPrepareForKickOff::Enter(AbstSoccerTeam* team)
{
  //reset key player pointers
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  //send Msg_GoHome to each player.
  team->ReturnAllFieldPlayersToHome();
}

void MyPrepareForKickOff::Execute(AbstSoccerTeam* team)
{
  //if both teams in position, start the game
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome() ||
		(team->AllPlayersAtHome() && team->Pitch()->OneTeamReady()))
  {
    team->GetFSM()->ChangeState(MyDefending::Instance());
  }
}

void MyPrepareForKickOff::Exit(AbstSoccerTeam* team)
{
	if (team->Color() == AbstSoccerTeam::blue)
		team->Pitch()->SetBlueTeamReady();
	else
		team->Pitch()->SetRedTeamReady();
}


