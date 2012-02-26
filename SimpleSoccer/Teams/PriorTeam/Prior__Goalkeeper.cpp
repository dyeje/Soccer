#include "Prior__Goalkeeper.h"
#include "misc/Cgdi.h"
#include "../../SteeringBehaviors.h"
#include "../../AbstSoccerTeam.h"
#include "../../SoccerPitch.h"
#include "2D/transformations.h"
#include "../../Goal.h"
#include "game/EntityFunctionTemplates.h"
#include "../../ParamLoader.h"

//----------------------------- ctor ------------------------------------
//-----------------------------------------------------------------------
Prior__GoalKeeper::Prior__GoalKeeper(AbstSoccerTeam*        home_team,
                              int                home_region,
                              State<GoalKeeper>* start_state,
                              State<GoalKeeper>* global_state,
                              Vector2D           heading,
                              Vector2D           velocity,
                              double              mass,
                              double              max_force,
                              double              max_speed,
                              double              max_turn_rate,
                              double              scale): GoalKeeper(home_team,
                                                             home_region,
                                                             start_state,
                                                             global_state,
                                                             heading,
                                                             velocity,
                                                             mass,
                                                             max_force,
                                                             max_speed,
                                                             max_turn_rate,
                                                             scale)
                                        
{   
}
