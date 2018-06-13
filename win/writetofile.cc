#include "writetofile.h"
#include "StructPlayer.h"

void WriteToFile::SaveData(std::vector<struct player> players){

  // first get date and time in stringstream "datum"
  std::stringstream datum;
  GetDateStream(datum);

  for(unsigned int i=0; i<players.size(); i++){
    // read old file and safe to tmp file
    std::stringstream filename;
    filename << players[i].Name << ".txt";
    std::ifstream oldfile;
    oldfile.open(filename.str().c_str());
    std::ofstream tempfile;
    tempfile.open("tempfile.txt");
    CopyFile(oldfile, tempfile);
    oldfile.close();
    tempfile.close();

    // create new file (which overwrites old file) -> copy from tempfile
    // (tempfile has to be ifstream now)
    std::ifstream tempfile2;
    tempfile2.open("tempfile.txt");
    std::ofstream newfile;
    newfile.open( filename.str().c_str() );
    CopyFile(tempfile2, newfile);
    tempfile2.close();
    for (unsigned int leg=0; leg<players[i].Scores.size(); leg++) {
      // write date, time, leg number, start score and if it was won
      newfile << datum.str() << ", " << "Leg" << leg+1 << ", ";
      newfile << players[i].StartScore << ", ";
      if(players[i].LegWinner[leg]) newfile << "won";
      else                          newfile << "lost";
      for (unsigned int k=0; k<players[i].Scores[leg].size(); k++) {
        //write scores
        newfile << ", " << players[i].Scores[leg][k];
      }
      newfile << ";\n";
    }
    newfile.close();
  }
}

void WriteToFile::GetDateStream(std::stringstream& datum){
  time_t Zeitstempel;
  tm *nun;
  Zeitstempel = time(0);
  nun = localtime(&Zeitstempel);
  if(nun->tm_mday < 10) datum << "0" << nun->tm_mday << ".";
  else datum << nun->tm_mday << ".";
  if(nun->tm_mon+1 < 10) datum << "0" << nun->tm_mon+1 << ".";
  else datum << nun->tm_mon+1 << ".";
  datum << nun->tm_year+1900 << ", ";

  if(nun->tm_hour < 10) datum << "0" << nun->tm_hour << ":";
  else datum << nun->tm_hour << ":";
  if(nun->tm_min < 10) datum << "0" << nun->tm_min;
  else datum << nun->tm_min;
  return;
}

void WriteToFile::CopyFile(std::ifstream& inData, std::ofstream& outData){
  std::string line;
  while (getline(inData, line)){
    outData << line << '\n';
  }
}
