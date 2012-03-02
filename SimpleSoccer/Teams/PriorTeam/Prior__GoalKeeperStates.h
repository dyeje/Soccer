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


class Prior__GlobalKeeperState: public State<GoalKeeper>
{
private:
  
  Prior__GlobalKeeperState(){}

public:

  //this is a singleton
  static Prior__GlobalKeeperState* Instance();

  void Enter(GoalKeeper* keeper){}

  void Execute(GoalKeeper* keeper){}

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&);
};

//-----------------------------------------------------------------------------

class Prior__TendGoal: public State<GoalKeeper>
{
private:
  
  Prior__TendGoal(){}

public:

  //this is a singleton
  static Prior__TendGoal* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__InterceptBall: public State<GoalKeeper>
{
private:
  
  Prior__InterceptBall(){}

public:

  //this is a singleton
  static Prior__InterceptBall* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__ReturnHome: public State<GoalKeeper>
{
private:
  
  Prior__ReturnHome(){}

public:

  //this is a singleton
  static Prior__ReturnHome* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__PutBallBackInPlay: public State<GoalKeeper>
{
private:
  
  Prior__PutBallBackInPlay(){}

public:

  //this is a singleton
  static Prior__PutBallBackInPlay* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};





#endif
