#ifndef Prior__SUPPORTSPOTCALCULATOR
#define Prior__SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)

#include <vector>

#include "Game/Region.h"
#include "2D/Vector2D.h"
#include "misc/Cgdi.h"

class PlayerBase;
class Goal;
class SoccerBall;
class AbstSoccerTeam;
class Regulator;

//------------------------------------------------------------------------

class Prior__SupportSpotCalculator 
{
private:
  
  //a data structure to hold the values and positions of each spot
  struct SupportSpot
  {
    
    Vector2D  m_vPos;

    double    m_dScore;

    SupportSpot(Vector2D pos, double value):m_vPos(pos),
                                            m_dScore(value)
    {}
  };

private:


  AbstSoccerTeam*               m_pTeam;

  std::vector<SupportSpot>  m_Spots;

  //a pointer to the highest valued spot from the last update
  SupportSpot*              m_pBestSupportingSpot;

  //this will regulate how often the spots are calculated (default is
  //one update per second)
  Regulator*                m_pRegulator;

public:
  
  Prior__SupportSpotCalculator(int numX,
                        int numY,
                        AbstSoccerTeam* team);

  ~Prior__SupportSpotCalculator();

  //draws the spots to the screen as a hollow circles. The higher the 
  //score, the bigger the circle. The best supporting spot is drawn in
  //bright green.
  void       Render()const;

  //this method iterates through each possible spot and calculates its
  //score.
  Vector2D  DetermineBestSupportingPosition();

  //returns the best supporting spot if there is one. If one hasn't been
  //calculated yet, this method calls DetermineBestSupportingPosition and
  //returns the result.
  Vector2D  GetBestSupportingSpot();
};


#endif
