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
#include <sstream>
#include <string>
#include <cassert>
#include <map>
#include <utility>
#include "../../constants.h"

using namespace::std;

#define GameState (*GameStateLoader::Instance())

class GameStateLoader {

private:

  string player, x, y, state;
  map<int, pair<double, double> > player_coords;
  map<int, string> player_states;

  GameStateLoader() {   

    ifstream gameStateFile("GameState.txt");
    if(gameStateFile.is_open()) {
      string line, token;
      while(gameStateFile.good()) {

        getline(gameStateFile,line);
        if(!line.empty() && line[0]!='#') {
          stringstream lineStream(line);
          lineStream >> player;
          lineStream >> x;
          lineStream >> y;
          lineStream >> state;
          player_coords[atoi(player.c_str())] = make_pair<double, double> (atof(x.c_str()), atof(y.c_str()));
          player_states[atoi(player.c_str())] = state;
        }
      }
      gameStateFile.close();
    }

  }

public:

  static GameStateLoader* Instance();
  pair<double, double> PlayerCoord(int player) { 
    return player_coords[player];
  }
  string PlayerState(int player) { 
    return player_states[player];
  }

};

#endif 
