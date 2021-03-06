#include "Prior__FieldPlayerStates.h"
#include "Debug/DebugConsole.h"
#include "../../SoccerPitch.h"
#include "../../FieldPlayer.h"
#include "../../SteeringBehaviors.h"
#include "Prior__Team.h"
#include "Prior__FieldPlayer.h"
#include "Prior__States.h"
#include "../../Goal.h"
#include "2D/geometry.h"
#include "../../SoccerBall.h"
#include "../../ParamLoader.h"
#include "Messaging/Telegram.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"

#include "time/Regulator.h"


//uncomment below to send state info to the debug window
//#define PLAYER_STATE_INFO_ON


//************************************************************************ Global state

Prior__GlobalPlayerState* Prior__GlobalPlayerState::Instance()
{
  static Prior__GlobalPlayerState instance;

  return &instance;
}


void Prior__GlobalPlayerState::Execute(FieldPlayer* player)                                     
{
  //if a player is in possession and close to the ball reduce his max speed
  if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
  {
    player->SetMaxSpeed(Prm.PlayerMaxSpeedWithBall);
  }

  else
  {
     player->SetMaxSpeed(Prm.PlayerMaxSpeedWithoutBall);
  }
    
}


bool Prior__GlobalPlayerState::OnMessage(FieldPlayer* player, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
  case Msg_ReceiveBall:
    {
      //set the target
      player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

      //change state 
      player->GetFSM()->ChangeState(Prior__ReceiveBall::Instance());

      return true;
    }

    break;

  case Msg_SupportAttacker:
    {
      Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
      if(plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14)
        return true;

      //if already supporting just return
      if (player->GetFSM()->isInState(*Prior__SupportAttacker::Instance()))
      {
        return true;
      }
      
      //set the target to be the best supporting position
      player->Steering()->SetTarget(static_cast<Prior__Team*>(player->Team())->GetSupportSpot());
      // player->Steering()->SetTarget(player->Team()->GetSupportSpot());

      //change the state
      player->GetFSM()->ChangeState(Prior__SupportAttacker::Instance());

      return true;
    }

    break;

 case Msg_Wait:
    {
      //change the state
      player->GetFSM()->ChangeState(Prior__Wait::Instance());

      return true;
    }

    break;

  case Msg_GoHome:
    {
      player->SetDefaultHomeRegion();
      
      player->GetFSM()->ChangeState(Prior__ReturnToHomeRegion::Instance());

      return true;
    }

    break;

  case Msg_PassToMe:
    {  
      
      //get the position of the player requesting the pass 
      FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

      #ifdef PLAYER_STATE_INFO_ON
      debug_con << "Player " << player->ID() << " received request from " <<
                    receiver->ID() << " to make pass" << "";
      #endif

      //if the ball is not within kicking range or their is already a 
      //receiving player, this player cannot pass the ball to the player
      //making the request.
      if (player->Team()->Receiver() != NULL ||
         !player->BallWithinKickingRange() )
      {
        #ifdef PLAYER_STATE_INFO_ON
        debug_con << "Player " << player->ID() << " cannot make requested pass <cannot kick ball>" << "";
        #endif

        return true;
      }
      
      //make the pass   
      player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
                           Prm.MaxPassingForce);

          
     #ifdef PLAYER_STATE_INFO_ON
     debug_con << "Player " << player->ID() << " Passed ball to requesting player" << "";
     #endif
        
      //let the receiver know a pass is coming 
      Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                              player->ID(),
                              receiver->ID(),
                              Msg_ReceiveBall,
                              &receiver->Pos());

   

      //change state   
      player->GetFSM()->ChangeState(Prior__Wait::Instance());

      player->FindSupport();

      return true;
    }

    break;
  }//end switch

  return false;
}
                                

       

//***************************************************************************** CHASEBALL

Prior__ChaseBall* Prior__ChaseBall::Instance()
{
  static Prior__ChaseBall instance;

  return &instance;
}


void Prior__ChaseBall::Enter(FieldPlayer* player)
{
  player->Steering()->SeekOn();

  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters chase state" << "";
  #endif
}

void Prior__ChaseBall::Execute(FieldPlayer* player)                                     
{
  //if the ball is within kicking range the player changes state to KickBall.
  if (player->BallWithinKickingRange())
  {
    player->GetFSM()->ChangeState(Prior__KickBall::Instance());
    
    return;
  }

  Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
  if((plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14) 
    && fabs(plyr->Team()->HomeGoal()->Center().x - plyr->Pos().x) > 337  //plyr->Pitch()->cxClient()/2 )
    && player->Pitch()->GameOn() )
    player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());

  // Default to chase ball, a mutch better defense
  player->Steering()->SetTarget(player->Ball()->Pos());

  //if the player is the closest player to the ball then he should keep
  //chasing it
  //if (player->isClosestTeamMemberToBall())
  //{
    //player->Steering()->SetTarget(player->Ball()->Pos());

    //return;
  //}
    
  //if the player is not closest to the ball anymore, he should return back
  //to his home region and wait for another opportunity
  //player->GetFSM()->ChangeState(Prior__ReturnToHomeRegion::Instance());
}


void Prior__ChaseBall::Exit(FieldPlayer* player)
{
  player->Steering()->SeekOff();
}



//*****************************************************************************SUPPORT ATTACKING PLAYER

Prior__SupportAttacker* Prior__SupportAttacker::Instance()
{
  static Prior__SupportAttacker instance;

  return &instance;
}


void Prior__SupportAttacker::Enter(FieldPlayer* player)
{
  player->Steering()->ArriveOn();

  player->Steering()->SetTarget(static_cast<Prior__Team*>(player->Team())->GetSupportSpot());
  
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters support state" << "";
  #endif
}

void Prior__SupportAttacker::Execute(FieldPlayer* player)                                     
{
  //if his team loses control go back home
  if (!player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(Prior__ReturnToHomeRegion::Instance()); return;
  } 
  //Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
  //if((plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14) 
  //  && fabs(plyr->Team()->HomeGoal()->Center().x - plyr->Pos().x) > plyr->Pitch()->cxClient() / 2 )
  //  player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());


  //if the best supporting spot changes, change the steering target
  Vector2D supportSpot = static_cast<Prior__Team*>(player->Team())->GetSupportSpot();
  if (supportSpot != player->Steering()->Target())
  {    
    player->Steering()->SetTarget(supportSpot);

    player->Steering()->ArriveOn();
  }

  //if this player has a shot at the goal AND the attacker can pass
  //the ball to him the attacker should pass the ball to this player
  if( player->Team()->CanShoot(player->Pos(),
                               Prm.MaxShootingForce))
  {
    player->Team()->RequestPass(player);
  }


  //if this player is located at the support spot and his team still have
  //possession, he should remain still and turn to face the ball
  if (player->AtTarget())
  {
    player->Steering()->ArriveOff();
        
    //the player should keep his eyes on the ball!
    player->TrackBall();

    player->SetVelocity(Vector2D(0,0));

    //if not threatened by another player request a pass
    if (!player->isThreatened())
    {
      player->Team()->RequestPass(player);
    }
  }
}


void Prior__SupportAttacker::Exit(FieldPlayer* player)
{
  //set supporting player to null so that the team knows it has to 
  //determine a new one.
  player->Team()->SetSupportingPlayer(NULL);

  player->Steering()->ArriveOff();
}




//************************************************************************ RETURN TO HOME REGION

Prior__ReturnToHomeRegion* Prior__ReturnToHomeRegion::Instance()
{
  static Prior__ReturnToHomeRegion instance;

  return &instance;
}


void Prior__ReturnToHomeRegion::Enter(FieldPlayer* player)
{
  player->Steering()->ArriveOn();

  if (!player->HomeRegion()->Inside(player->Steering()->Target(), Region::halfsize))
  {
    player->Steering()->SetTarget(player->HomeRegion()->Center());
  }

  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters ReturnToHome state" << "";
  #endif
}

void Prior__ReturnToHomeRegion::Execute(FieldPlayer* player)
{
  if (player->Pitch()->GameOn())
  {
    //if the ball is nearer this player than any other team member  &&
    //there is not an assigned receiver && the goalkeeper does not gave
    //the ball, go chase it
    if ( player->isClosestTeamMemberToBall() &&
         (player->Team()->Receiver() == NULL) &&
         !player->Pitch()->GoalKeeperHasBall())
    {
      player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());

      return;
    }
  }

  //if game is on and close enough to home, change state to wait and set the 
  //player target to his current position.(so that if he gets jostled out of 
  //position he can move back to it)
  if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
                                                             Region::halfsize))
  {
    player->Steering()->SetTarget(player->Pos());
    player->GetFSM()->ChangeState(Prior__Wait::Instance());
  }
  //if game is not on the player must return much closer to the center of his
  //home region
  else if(!player->Pitch()->GameOn() && player->AtTarget())
  {
    // Doing the following cuases the defense meister to not enter wait,
    // and the game pauses with all the other players in the home region -
    // the prep for kick off must require they all be in wait state to start
    //
    // If player is designated defense meister, place them back in that
    // state instead of wait.
    //Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
    //if((plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14) &&
    //    !player->Team()->GetFSM()->isInState(*Prior__PrepareForKickOff::Instance()) )
    //  player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());
    //else
      player->GetFSM()->ChangeState(Prior__Wait::Instance());
  }
}

void Prior__ReturnToHomeRegion::Exit(FieldPlayer* player)
{
  // player->Steering()->ArriveOff();
  // If player is designated defense meister, place them back in that state
  // Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
  // if(plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14)
  //   player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());
  // else
    player->Steering()->ArriveOff();
 }




//***************************************************************************** WAIT

Prior__Wait* Prior__Wait::Instance()
{
  static Prior__Wait instance;

  return &instance;
}


void Prior__Wait::Enter(FieldPlayer* player)
{
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters wait state" << "";
  #endif

  //if the game is not on make sure the target is the center of the player's
  //home region. This is ensure all the players are in the correct positions
  //ready for kick off
  if (!player->Pitch()->GameOn())
  {
    player->Steering()->SetTarget(player->HomeRegion()->Center());
  }
}

void Prior__Wait::Execute(FieldPlayer* player)
{    
  //if the player has been jostled out of position, get back in position  
  if (!player->AtTarget())
  {
    player->Steering()->ArriveOn();

    return;
  }

  else
  {
    player->Steering()->ArriveOff();

    player->SetVelocity(Vector2D(0,0));

    //the player should keep his eyes on the ball!
    player->TrackBall();
  }

  Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
  if((plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14)
    //&& ( !plyr->Team()->GetFSM()->isInState(*Prior__PrepareForKickOff::Instance())) 
    && plyr->Pitch()->GameOn() ) {
      player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());
  }

  //if this player's team is controlling AND this player is not the attacker
  //AND is further up the field than the attacker he should request a pass.
  if ( player->Team()->InControl()    &&
     (!player->isControllingPlayer()) &&
       player->isAheadOfAttacker() )
  {
    player->Team()->RequestPass(player);

    return;
  }

  if (player->Pitch()->GameOn())
  {
   //if the ball is nearer this player than any other team member  AND
    //there is not an assigned receiver AND neither goalkeeper has
    //the ball, go chase it
   if (player->isClosestTeamMemberToBall() &&
       player->Team()->Receiver() == NULL  &&
       !player->Pitch()->GoalKeeperHasBall())
   {
     player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());

     return;
   }
  } 

  // Improvement #5, active defense.
  if (!player->Team()->InControl() && 
      !player->Team()->GetFSM()->isInState(*Prior__PrepareForKickOff::Instance()))
  {
    player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
  }

}

void Prior__Wait::Exit(FieldPlayer* player){}




//************************************************************************ KICK BALL

Prior__KickBall* Prior__KickBall::Instance()
{
  static Prior__KickBall instance;

  return &instance;
}


void Prior__KickBall::Enter(FieldPlayer* player)
{
  //let the team know this player is controlling
   player->Team()->SetControllingPlayer(player);
   
   //the player can only make so many kick attempts per second.
   if (!player->isReadyForNextKick()) 
   {
     player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
   }

   
  #ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters kick state" << "";
  #endif
}

void Prior__KickBall::Execute(FieldPlayer* player)
{ 
  Prior__FieldPlayer* plyr = static_cast<Prior__FieldPlayer*>(player);
  bool defenseMeister = (plyr->HomeRegion() == 5 || plyr->HomeRegion() == 14);

  // Two types of Kicks are evaluated in order of importance: Shoot on 
  // goal and pass.  Perform them if possible, and if not, default to 
  // dribble.

  //calculate the dot product of the vector pointing to the ball
  //and the player's heading
  Vector2D ToBall = player->Ball()->Pos() - player->Pos();
  double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

  //cannot kick the ball if the goalkeeper is in possession or if it is 
  //behind the player or if there is already an assigned receiver. So just
  //continue chasing the ball
  if (player->Team()->Receiver() != NULL   ||
      player->Pitch()->GoalKeeperHasBall() ||
      (dot < 0) ) 
  {
    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Goaly has ball / ball behind player" << "";
    #endif
    
    player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());

    return;
  }

  /* 1) Attempt a shot at the goal */
  /* ----------------------------- */

  //if a shot is possible, this vector will hold the position along the 
  //opponent's goal line the player should aim for.
  Vector2D    BallTarget;

  //the dot product is used to adjust the shooting force. The more
  //directly the ball is ahead, the more forceful the kick
  double power = Prm.MaxShootingForce * dot;

  //if it is determined that the player could score a goal from this position
  //OR if he should just kick the ball anyway, the player will attempt
  //to make the shot
  if (player->Team()->CanShoot(player->Ball()->Pos(),
                               power,
                               BallTarget)                   || 
     (RandFloat() < Prm.ChancePlayerAttemptsPotShot))
  {
   #ifdef PLAYER_STATE_INFO_ON
   debug_con << "Player " << player->ID() << " attempts a shot at " << BallTarget << "";
   #endif

   //add some noise to the kick. We don't want players who are 
   //too accurate! The amount of noise can be adjusted by altering
   //Prm.PlayerKickingAccuracy
   BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

   //this is the direction the ball will be kicked in
   Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
   player->Ball()->Kick(KickDirection, power);
    
   //change state   
   //player->GetFSM()->ChangeState(Prior__Wait::Instance());
    if(defenseMeister && player->Pitch()->GameOn())
      player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());
    else
      player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
   
   player->FindSupport();
  
   return;
 }

 
  // 2) Attempt a pass to a player (original Buckland way)

  PlayerBase* receiver = NULL; //if a receiver is found this will point to it
  power = Prm.MaxPassingForce * dot;

  // If there are any potential candidates available to receive a pass, then pass
  //bool findPass = player->Team()->FindPass(player,
  //                            receiver,
  //                            BallTarget,
  //                            power,
  //                            Prm.MinPassDist);
  //bool passBall = (findPass && (defenseMeister || player->isThreatened())); 

  //if(passBall)
  if (player->isThreatened()  &&
      player->Team()->FindPass(player,
                              receiver,
                              BallTarget,
                              power,
                              Prm.MinPassDist))
  {     
    //add some noise to the kick
    BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

    Vector2D KickDirection = BallTarget - player->Ball()->Pos();
   
    player->Ball()->Kick(KickDirection, power);

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " passes the ball with force " << power << "  to player " 
              << receiver->ID() << "  Target is " << BallTarget << "";
    #endif

    
    //let the receiver know a pass is coming 
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                            player->ID(),
                            receiver->ID(),
                            Msg_ReceiveBall,
                            &BallTarget);                            
   

    //the player should wait at his current position unless instruced
    //otherwise  
    //player->GetFSM()->ChangeState(Prior__Wait::Instance());
    if(defenseMeister && plyr->Pitch()->GameOn())
      player->GetFSM()->ChangeState(Prior__DefenseMeister::Instance());
    else
      player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());

    player->FindSupport();

    return;
  }

  player->FindSupport();
  player->GetFSM()->ChangeState(Prior__Dribble::Instance());
}


//*************************************************************************** DRIBBLE

Prior__Dribble* Prior__Dribble::Instance()
{
  static Prior__Dribble instance;

  return &instance;
}


void Prior__Dribble::Enter(FieldPlayer* player)
{
  //let the team know this player is controlling
  player->Team()->SetControllingPlayer(player);

#ifdef PLAYER_STATE_INFO_ON
  debug_con << "Player " << player->ID() << " enters dribble state" << "";
  #endif
}

void Prior__Dribble::Execute(FieldPlayer* player)
{
  double dot = player->Team()->HomeGoal()->Facing().Dot(player->Heading());

  //if the ball is between the player and the home goal, it needs to swivel
  // the ball around by doing multiple small kicks and turns until the player 
  //is facing in the correct direction
  if (dot < 0)
  {
    //the player's heading is going to be rotated by a small amount (Pi/4) 
    //and then the ball will be kicked in that direction
    Vector2D direction = player->Heading();

    //calculate the sign (+/-) of the angle between the player heading and the 
    //facing direction of the goal so that the player rotates around in the 
    //correct direction
    double angle = QuarterPi * -1 *
                 player->Team()->HomeGoal()->Facing().Sign(player->Heading());

    Vec2DRotateAroundOrigin(direction, angle);

    //this value works well whjen the player is attempting to control the
    //ball and turn at the same time
    const double KickingForce = 0.8;

    player->Ball()->Kick(direction, KickingForce);
  }

  //kick the ball down the field
  else
  {
    player->Ball()->Kick(player->Team()->HomeGoal()->Facing(),
                         Prm.MaxDribbleForce);  
  }

  //the player has kicked the ball so he must now change state to follow it
  player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
    
  return;  
}



//************************************************************************     RECEIVEBALL

Prior__ReceiveBall* Prior__ReceiveBall::Instance()
{
  static Prior__ReceiveBall instance;

  return &instance;
}


void Prior__ReceiveBall::Enter(FieldPlayer* player)
{
  //let the team know this player is receiving the ball
  player->Team()->SetReceiver(player);
  
  //this player is also now the controlling player
  player->Team()->SetControllingPlayer(player);

  //there are two types of receive behavior. One uses arrive to direct
  //the receiver to the position sent by the passer in its telegram. The
  //other uses the pursuit behavior to pursue the ball. 
  //This statement selects between them dependent on the probability
  //ChanceOfUsingArriveTypeReceiveBehavior, whether or not an opposing
  //player is close to the receiving player, and whether or not the receiving
  //player is in the opponents 'hot region' (the third of the pitch closest
  //to the opponent's goal
  const double PassThreatRadius = 70.0;

  if (( player->InHotRegion() ||
        RandFloat() < Prm.ChanceOfUsingArriveTypeReceiveBehavior) &&
     !player->Team()->isOpponentWithinRadius(player->Pos(), PassThreatRadius))
  {
    player->Steering()->ArriveOn();
    
    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " enters receive state (Using Arrive)" << "";
    #endif
  }
  else
  {
    player->Steering()->PursuitOn();

    #ifdef PLAYER_STATE_INFO_ON
    debug_con << "Player " << player->ID() << " enters receive state (Using Pursuit)" << "";
    #endif
  }
}

void Prior__ReceiveBall::Execute(FieldPlayer* player)
{
  //if the ball comes close enough to the player or if his team lose control
  //he should change state to chase the ball
  if (player->BallWithinReceivingRange() || !player->Team()->InControl())
  {
    player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());

    return;
  }  

  if (player->Steering()->PursuitIsOn())
  {
    player->Steering()->SetTarget(player->Ball()->Pos());
  }

  //if the player has 'arrived' at the steering target he should wait and
  //turn to face the ball
  if (player->AtTarget())
  {
    player->Steering()->ArriveOff();
    player->Steering()->PursuitOff();
    player->TrackBall();    
    player->SetVelocity(Vector2D(0,0));
  } 
}

void Prior__ReceiveBall::Exit(FieldPlayer* player)
{
  player->Steering()->ArriveOff();
  player->Steering()->PursuitOff();

  player->Team()->SetReceiver(NULL);
}





Prior__DefenseMeister* Prior__DefenseMeister::Instance()
{
  static Prior__DefenseMeister instance;
  return &instance;
}


void Prior__DefenseMeister::Enter(FieldPlayer* player)
{
  // Interpose between goalie and ball
  double interpose = fabs(player->Ball()->Pos().x - player->Pos().x)*0.65;
  player->Steering()->InterposeOn(interpose);

  const Vector2D *target = new Vector2D(player->Team()->HomeGoal()->Center().x, 
                                player->Team()->HomeGoal()->Center().y);
  player->Steering()->SetTarget(*target);
}

void Prior__DefenseMeister::Execute(FieldPlayer* player)
{
  if(Vec2DDistance(player->Pos(), player->Ball()->Pos()) < 100 &&
    player->Ball()->Pos().x > 350) {
    player->GetFSM()->ChangeState(Prior__ChaseBall::Instance());
  }
  else {
    double interpose = fabs(player->Ball()->Pos().x - player->Pos().x)*0.65;
    player->Steering()->InterposeOn(interpose);

    const Vector2D *target = new Vector2D(player->Team()->HomeGoal()->Center().x, 
                                  player->Team()->HomeGoal()->Center().y);
    player->Steering()->SetTarget(*target);
  }
}

void Prior__DefenseMeister::Exit(FieldPlayer* player)
{
}


