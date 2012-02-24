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

  string player;
  string x;
  string y;
  map<int, pair<double, double> > players;

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
          players[atoi(player.c_str())] = make_pair<double, double> (atof(x.c_str()), atof(y.c_str()));
        }
      }
      gameStateFile.close();
    }

  }

public:

  static GameStateLoader* Instance();
  pair<double, double> PlayerCoord(int player) { 
    return players[player];
  }

};

#endif 
