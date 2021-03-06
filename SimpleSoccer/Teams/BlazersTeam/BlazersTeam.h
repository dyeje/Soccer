#ifndef MYBLAZERSTEAM_H
#define MYBLAZERSTEAM_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   BlazersTeam.h
//
//  Desc:   class to define a team of soccer playing agents. A SoccerTeam
//          contains several field players and one goalkeeper. A SoccerTeam
//          is implemented as a finite state machine and has states for
//          attacking, defending, and KickOff.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
// 2/3/2006 RB
// BlazersTeam
// Modified as an example of a custom soccer team.

#include <vector>

#include "Game/Region.h"
#include "../../SupportSpotCalculator.h"
#include "BlazersSupportSpotCalculator.h"
#include "FSM/StateMachine.h"
#include "../../AbstSoccerTeam.h"

                
class Goal;
class PlayerBase;
class BlazersFieldPlayer;
class BlazersGoalKeeper;
class SoccerPitch;
class BlazersSupportSpotCalculator;

class BlazersTeam : public AbstSoccerTeam
{

public:

  BlazersTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~BlazersTeam();

  std::string Name()const{return "BlazersTeam";}

	void InitStateMachine();
	void CreatePlayers();
	void InitPlayers();
	void prepareForKickoff();
	
  void UpdateTargetsOfWaitingPlayers()const;

  // Override AbstSoccerTeam::CanShoot (Improvement #1)
  virtual bool CanShoot(Vector2D  BallPos,
                       double     power, 
                       Vector2D&  ShotTarget = Vector2D())const;

  // Override AbstSoccerTeam supproting spot code to use custom
  virtual Vector2D GetSupportSpot()const;
  virtual void DetermineBestSupportingPosition()const;
  virtual PlayerBase* DetermineBestSupportingAttacker();
  BlazersSupportSpotCalculator* m_pBlazersSupportSpotCalc;

  //// Improvement #2 (CPU cost outweighted benefit)
  //bool FindPassOffBoards(const PlayerBase*const passer,
  //                      PlayerBase*&            receiver,
  //                      Vector2D&               PassTarget,
  //                      double                  power,
  //                      double                  MinPassingDistance)const;
  //bool GetBestPassToReceiverOffBoards(const PlayerBase* const passer,
  //                                    const PlayerBase* const receiver,
  //                                    Vector2D&         PassTarget,
  //                                    double            power)const;

  // Cached items (Effiiency gains)
  void SetBlazersGoalie(PlayerBase* player);
  PlayerBase* GetBlazersGoalie();
	double goalYMin;
	double goalYMax;
	double ballRadius;
  int pitchMinY;
  int pitchMaxY;
	PlayerBase* opponentGoalie;
	PlayerBase* blazersGoalie;
};

#endif
