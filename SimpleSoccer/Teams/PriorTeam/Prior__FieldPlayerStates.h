#ifndef FIELDPLAYERSTATES_H
#define FIELDPLAYERSTATES_H
//------------------------------------------------------------------------
//
//  Name: Prior__FieldPlayerStates.h
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
class Prior__GlobalPlayerState : public State<FieldPlayer>
{
private:
  
  Prior__GlobalPlayerState(){}

public:

  //this is a singleton
  static Prior__GlobalPlayerState* Instance();

  void Enter(FieldPlayer* player){}

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&);
};

//------------------------------------------------------------------------
class Prior__ChaseBall : public State<FieldPlayer>
{
private:
  
  Prior__ChaseBall(){}

public:

  //this is a singleton
  static Prior__ChaseBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__Dribble : public State<FieldPlayer>
{
private:
  
  Prior__Dribble(){}

public:

  //this is a singleton
  static Prior__Dribble* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class Prior__ReturnToHomeRegion: public State<FieldPlayer>
{
private:
  
  Prior__ReturnToHomeRegion(){}

public:

  //this is a singleton
  static Prior__ReturnToHomeRegion* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__Wait: public State<FieldPlayer>
{
private:
  
  Prior__Wait(){}

public:

  //this is a singleton
  static Prior__Wait* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__KickBall: public State<FieldPlayer>
{
private:
  
  Prior__KickBall(){}

public:

  //this is a singleton
  static Prior__KickBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class Prior__ReceiveBall: public State<FieldPlayer>
{
private:
  
  Prior__ReceiveBall(){}

public:

  //this is a singleton
  static Prior__ReceiveBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class Prior__SupportAttacker: public State<FieldPlayer>
{
private:
  
  Prior__SupportAttacker(){}

public:

  //this is a singleton
  static Prior__SupportAttacker* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};




  
#endif
