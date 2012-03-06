#pragma warning (disable:4786)
#ifndef BLAZERSFIELDPLAYER_H
#define BLAZERSFIELDPLAYER_H

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "2D/Vector2D.h"
#include "FSM/StateMachine.h"
#include "../../PlayerBase.h"
#include "../../FieldPlayer.h"
#include "FSM/StateMachine.h"
#include "time/Regulator.h"

class CSteeringBehavior;
class AbstSoccerTeam;
class SoccerPitch;
class Goal;
struct Telegram;

class BlazersFieldPlayer : public FieldPlayer {

public:

  BlazersFieldPlayer(AbstSoccerTeam*    home_team,
             int        home_region,
             State<FieldPlayer>* start_state,
             State<FieldPlayer>* global_state,
             Vector2D  heading,
             Vector2D      velocity,
             double         mass,
             double         max_force,
             double         max_speed,
             double         max_turn_rate,
             double         scale,
             player_role role);
  
  void ForcePosition(double x, double y);
  int HomeRegion();

  State<FieldPlayer>* init_start_state;

};

#endif
