#include "MySoccerTeamMaker.h"


const MySoccerTeamMaker MySoccerTeamMaker::registerThis;

MySoccerTeamMaker::MySoccerTeamMaker() : TeamMaker("MySoccerTeam") {}

AbstSoccerTeam* MySoccerTeamMaker::makeTeam(Goal* home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const
   { 
      return new MySoccerTeam(home_goal, opponents_goal, pitch, color); 
   }