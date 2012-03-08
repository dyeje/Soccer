#include "BlazersStates.h"
#include "../../AbstSoccerTeam.h"
#include "BlazersTeam.h"
#include "../../PlayerBase.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "../../constants.h"
#include "../../SoccerPitch.h"

//uncomment to send state info to debug window
//#define DEBUG_TEAM_STATES
#include "Debug/DebugConsole.h"

//************************************************************************ ATTACKING

BlazersAttacking* BlazersAttacking::Instance()
{
  static BlazersAttacking instance;

  return &instance;
}


void BlazersAttacking::Enter(AbstSoccerTeam* team)
{
//#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering BlazersAttacking state" << "";
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


void BlazersAttacking::Execute(AbstSoccerTeam* team)
{
  //if this team is no longer in control change states
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(BlazersDefending::Instance()); return;
  }

  //calculate the best position for any supporting attacker to move to
  (static_cast<BlazersTeam*>(team))->DetermineBestSupportingPosition();
}

void BlazersAttacking::Exit(AbstSoccerTeam* team)
{
  //there is no supporting player for defense
  team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

BlazersDefending* BlazersDefending::Instance()
{
  static BlazersDefending instance;

  return &instance;
}

void BlazersDefending::Enter(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering BlazersDefending state" << "";
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

void BlazersDefending::Execute(AbstSoccerTeam* team)
{
  //if in control change states
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(BlazersAttacking::Instance()); return;
  }
}


void BlazersDefending::Exit(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " exiting BlazersDefending state" << "";
#endif
}


//************************************************************************ KICKOFF
BlazersPrepareForKickOff* BlazersPrepareForKickOff::Instance()
{
  static BlazersPrepareForKickOff instance;

  return &instance;
}

void BlazersPrepareForKickOff::Enter(AbstSoccerTeam* team)
{
  //reset key player pointers
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  //send Msg_GoHome to each player.
  team->ReturnAllFieldPlayersToHome();
}

void BlazersPrepareForKickOff::Execute(AbstSoccerTeam* team)
{
  //if both teams in position, start the game
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome() ||
		(team->AllPlayersAtHome() && team->Pitch()->OneTeamReady()))
  {
    team->GetFSM()->ChangeState(BlazersDefending::Instance());
  }
}

void BlazersPrepareForKickOff::Exit(AbstSoccerTeam* team)
{
	if (team->Color() == AbstSoccerTeam::blue)
		team->Pitch()->SetBlueTeamReady();
	else
		team->Pitch()->SetRedTeamReady();
}


