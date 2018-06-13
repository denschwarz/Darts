#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include "StructPlayer.h"
#include "display.h"
#include "writetofile.h"


std::vector<player> players;
bool GameOver;
int start;
int leg;

void Setup();
void StartLeg();
void ThrowDarts(player *p);
void PlayLeg(int);
void EndGame();
bool PlayAgain();
double AverageLeg(std::vector< std::vector<int> >, int);
double AverageTotal(std::vector< std::vector<int> >);
