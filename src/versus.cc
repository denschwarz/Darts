#include "../include/versus.h"

/*
███    ███  █████  ██ ███    ██
████  ████ ██   ██ ██ ████   ██
██ ████ ██ ███████ ██ ██ ██  ██
██  ██  ██ ██   ██ ██ ██  ██ ██
██      ██ ██   ██ ██ ██   ████
*/



int main(){
  Setup();
  Display D;
  D.ShowInfo(players.size(), start);

  int offset = 0; //used for player rotation
  while(!GameOver){
    StartLeg();
    PlayLeg(offset);
    if(!PlayAgain()){
      GameOver = true;
      system("clear");
      D.ShowStats(players);
      EndGame();
    }
    // change offset to rotate the beginning player of next leg
    offset += 1;
    if(offset == players.size()) offset = 0;
  }

  // write scores to file
  WriteToFile Writer;
  Writer.SaveData(players);

  return 0;
}

/*
███████ ███████ ████████ ██    ██ ██████
██      ██         ██    ██    ██ ██   ██
███████ █████      ██    ██    ██ ██████
     ██ ██         ██    ██    ██ ██
███████ ███████    ██     ██████  ██
*/

void Setup(){
  system("clear");

  GameOver = false;
  leg=0;

  // setup number of players
  int nr_player;
  std::cout << "\033[1;32m [setup] \033[0m number of players: ";
  std::cin >> nr_player;
  std::cout << std::endl;

  // Name every player
  std::string name_temp;
  for(unsigned int i=0; i<nr_player; ++i){
    player player_temp;
    players.push_back(player_temp);
    std::cout << "\033[1;32m [setup] \033[0m name player "<< i+1 <<": ";
    std::cin >> name_temp;
    std::cout << std::endl;
    players[i].Name = name_temp;
  }

  // setup start score
  std::cout << "\033[1;32m [setup] \033[0m Please set your start score: ";
  std::cin >> start;
  std::cout << std::endl;
  for(unsigned int i=0; i<players.size(); i++){
    players[i].LegsWon = 0;
    players[i].AverageTotal = 0;
    players[i].ScoreLeft = start;
    players[i].StartScore = start;
  }
  return;
}

/*
███████ ████████  █████  ██████  ████████     ██      ███████  ██████
██         ██    ██   ██ ██   ██    ██        ██      ██      ██
███████    ██    ███████ ██████     ██        ██      █████   ██   ███
     ██    ██    ██   ██ ██  ██     ██        ██      ██      ██
███████    ██    ██   ██ ██   ██    ██        ███████ ███████  ██████
*/



void StartLeg(){
  leg++;
  for(unsigned int i=0; i<players.size(); i++){
    std::vector<int> v;    // dummy to make Scores Vector the correct size
    players[i].Scores.push_back(v);
    players[i].AverageLeg.push_back(0);
    players[i].ScoreLeft = start;
    players[i].LegWinner.push_back(false);
  }
  return;
}

/*
████████ ██   ██ ██████   ██████  ██     ██     ██████   █████  ██████  ████████ ███████
   ██    ██   ██ ██   ██ ██    ██ ██     ██     ██   ██ ██   ██ ██   ██    ██    ██
   ██    ███████ ██████  ██    ██ ██  █  ██     ██   ██ ███████ ██████     ██    ███████
   ██    ██   ██ ██   ██ ██    ██ ██ ███ ██     ██   ██ ██   ██ ██   ██    ██         ██
   ██    ██   ██ ██   ██  ██████   ███ ███      ██████  ██   ██ ██   ██    ██    ███████
*/

void ThrowDarts(player *p){
  //show display here
  int points[] = {0,0,0};
  int oldscore = p->ScoreLeft; //store in case of overthrown
  int lastDart = 3; // this only changes if player finishes with dart 1 or 2
  for(int dartnr=1; dartnr<=3; dartnr++){
    if(dartnr==1) std::cout << "\033[1;32m [Round "<< (p->Scores[leg-1].size())/3+1 << "] \033[0m " << p->Name << " (DART " << dartnr << "): ";
    else          std::cout << "            " << p->Name << " (DART " << dartnr << "): ";
    std::cin >> points[dartnr-1];
    while(!std::cin.good()){
      std::cin.clear();
      std::cin.ignore(1000000, '\n');
      std::cout << "\033[1;31m [error] \033[0m Wrong input format! Try again." << std::endl;
      std::cout << "            " << p->Name << " (DART " << dartnr << "): ";
      std::cin >> points[dartnr-1];
    }

    
    // player cannot score more than 180
    while(points[dartnr-1] > 60){
      std::cout << "\033[1;31m [error] \033[0m You can not score more than 60 points with one throw. Try again: " << std::endl;
      std::cout << "            " << p->Name << " (DART " << dartnr << "): ";
      std::cin >> points[dartnr-1];
      while(!std::cin.good()){
	std::cin.clear();
	std::cin.ignore(1000000, '\n');
	std::cout << "\033[1;31m [error] \033[0m Wrong input format! Try again." << std::endl;
	std::cout << "            " << p->Name << " (DART " << dartnr << "): ";
	std::cin >> points[dartnr-1];
      }
    }

    // player has to hit exactly 0
    bool overthrown = false;
    if(points[dartnr-1] > p->ScoreLeft){
      std::cout << "\033[1;32m [info] \033[0m you can not end below 0, score is reset to: " << oldscore << std::endl;
      p->ScoreLeft = oldscore;
      points[0] = 0;
      points[1] = 0;
      points[2] = 0;
      overthrown = true;
    }

    // update score left
    p->ScoreLeft -= points[dartnr-1];

    // is score = 0?
    bool finished = false;
    if(p->ScoreLeft == 0){
      finished = true;
      lastDart = dartnr;
    }

    // if score is 0 or overthrown do not throw more darts
    if(finished || overthrown) break;
  }

  // update statistics
  for(int dartnr=1; dartnr<=lastDart; dartnr++){
    p->Scores[leg-1].push_back(points[dartnr-1]);
  }
  p->AverageLeg[leg-1] = AverageLeg(p->Scores, leg);
  p->AverageTotal = AverageTotal(p->Scores);

  return;
}

/*
██   ██ ███████ ██      ██████  ███████ ██████  ███████
██   ██ ██      ██      ██   ██ ██      ██   ██ ██
███████ █████   ██      ██████  █████   ██████  ███████
██   ██ ██      ██      ██      ██      ██   ██      ██
██   ██ ███████ ███████ ██      ███████ ██   ██ ███████
*/


double AverageLeg(std::vector< std::vector<int> > vec, int leg){
  double sum = 0;
  double entries = 0;
  for(unsigned int j=0; j<vec[leg-1].size(); j++){
    sum += vec[leg-1][j];
    entries++;
  }
  double average = 3*sum/entries;
  return average;
}

double AverageTotal(std::vector< std::vector<int> > vec){
  double sum = 0;
  double entries = 0;
  for(unsigned int i=0; i<vec.size(); i++){
    for(unsigned int j=0; j<vec[i].size(); j++){
      sum += vec[i][j];
      entries++;
    }
  }
  double average = 3*sum/entries;
  return average;
}


bool PlayAgain(){
  std::string rematch;
  std::cout << "\033[1;32m [info] \033[0m do you want to play another leg? (y/n): ";
  std::cin >> rematch;
  if(rematch == "y") return true;
  else return false;
}

void PlayLeg(int offset){
  bool play = true;
  bool first = true; //for player rotation
  Display D;
  while(play){
    for(unsigned int i=0; i<players.size(); i++){
      if(first){
        i = offset;
        first = false;
      }
      system("clear");
      D.RoundStart((players[i].Scores[leg-1].size())/3 + 1);
      D.Standing(players,i);
      std::cout << std::endl << std::endl << std::endl;
      ThrowDarts(&players[i]);
      if(players[i].ScoreLeft == 0){
        players[i].LegsWon++;
        players[i].LegWinner[leg-1] = true;
        system("clear");
        D.RoundStart((players[i].Scores[leg-1].size())/3);
        D.Standing(players,i);
        D.LegWon(players[i].Name);
        return;
      }
    }
  }
}

void EndGame(){
  std::string quit;
  while(true){
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "\033[1;32m [info] \033[0m Type 'q' to quit (and save data): ";
    std::cin >> quit;
    if(quit == "q") return;
  }
}
