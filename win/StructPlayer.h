struct player {
  std::string Name;                       // Name of Player
  int StartScore;                         // Start Score of Legs (e.g. 301, 501, ...)
  int LegsWon;                            // Number of legs won in game
  double AverageTotal;                    // 3-darts average over all legs
  std::vector<double> AverageLeg;         // i-th entry contains average of i-th leg
  int ScoreLeft;                          // how many points are left in current leg
  std::vector< std::vector<int> > Scores; // contains the points made with the i-th dart in the j-th leg
  std::vector< bool > LegWinner;          // i-th entry is true if player won the i-th leg
} ;
