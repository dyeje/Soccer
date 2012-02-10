#include "TeamMaker.h"
#include <string>
#include <map>
#include "AbstSoccerTeam.h"

MakerMap* TeamMaker::registry;

TeamMaker::TeamMaker (std::string className)
	{
		if (registry == NULL)
			registry = new MakerMap();
		registry->insert( std::make_pair(className, this) );
	}

AbstSoccerTeam* TeamMaker::newTeam (std::string name,
			 Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color)
{
	MakerMap::iterator iter = registry->find(name);
	if (iter != registry->end())
	// Get the TeamMaker object and call the factory method
	{
		TeamMaker* maker = (*iter).second;
		return maker->makeTeam(home_goal, opponents_goal, pitch, color);
	} else
	{
		return NULL;
	}
}