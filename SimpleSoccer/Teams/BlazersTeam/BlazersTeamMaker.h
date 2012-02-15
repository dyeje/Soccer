#ifndef MYSOCCERTEAMMAKER_H
#define MYSOCCERTEAMMAKER_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   BlazersTeamMaker.h
//
//  Desc:   Factory class for SoccerTeam. 
//
//  Author: Robin Burke 2005 rburke@cs.depaul.edu
//
//------------------------------------------------------------------------

#include "../../TeamMaker.h"
#include "BlazersTeam.h"
#include <string>
#include "../../Goal.h"
#include "../../SoccerPitch.h"

class BlazersTeamMaker : public TeamMaker {
private:
   BlazersTeamMaker();

   AbstSoccerTeam* makeTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const;
	
   // creation of static instance does automatic registration
    static const BlazersTeamMaker registerThis;
};
#endif
