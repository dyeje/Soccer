#include "Prior__TeamMaker.h"


const Prior__TeamMaker Prior__TeamMaker::registerThis;

Prior__TeamMaker::Prior__TeamMaker() : TeamMaker("Prior__Team") {}

AbstSoccerTeam* Prior__TeamMaker::makeTeam(Goal* home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const
   { 
      return new Prior__Team(home_goal, opponents_goal, pitch, color); 
   }
