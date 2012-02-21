#ifndef GAMESTATELOADER
#define GAMESTATELOADER
#pragma warning(disable:4800)
//------------------------------------------------------------------------
//
// Name:  StateLoader.h
//
// Desc:  Singleton class to handle the loading of an existing game state
//
//------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <map>
#include <utility>
#include "constants.h"

using namespace::std;

#define GameState (*GameStateLoader::Instance())

class GameStateLoader {

private:

  string player;
  string x;
  string y;
  map<string, pair<double, double> > players;

  GameStateLoader() {   
    ifstream gameStateFile("GameState.ini");
    if(gameStateFile.fail()) { 
    }
    else {
      while(gameStateFile >> player) {:w

        gameStateFile >> x;
        gameStateFile >> y;
        players[player] = make_pair<double, double> (atof(x.c_str()), atof(y.c_str()));
      }

    }
  }
  
public:

  static GameStateLoader* Instance();
  pair<double, double> PlayerCoord(const string player) { 
    return players[player];
  }

};

#endif 
