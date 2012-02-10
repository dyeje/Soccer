#ifndef SOCCERTEAM_H
#define SOCCERTEAM_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   SoccerTeam.h
//
//  Desc:   class to define a team of soccer playing agents. A SoccerTeam
//          contains several field players and one goalkeeper. A SoccerTeam
//          is implemented as a finite state machine and has states for
//          attacking, defending, and KickOff.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <vector>

#include "Game/Region.h"
#include "../../SupportSpotCalculator.h"
#include "FSM/StateMachine.h"
#include "../../AbstSoccerTeam.h"
#include "FieldPlayerStates.h"

class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;




                
class SoccerTeam : public AbstSoccerTeam
{

public:

  SoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~SoccerTeam();

  std::string Name()const{return "Buckland";}

	void InitStateMachine();
	void CreatePlayers();
	void InitPlayers();
	void prepareForKickoff();
	
  void UpdateTargetsOfWaitingPlayers()const;
};

#endif