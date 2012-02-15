#include "BlazersTeamMaker.h"


const BlazersTeamMaker BlazersTeamMaker::registerThis;

BlazersTeamMaker::BlazersTeamMaker() : TeamMaker("BlazersTeam") {}

AbstSoccerTeam* BlazersTeamMaker::makeTeam(Goal* home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const
   { 
      return new BlazersTeam(home_goal, opponents_goal, pitch, color); 
   }
