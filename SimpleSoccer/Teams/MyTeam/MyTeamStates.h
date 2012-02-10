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
class MyAttacking : public State<AbstSoccerTeam>
{ 
private:
  
  MyAttacking(){}

public:

  //this is a singleton
  static MyAttacking* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class MyDefending : public State<AbstSoccerTeam>
{ 
private:
  
  MyDefending(){}

public:

    //this is a singleton
  static MyDefending* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class MyPrepareForKickOff : public State<AbstSoccerTeam>
{ 
private:
  
  MyPrepareForKickOff(){}

public:

    //this is a singleton
  static MyPrepareForKickOff* Instance();
  
  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};


#endif