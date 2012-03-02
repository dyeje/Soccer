#ifndef KEEPERSTATES_H
#define KEEPERSTATES_H
//------------------------------------------------------------------------
//
//  Name: GoalKeeperStates.h
//
//  Desc:   Declarations of all the states used by a Simple Soccer
//          goalkeeper
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class GoalKeeper;
class SoccerPitch;


class BlazersGlobalKeeperState: public State<GoalKeeper>
{
private:
  
  BlazersGlobalKeeperState(){}

public:

  //this is a singleton
  static BlazersGlobalKeeperState* Instance();

  void Enter(GoalKeeper* keeper){}

  void Execute(GoalKeeper* keeper){}

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&);
};

//-----------------------------------------------------------------------------

class BlazersTendGoal: public State<GoalKeeper>
{
private:
  
  BlazersTendGoal(){}

public:

  //this is a singleton
  static BlazersTendGoal* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersInterceptBall: public State<GoalKeeper>
{
private:
  
  BlazersInterceptBall(){}

public:

  //this is a singleton
  static BlazersInterceptBall* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersReturnHome: public State<GoalKeeper>
{
private:
  
  BlazersReturnHome(){}

public:

  //this is a singleton
  static BlazersReturnHome* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersPutBallBackInPlay: public State<GoalKeeper>
{
private:
  
  BlazersPutBallBackInPlay(){}

public:

  //this is a singleton
  static BlazersPutBallBackInPlay* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};





#endif
