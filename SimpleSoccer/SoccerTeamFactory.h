#ifndef SOCCERTEAMFACTORY_H
#define SOCCERTEAMFACTORY_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   SoccerTeamFactory.h
//
//  Desc:   class allows the creation of multiple soccer teams. No fancy
//          run-time binding here. The teams must be compiled in.
//
//  Author: Robin Burke 2005 rburke@cs.depaul.edu
//
//------------------------------------------------------------------------

#include "SoccerTeam.h"
#include <string>
#include <map>
#include "TeamMaker.h"

class SoccerTeam;

                
class SoccerTeamFactory
{
public:
  
	MakeSoccerTeam(std::string		team_id,
			 Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);


private:

   //the 

};

#endif