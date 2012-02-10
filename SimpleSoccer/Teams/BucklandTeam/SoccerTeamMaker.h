#ifndef SOCCERTEAMMAKER_H
#define SOCCERTEAMMAKER_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   SoccerTeamMaker.h
//
//  Desc:   Factory class for SoccerTeam. 
//
//  Author: Robin Burke 2005 rburke@cs.depaul.edu
//
//------------------------------------------------------------------------

#include "../../TeamMaker.h"
#include "SoccerTeam.h"
#include <string>
#include "../../Goal.h"
#include "../../SoccerPitch.h"

class SoccerTeamMaker : public TeamMaker {
private:
   SoccerTeamMaker();

   SoccerTeam* makeTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 SoccerTeam::team_color   color) const;
	
   // creation of static instance does automatic registration
    static const SoccerTeamMaker registerThis;
};
#endif