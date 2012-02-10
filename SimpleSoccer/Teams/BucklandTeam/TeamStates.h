#ifndef TEAMSTATES_H
#define TEAMSTATES_H

#define	DEBUG_TEAM_STATES
//------------------------------------------------------------------------
//
//  Name: TeamStates.h
//
//  Desc: State prototypes for soccer team states
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>

#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class AbstSoccerTeam;



//------------------------------------------------------------------------
class Attacking : public State<AbstSoccerTeam>
{ 
private:
  
  Attacking(){}

public:

  //this is a singleton
  static Attacking* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Defending : public State<AbstSoccerTeam>
{ 
private:
  
  Defending(){}

public:

    //this is a singleton
  static Defending* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class PrepareForKickOff : public State<AbstSoccerTeam>
{ 
private:
  
  PrepareForKickOff(){}

public:

    //this is a singleton
  static PrepareForKickOff* Instance();
  
  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};


#endif