#include "analysis.h"

using namespace std;

int main(){

  // first open file and store data as vector<vector<string>> (one vector for every game)
  string line;
  ifstream infile;
  infile.open ("Dennis.txt");
  vector<vector<string>> segments;
  while(getline(infile,line,';')){
    cout << line ;
    std::replace(line.begin(), line.end(), ',', ' '); // remove comma from string
    stringstream ss(line);
    string temp;
    vector<string> line_segments;
    while (ss >> temp){
      line_segments.push_back(temp);
    }
    segments.push_back(line_segments);
  }
  infile.close();

  segments.pop_back(); // delete last element (this is empty due to txt format)

  // now convert to correct format
  vector<TString> date;
  vector<TString> legnr;
  vector<TString> wonlost;
  vector<vector<int>> score;
  for(unsigned int j=0; j<segments.size(); j++){
    date.push_back(segments[j][0]);
    legnr.push_back(segments[j][2]);
    wonlost.push_back(segments[j][3]);
    vector<int> score_;
    for(unsigned int i=4; i<segments[j].size(); i++){
      score_.push_back(stoi(segments[j][i]));
    }
    score.push_back(score_);
  }

  // calculate average
  vector<double> average1dart;
  vector<double> average3dart;
  for(unsigned int j=0; j<score.size(); j++){
    int sum = 0;
    for(unsigned int i=0; i<score[j].size(); i++){
      sum += score[j][i];
    }
    double ave = (double) sum / score[j].size();
    average1dart.push_back(ave);
    average3dart.push_back(3*ave);
  }

  TH1D * average3 = new TH1D("","",date.size(),0,date.size());
  for(unsigned int i=0; i<average3dart.size(); i++){
    average3->Fill(i,average3dart[i]);
    TString label = date[i] + " " + legnr[i];
    average3->GetXaxis()->SetBinLabel(i+1, label);
  }

  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  TCanvas *C = new TCanvas("","",600,600);
  average3->GetYaxis()->SetTitle("3-dart average");
  average3->SetFillColor(kRed);
  average3->Draw("HIST");
  C->SaveAs("Dennis_3erAverage.pdf");







  return 0;
}
