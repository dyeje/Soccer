#include "SoccerTeamMaker.h"


const SoccerTeamMaker SoccerTeamMaker::registerThis;

SoccerTeamMaker::SoccerTeamMaker() : TeamMaker("BucklandTeam") {}

SoccerTeam* SoccerTeamMaker::makeTeam(Goal* home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 SoccerTeam::team_color   color) const
   { 
      return new SoccerTeam(home_goal, opponents_goal, pitch, color); 
   }