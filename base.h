
#include <fstream>
#include "realplayer.h"
#include "computerplayer.h"
#include <headerlib/RangeIterator.h>
#include <cmath>
#include "help/pointoperators.h"
#include "help/iterfns.h"
class DebugInter;
void SaveGameInfo(string FileName);
bool LoadGameInfo(string FileName);
void ResetGame();
void AttackLocation(Point Loc, Troop * AttackingTroop);
bool AttemptAttack(Point Loc, Troop * AttackingTroop);
void ChangeOverallTurn();
void DeletePlayer(int PNum);
void DeleteAllPlayers();
void InitiateGame(DebugInter * deb);
Player * GetCurrentPlayer();
void ChangeMiniTurn();
