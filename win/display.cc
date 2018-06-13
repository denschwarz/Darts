#include "display.h"
#include "StructPlayer.h"

/*
███████ ██   ██  ██████  ██     ██     ██ ███    ██ ███████  ██████
██      ██   ██ ██    ██ ██     ██     ██ ████   ██ ██      ██    ██
███████ ███████ ██    ██ ██  █  ██     ██ ██ ██  ██ █████   ██    ██
     ██ ██   ██ ██    ██ ██ ███ ██     ██ ██  ██ ██ ██      ██    ██
███████ ██   ██  ██████   ███ ███      ██ ██   ████ ██       ██████
*/

void Display::ShowInfo(int N, int start){
  if(N == 1)
    std::cout<<" [setup]  you are playing a game with "
	<< N << " player from "
	<< start << std::endl << std::endl;
  else
    std::cout<<" [setup]  you are playing a game with "
	<< N << " players from "
	<< start << std::endl << std::endl;
  return;
}

/*
██████   ██████  ██    ██ ███    ██ ██████
██   ██ ██    ██ ██    ██ ████   ██ ██   ██
██████  ██    ██ ██    ██ ██ ██  ██ ██   ██
██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██
██   ██  ██████   ██████  ██   ████ ██████
*/

void Display::RoundStart(int N){
  std::cout << ""
	    << "                     ####################" << std::endl
	    << "                     ###   Round ";
  if(N < 10) std::cout << " ";
  std::cout << N << "   ###"<<std::endl
	    << "                     ####################" << std::endl
	    << "\n" << std::endl;
  return;
}

/*
███████ ████████  █████  ███    ██ ██████  ██ ███    ██  ██████
██         ██    ██   ██ ████   ██ ██   ██ ██ ████   ██ ██
███████    ██    ███████ ██ ██  ██ ██   ██ ██ ██ ██  ██ ██   ███
     ██    ██    ██   ██ ██  ██ ██ ██   ██ ██ ██  ██ ██ ██    ██
███████    ██    ██   ██ ██   ████ ██████  ██ ██   ████  ██████
*/

void Display::Standing(std::vector<player> players, int turn){
  //TITLE
  std::cout << "                "
	    << "|  legs  |  score  | average total | average leg |" << std::endl;
  //PLAYERS
  for(unsigned int i=0; i < players.size(); ++i){
    std::cout << "      ------------------------------------------------------------" << std::endl;
    if(i==turn) std::cout  << "  X " << std::setw(10) << players[i].Name << "  |";
    else std::cout  << "    "<< std::setw(10) << players[i].Name << "  |";

    std::cout << "  " << std::setw(4) << players[i].LegsWon << "  |";
    std::cout << "  " << std::setw(5) << players[i].ScoreLeft << "  |";
    std::cout << "  " << std::setw(11) << players[i].AverageTotal << "  |";
    std::cout << "  " << std::setw(9) << players[i].AverageLeg.back() << "  |";
    std::cout << std::endl;
  }

  return;
}

/*
██      ███████  ██████      ██     ██  ██████  ███    ██
██      ██      ██           ██     ██ ██    ██ ████   ██
██      █████   ██   ███     ██  █  ██ ██    ██ ██ ██  ██
██      ██      ██    ██     ██ ███ ██ ██    ██ ██  ██ ██
███████ ███████  ██████       ███ ███   ██████  ██   ████
*/

void Display::LegWon(std::string name){
  std::cout << std::endl << std::endl << std::endl;
  std::cout << "      *************************************************" << std::endl;
  std::cout << "      *************************************************" << std::endl;
  std::cout << "      *************"<< std::setw(10) << name << " won the leg *************" << std::endl;
  std::cout << "      *************************************************" << std::endl;
  std::cout << "      *************************************************" << std::endl;
  std::cout << std::endl;
}

/*
███████ ████████  █████  ████████ ███████
██         ██    ██   ██    ██    ██
███████    ██    ███████    ██    ███████
     ██    ██    ██   ██    ██         ██
███████    ██    ██   ██    ██    ███████
*/

void Display::ShowStats(std::vector<player> players){
  std::cout << std::endl << std::endl << std::endl;
  std::cout << "               | ";
  for(int i=0; i < players.size(); ++i) std::cout << std::setw(10) << players[i].Name << " |";
  std::cout << std::endl;
  std::cout << "  ---------------";
  for(int i=0; i < players.size(); ++i) std::cout  << "------------";
  std::cout << std::endl;
  std::cout << "      legs won | " ;
  for(int i=0; i< players.size(); ++i) std::cout << std::setw(10) << players[i].LegsWon << " |";
  std::cout << std::endl;
  std::cout << "   tot average | ";
  for(int i=0; i< players.size(); ++i) std::cout  << std::setw(10) << players[i].AverageTotal << " |";
  std::cout << std::endl;
  for(int j=0; j<players[0].AverageLeg.size(); ++j){
    std::cout <<  " leg " << j+1 <<" average | ";
    for(int i=0; i< players.size(); ++i){
      if(players[i].LegWinner[j]) std::cout << "X  "<< std::setw(7) << players[i].AverageLeg[j] << " |";
      else std::cout << "   " << std::setw(7) << players[i].AverageLeg[j] << " |";
    }
    std::cout << std::endl;

  }


}
