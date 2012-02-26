#include "../../FieldPlayer.h"
#include "Prior__FieldPlayer.h"
#include "../../PlayerBase.h"
#include "../../SteeringBehaviors.h"
#include "2D/Transformations.h"
#include "2D/Geometry.h"
#include "misc/Cgdi.h"
#include "2D/C2DMatrix.h"
#include "../../Goal.h"
#include "Game/Region.h"
#include "game/EntityFunctionTemplates.h"
#include "../../ParamLoader.h"
#include "../../AbstSoccerTeam.h"
#include "time/Regulator.h"
#include "Debug/DebugConsole.h"

#include <limits>

using std::vector;

//----------------------------- ctor -------------------------------------
//------------------------------------------------------------------------
Prior__FieldPlayer::Prior__FieldPlayer(AbstSoccerTeam* home_team,
                      int   home_region,
                      State<FieldPlayer>* start_state,
          					  State<FieldPlayer>* global_state,
                      Vector2D  heading,
                      Vector2D velocity,
                      double    mass,
                      double    max_force,
                      double    max_speed,
                      double    max_turn_rate,
                      double    scale,
                      player_role role): FieldPlayer(home_team,
                                                    home_region,
                                                    start_state,
                                                    global_state,
                                                    heading,
                                                    velocity,
                                                    mass,
                                                    max_force,
                                                    max_speed,
                                                    max_turn_rate,
                                                    scale,
                                                    role)                                    
{
}

void Prior__FieldPlayer::ForcePosition(int x, int y) {
  m_vPosition.x = x;
  m_vPosition.y = y;
}

int Prior__FieldPlayer::HomeRegion() {
  return m_iHomeRegion;
}
  
