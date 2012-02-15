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
class BlazersAttacking : public State<AbstSoccerTeam>
{ 
private:
  
  BlazersAttacking(){}

public:

  //this is a singleton
  static BlazersAttacking* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersDefending : public State<AbstSoccerTeam>
{ 
private:
  
  BlazersDefending(){}

public:

    //this is a singleton
  static BlazersDefending* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersPrepareForKickOff : public State<AbstSoccerTeam>
{ 
private:
  
  BlazersPrepareForKickOff(){}

public:

    //this is a singleton
  static BlazersPrepareForKickOff* Instance();
  
  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};


#endif
