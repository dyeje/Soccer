#ifndef TEAMMAKER_H
#define TEAMMAKER_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   TeamMaker.h
//
//  Desc:   Abstract class for factory classes. Allows dynamic registry of
//          new teams. Probably overkill, but it is the "right way" to do it.
//
//  Author: Robin Burke 2005 rburke@cs.depaul.edu
//
//------------------------------------------------------------------------

#include "AbstSoccerTeam.h"
#include <string>
#include <map>
#include "Goal.h"
#include "SoccerPitch.h"

using namespace std;

class TeamMaker;

typedef map<string, TeamMaker*> MakerMap;

class TeamMaker {
public:
	static AbstSoccerTeam* newTeam(string name,
			 Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color);
	static MakerMap* registry;
protected:
	TeamMaker (string className);

	virtual AbstSoccerTeam* makeTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const=0;
};

//#include "SoccerTeamMaker.h"
#endif