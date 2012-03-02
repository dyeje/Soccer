#include "BlazersGoalKeeperStates.h"
#include "Debug/DebugConsole.h"
#include "../../SoccerPitch.h"
#include "../../PlayerBase.h"
#include "../../GoalKeeper.h"
#include "../../SteeringBehaviors.h"
#include "BlazersTeam.h"
#include "../../Goal.h"
#include "2D/geometry.h"
#include "../../FieldPlayer.h"
#include "../../ParamLoader.h"
#include "Messaging/Telegram.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"


//uncomment to send state info to debug window
//#define GOALY_STATE_INFO_ON


//--------------------------- BlazersGlobalKeeperState -------------------------------
//-----------------------------------------------------------------------------

BlazersGlobalKeeperState* BlazersGlobalKeeperState::Instance()
{
  static BlazersGlobalKeeperState instance;

  return &instance;
}


bool BlazersGlobalKeeperState::OnMessage(GoalKeeper* keeper, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
    case Msg_GoHome:
    {
      keeper->SetDefaultHomeRegion();
      
      keeper->GetFSM()->ChangeState(BlazersReturnHome::Instance());
    }

    break;

    case Msg_ReceiveBall:
      {
        keeper->GetFSM()->ChangeState(BlazersInterceptBall::Instance());
      }

      break;

  }//end switch

  return false;
}


//--------------------------- BlazersTendGoal -----------------------------------
//
//  This is the main state for the goalkeeper. When in this state he will
//  move left to right across the goalmouth using the 'interpose' steering
//  behavior to put himself between the ball and the back of the net.
//
//  If the ball comes within the 'goalkeeper range' he moves out of the
//  goalmouth to attempt to intercept it. (see next state)
//------------------------------------------------------------------------

BlazersTendGoal* BlazersTendGoal::Instance()
{
  static BlazersTendGoal instance;

  return &instance;
}


void BlazersTendGoal::Enter(GoalKeeper* keeper)
{
  //turn interpose on
  keeper->Steering()->InterposeOn(Prm.GoalKeeperTendingDistance);

  //interpose will position the agent between the ball position and a target
  //position situated along the goal mouth. This call sets the target
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

void BlazersTendGoal::Execute(GoalKeeper* keeper)
{
  // target = max(fabs(goalYMin - goalyPos.x), fabs(goalYMax - goalyPos.x));
	//ShotTarget.y = target;

 //   //make sure striking the ball with the given power is enough to drive
 //   //the ball over the goal line.
 //   double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
 //                                                     ShotTarget,
 //                                                     power);
    
  //  //if it is, this shot is then tested to see if any of the opponents
  //  //can intercept it.
  //  if (time >= 0)
  //  {
  //    if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
  //    {
  //      return true;
  //    }
  //  }
  
  //the rear interpose target will change as the ball's position changes
  //so it must be updated each update-step 
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

  //if the ball comes in range the keeper traps it and then changes state
  //to put the ball back in play
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();

    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(BlazersPutBallBackInPlay::Instance());

    return;
  }

  //if ball is within a predefined distance, the keeper moves out from
  //position to try and intercept it.
  if (keeper->BallWithinRangeForIntercept() && !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(BlazersInterceptBall::Instance());
  }

  //if the keeper has ventured too far away from the goal-line and there
  //is no threat from the opponents he should move back towards it
  if (keeper->TooFarFromGoalMouth() && keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(BlazersReturnHome::Instance());

    return;
  }
}


void BlazersTendGoal::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->InterposeOff();
}


//------------------------- BlazersReturnHome: ----------------------------------
//
//  In this state the goalkeeper simply returns back to the center of
//  the goal region before changing state back to BlazersTendGoal
//------------------------------------------------------------------------

BlazersReturnHome* BlazersReturnHome::Instance()
{
  static BlazersReturnHome instance;

  return &instance;
}


void BlazersReturnHome::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOn();
}

void BlazersReturnHome::Execute(GoalKeeper* keeper)
{
  keeper->Steering()->SetTarget(keeper->HomeRegion()->Center());

  //if close enough to home or the opponents get control over the ball,
  //change state to tend goal
  if (keeper->InHomeRegion() || !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(BlazersTendGoal::Instance());
  }
}

void BlazersReturnHome::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOff();
}



//----------------- BlazersInterceptBall ----------------------------------------
//
//  In this state the GP will attempt to intercept the ball using the
//  pursuit steering behavior, but he only does so so long as he remains
//  within his home region.
//------------------------------------------------------------------------

BlazersInterceptBall* BlazersInterceptBall::Instance()
{
  static BlazersInterceptBall instance;

  return &instance;
}


void BlazersInterceptBall::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOn();  

    #ifdef GOALY_STATE_INFO_ON
    debug_con << "Goaly " << keeper->ID() << " enters BlazersInterceptBall" <<  "";
    #endif
}

void BlazersInterceptBall::Execute(GoalKeeper* keeper)
{ 
  //if the goalkeeper moves to far away from the goal he should return to his
  //home region UNLESS he is the closest player to the ball, in which case,
  //he should keep trying to intercept it.
  if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
  {
    keeper->GetFSM()->ChangeState(BlazersReturnHome::Instance());

    return;
  }
  
  //if the ball becomes in range of the goalkeeper's hands he traps the 
  //ball and puts it back in play
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();
    
    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(BlazersPutBallBackInPlay::Instance());

    return;
  }
}

void BlazersInterceptBall::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOff();
}



//--------------------------- BlazersPutBallBackInPlay --------------------------
//
//------------------------------------------------------------------------

BlazersPutBallBackInPlay* BlazersPutBallBackInPlay::Instance()
{
  static BlazersPutBallBackInPlay instance;

  return &instance;
}

void BlazersPutBallBackInPlay::Enter(GoalKeeper* keeper)
{
  //let the team know that the keeper is in control
  keeper->Team()->SetControllingPlayer(keeper);

  //send all the players home
  keeper->Team()->Opponents()->ReturnAllFieldPlayersToHome();
  keeper->Team()->ReturnAllFieldPlayersToHome();
}


void BlazersPutBallBackInPlay::Execute(GoalKeeper* keeper)
{
  PlayerBase*  receiver = NULL;
  Vector2D     BallTarget;
    
  //test if there are players further forward on the field we might
  //be able to pass to. If so, make a pass.
  if (keeper->Team()->FindPass(keeper,
                              receiver,
                              BallTarget,
                              Prm.MaxPassingForce,
                              Prm.GoalkeeperMinPassDist))
  {     
    //make the pass   
    keeper->Ball()->Kick(Vec2DNormalize(BallTarget - keeper->Ball()->Pos()),
                         Prm.MaxPassingForce);

    //goalkeeper no longer has ball 
    keeper->Pitch()->SetGoalKeeperHasBall(false);

    //let the receiving player know the ball's comin' at him
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                          keeper->ID(),
                          receiver->ID(),
                          Msg_ReceiveBall,
                          &BallTarget);
    
    //go back to tending the goal   
    keeper->GetFSM()->ChangeState(BlazersTendGoal::Instance());

    return;
  }  

  keeper->SetVelocity(Vector2D());
}

