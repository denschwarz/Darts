#include "../include/analysis.h"

using namespace std;

void doAnalysis(string name);
vector<double> GetAverage(vector<vector<int>> score);
vector<double> GetAverageAdjusted(vector<vector<int>> score, vector<int> startscore, int scoremax);
vector<double> GetHitPercent(vector<vector<int>> score, vector<int> startscore);
vector<double> GetTriplePercent(vector<vector<int>> score, vector<int> startscore);



int main(){

  string name;
  while(true){
    cout << "Do analysis for which player? ['q' for quit] ";
    cin >> name;
    if(name == "q") return 0;
    else doAnalysis(name);
  }
  return 0;

}

void doAnalysis(string name){
  // first open file and store data as vector<vector<string>> (one vector for every game)
  string line;
  ifstream infile;
  infile.open ("data/"+name+".txt");
  vector<vector<string>> segments;
  while(getline(infile,line,';')){
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
  vector<int> startscore;
  vector<TString> wonlost;
  vector<vector<int>> score;
  for(unsigned int j=0; j<segments.size(); j++){
    date.push_back(segments[j][0]);
    legnr.push_back(segments[j][2]);
    startscore.push_back(stoi(segments[j][3]));
    wonlost.push_back(segments[j][4]);
    vector<int> score_;
    for(unsigned int i=5; i<segments[j].size(); i++){
      score_.push_back(stoi(segments[j][i]));
    }
    score.push_back(score_);
  }

  // calculate average
  // also calculate adjusted average where throws inside the last 100 points do not count
  vector<double> average3dart = GetAverage(score);
  vector<double> average3dart_adjusted = GetAverageAdjusted(score, startscore, 100);

  // calculate percentage of 17/18/19/20 (count all and only count triples)
  vector<double> hitpercent = GetHitPercent(score, startscore);
  vector<double> triplepercent = GetTriplePercent(score, startscore);

  // Setup Hists
  TH1D * average3 = new TH1D("","",date.size(),0,date.size());
  TH1D * average3_adj = new TH1D("","",date.size(),0,date.size());
  TH1D * triples = new TH1D("","",date.size(),0,date.size());
  TH1D * hits = new TH1D("","",date.size(),0,date.size());

  // Fill Hists
  for(unsigned int i=0; i<date.size(); i++){
    // set y-value
    average3->Fill(i,average3dart[i]);
    average3_adj->Fill(i,average3dart_adjusted[i]);
    triples->Fill(i,triplepercent[i]);
    hits->Fill(i,hitpercent[i]);

    // label bins
    TString label = "#splitline{" + date[i] + "}{" + legnr[i] + " (" + wonlost[i] + ")}";
    average3->GetXaxis()->SetBinLabel(i+1, label);
    average3_adj->GetXaxis()->SetBinLabel(i+1, label);
    triples->GetXaxis()->SetBinLabel(i+1, label);
    hits->GetXaxis()->SetBinLabel(i+1, label);
  }


  // Draw Hists
  gStyle->SetOptStat(kFALSE);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);

  TString NAME = name;

  // Get Number of data points to set width of Canvas
  // (so that bin labels are being displayed correctly)
  int Ndata = date.size();
  int width = 200 + 80 * Ndata;
  int Nplots = 2;
  int height = 600 * Nplots;

  TCanvas *A = new TCanvas(NAME,NAME,width,height);
  A->SetTitle(NAME);
  A->Divide(1,2);
  A->cd(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.25);
  //average3_adj->SetTitle(NAME);
  double xmin = 0;
  double xmax = (double) date.size();
  vector<TLine*> lines;
  lines.push_back(new TLine(xmin, 20, xmax, 20));
  lines.push_back(new TLine(xmin, 40, xmax, 40));
  lines.push_back(new TLine(xmin, 60, xmax, 60));
  lines.push_back(new TLine(xmin, 80, xmax, 80));
  lines.push_back(new TLine(xmin, 100, xmax, 100));

  average3_adj->GetYaxis()->SetTitle("3-dart average");
  average3_adj->GetYaxis()->SetRangeUser(0, 120);
  average3_adj->SetLineColor(kRed);
  average3_adj->SetLineWidth(2);
  average3_adj->Draw("HIST");
  average3->SetLineColor(kAzure+7);
  average3->SetLineWidth(2);
  average3->Draw("HIST SAME");
  for(auto l: lines){
    l->SetLineStyle(2);
    l->SetLineColor(13);
    l->Draw("SAME");
  }
  average3_adj->Draw("HIST SAME");
  average3->Draw("HIST SAME");


  TLegend *leg = new TLegend(0.18, 0.68, 0.35, 0.85);
  leg->AddEntry(average3, "all darts","l");
  leg->AddEntry(average3_adj, "excluding last 100 points","l");
  leg->Draw();
  gPad->RedrawAxis();

  A->cd(2);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.25);
  //hits->SetTitle(NAME);
  vector<TLine*> lines2;
  lines2.push_back(new TLine(xmin, 20, xmax, 20));
  lines2.push_back(new TLine(xmin, 40, xmax, 40));
  lines2.push_back(new TLine(xmin, 60, xmax, 60));
  lines2.push_back(new TLine(xmin, 80, xmax, 80));

  hits->GetYaxis()->SetTitle("17/18/19/20 hit percentage [%]");
  hits->GetYaxis()->SetRangeUser(0, 120);
  hits->SetLineColor(kRed);
  hits->SetLineWidth(2);
  hits->Draw("HIST");
  triples->SetLineColor(kAzure+7);
  triples->SetLineWidth(2);
  triples->Draw("HIST SAME");
  for(auto l: lines2){
    l->SetLineStyle(2);
    l->SetLineColor(13);
    l->Draw("SAME");
  }
  hits->Draw("HIST SAME");
  triples->Draw("HIST SAME");
  TLegend *legC = new TLegend(0.18, 0.68, 0.35, 0.85);
  legC->AddEntry(hits, "singles/doubles/triples (excl. last 16 points)","l");
  legC->AddEntry(triples, "triples only (excl. last 50 points)","l");
  legC->Draw();
  gPad->RedrawAxis();
  A->SaveAs(NAME+"_Analysis.pdf");


  return;
}



vector<double> GetAverage(vector<vector<int>> score){
  vector<double> average3dart;
  for(unsigned int j=0; j<score.size(); j++){
    int sum = 0;
    for(unsigned int i=0; i<score[j].size(); i++){
      sum += score[j][i];
    }
    double ave;
    if(score[j].size()) ave = (double) sum / score[j].size();
    else                ave = 0;
    average3dart.push_back(3*ave);
  }
  return average3dart;
}

vector<double> GetAverageAdjusted(vector<vector<int>> score, vector<int> startscore, int scoremax){
  vector<double> average3dart_adjusted;
  for(unsigned int j=0; j<score.size(); j++){
    int sum_adj = 0;
    int size_adj = 0;
    for(unsigned int i=0; i<score[j].size(); i++){
      if(sum_adj < startscore[j]-scoremax){
        sum_adj += score[j][i];
        size_adj = i+1;
      }
    }
    double ave_adj;
    if(size_adj) ave_adj = (double) sum_adj / size_adj;
    else         ave_adj = 0;
    average3dart_adjusted.push_back(3*ave_adj);
  }
  return average3dart_adjusted;
}

vector<double> GetHitPercent(vector<vector<int>> score, vector<int> startscore){
  vector<double> hitpercent;
  for(unsigned int j=0; j<score.size(); j++){
    int Nhits = 0;
    int sum = 0;
    int size_hits = 0;
    for(unsigned int i=0; i<score[j].size(); i++){
      if(sum < startscore[j]-17){
        if(score[j][i] % 20 == 0 || score[j][i] % 19 == 0 || score[j][i] % 18 == 0 || score[j][i] % 17 == 0){
          if(score[j][i] != 0) Nhits++;
        }
        size_hits = i+1;
      }
      sum += score[j][i]; // this has to be at the end of the loop
    }
    double percent_all;
    if(size_hits) percent_all = (double) Nhits / size_hits * 100;
    else          percent_all = 0;
    hitpercent.push_back(percent_all);
  }
  return hitpercent;
}

vector<double> GetTriplePercent(vector<vector<int>> score, vector<int> startscore){
  vector<double> triplepercent;
  for(unsigned int j=0; j<score.size(); j++){
    int Ntriples = 0;
    int sum = 0;
    int size_trip = 0;
    for(unsigned int i=0; i<score[j].size(); i++){
      if(sum < startscore[j]-51){
        if(score[j][i] >= 51){
          Ntriples++;
        }
        size_trip = i+1;
      }
      sum += score[j][i]; // this has to be at the end of the loop
    }
    double percent_trip;
    if(size_trip) percent_trip = (double) Ntriples / size_trip * 100;
    else          percent_trip = 0;
    triplepercent.push_back(percent_trip);
  }
  return triplepercent;
}
