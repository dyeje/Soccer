#ifndef MYTEAMSTATES_H
#define MYTEAMSTATES_H

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
class Prior__Attacking : public State<AbstSoccerTeam>
{ 
private:
  
  Prior__Attacking(){}

public:

  //this is a singleton
  static Prior__Attacking* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__Defending : public State<AbstSoccerTeam>
{ 
private:
  
  Prior__Defending(){}

public:

    //this is a singleton
  static Prior__Defending* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__PrepareForKickOff : public State<AbstSoccerTeam>
{ 
private:
  
  Prior__PrepareForKickOff(){}

public:

    //this is a singleton
  static Prior__PrepareForKickOff* Instance();
  
  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};


#endif
