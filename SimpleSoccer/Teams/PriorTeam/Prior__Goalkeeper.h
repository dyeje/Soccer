#ifndef Prior__GOALY_H
#define Prior__GOALY_H

#include "2D/Vector2D.h"
#include "../../Goalkeeper.h"
#include "FSM/StateMachine.h"
#include "../../AbstSoccerTeam.h"

class GoalKeeper;

class Prior__GoalKeeper : public GoalKeeper {

public:
  
  Prior__GoalKeeper(AbstSoccerTeam*    home_team,
                    int                home_region,
                    State<GoalKeeper>* start_state,
                    State<GoalKeeper>* global_state,
                    Vector2D           heading,
                    Vector2D           velocity,
                    double             mass,
                    double             max_force,
                    double             max_speed,
                    double             max_turn_rate,
                    double             scale);

};

#endif
