#ifndef FIELDPLAYERSTATES_H
#define FIELDPLAYERSTATES_H
//------------------------------------------------------------------------
//
//  Name: BlazersFieldPlayerStates.h
//
//  Desc: States for the field players of Simple Soccer. See my book
//        for detailed descriptions
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class FieldPlayer;
class SoccerPitch;


//------------------------------------------------------------------------
class BlazersGlobalPlayerState : public State<FieldPlayer>
{
private:
  
  BlazersGlobalPlayerState(){}

public:

  //this is a singleton
  static BlazersGlobalPlayerState* Instance();

  void Enter(FieldPlayer* player){}

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&);
};

//------------------------------------------------------------------------
class BlazersChaseBall : public State<FieldPlayer>
{
private:
  
  BlazersChaseBall(){}

public:

  //this is a singleton
  static BlazersChaseBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersDribble : public State<FieldPlayer>
{
private:
  
  BlazersDribble(){}

public:

  //this is a singleton
  static BlazersDribble* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class BlazersReturnToHomeRegion: public State<FieldPlayer>
{
private:
  
  BlazersReturnToHomeRegion(){}

public:

  //this is a singleton
  static BlazersReturnToHomeRegion* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersWait: public State<FieldPlayer>
{
private:
  
  BlazersWait(){}

public:

  //this is a singleton
  static BlazersWait* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersKickBall: public State<FieldPlayer>
{
private:
  
  BlazersKickBall(){}

public:

  //this is a singleton
  static BlazersKickBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class BlazersReceiveBall: public State<FieldPlayer>
{
private:
  
  BlazersReceiveBall(){}

public:

  //this is a singleton
  static BlazersReceiveBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class BlazersSupportAttacker: public State<FieldPlayer>
{
private:
  
  BlazersSupportAttacker(){}

public:

  //this is a singleton
  static BlazersSupportAttacker* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};




  
#endif
