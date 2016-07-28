#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

using namespace std;

vector<string> split(const string &s, char delim);
vector<string> &split(const string &s, char delim, vector<string> &elems);


vector<string> &split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}


vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, elems);
  return elems;
}


void WrapUpT5qqqqVVSignalContamination(string type="T1bbbb",string outStr="Jul"){

  char tempname[200];
  char filenames[500];

  // open a file to write the output 
  sprintf(tempname,"TauHad2/%s_SignalFiles.root",type.c_str());
  std::cout<<" Output file name "<< tempname <<endl;
  TFile * outfile = new TFile(tempname,"RECREATE");
  //  TDirectory * tdirSearch = outfile->mkdir("SearchH_b");



  // *AR -Jul22,2016-With Rishi's request to be consistent with Simon's naming
  double ScaleLumiTo     = 1.0; //(pb-1)
  double ScaleLumiFrom = 3000.0; // normaliza to 3 (fb-1)
 
  //*AR- Jul22,2016- Here name of SearchH_b directory is changed to SignalContamination on Rishi's request to be consistent with Simon's naming in contamination files
  TDirectory * tdirSearch = outfile->mkdir("SignalContamination");
  TDirectory * tdirQCD_Up = outfile->mkdir("QCD_Up");
  TDirectory * tdirQCD_Low = outfile->mkdir("QCD_Low");

  // a temporary histogram
  TH1D * temphist;

  vector<string> filesVec;
  sprintf(tempname,"%s_InputSignal.txt",type.c_str());
  ifstream fin(tempname);
  // check the timing of the file. If outdated alert. 
  struct stat t_stat; 
  stat(tempname, &t_stat);
  tm  timeinfo = *localtime(&t_stat.st_ctime); // or gmtime() depending on what you want
  printf(" File's date: year: %g month: %g day: %g \n",1900+timeinfo.tm_year,1+timeinfo.tm_mon,timeinfo.tm_mday );
  // Today's date
  time_t now = time(0);
  tm ltm = *localtime(&now);
  printf(" Today: year: %g month: %g day: %g \n",1900+ltm.tm_year,1+ltm.tm_mon,ltm.tm_mday );
  // the real check
  if( (timeinfo.tm_year != ltm.tm_year) || (timeinfo.tm_mon != ltm.tm_mon) || (timeinfo.tm_mday != ltm.tm_mday) ){
    cout << " Warning: file is not updated today. Make sure you are using the correct file.\n";
    //return ; 
  }


  ///read the file names from the .txt files and load them to a vector.
  while(fin.getline(filenames, 500) ){filesVec.push_back(filenames);}
  std::cout<<" filesVec_size "<<filesVec.size()<<endl;
  //HadTauEstimation_RA2bin_T1tttt_1950_600_fast-_00.root
  for(unsigned int in=0; in<filesVec.size(); in++){

// *AR -Jul22,2016-With Rishi's request to be consistent with Simon's naming

      vector<string> elems =  split(filesVec.at(in), '_');
    
      sprintf(tempname,"mGluino_%s_mLSP_%s",elems.at(0).c_str(),elems.at(1).c_str());
      std::cout<<" histname "<< tempname <<endl;
    


    //    sprintf(tempname,"RA2bin_%s",filesVec.at(in).c_str());
    //std::cout<<" filesVec_i "<< tempname <<endl;
    //    sprintf(filenames,"TauHad2Multiple/HadTauEstimation_%s-Apr22_00.root",filesVec.at(in).c_str());
    sprintf(filenames,"TauHad2Multiple/HadTauEstimation_RA2bin_%s_%s_fast-%s_00.root",type.c_str(),filesVec.at(in).c_str(),outStr.c_str());
    //    sprintf(filenames,"TauHad2Multiple/HadTauEstimation_%s-Apr22_00.root",filesVec.at(in).c_str());  
    TFile * infile = new TFile(filenames,"READ");
    if(!infile->IsOpen()){
      printf(" file: %s is not open \n",filesVec.at(in).c_str());
      continue;
    }

    //    cout << " name: " <<  filesVec.at(in) << endl;

    temphist = (TH1D*) infile->Get("searchH_b")->Clone();
    temphist->Scale(ScaleLumiTo/ScaleLumiFrom);
    temphist->SetName(tempname);
    temphist->SetTitle(tempname);
    tdirSearch->cd();
    temphist->Write();
    temphist = (TH1D*) infile->Get("QCD_Up")->Clone();
    temphist->Scale(ScaleLumiTo/ScaleLumiFrom);
    temphist->SetName(tempname);
    temphist->SetTitle(tempname);
    tdirQCD_Up->cd();
    temphist->Write();
    temphist = (TH1D*) infile->Get("QCD_Low")->Clone();
    temphist->Scale(ScaleLumiTo/ScaleLumiFrom);
    temphist->SetName(tempname);
    temphist->SetTitle(tempname);
    tdirQCD_Low->cd();
    temphist->Write();    
    
  }

































}
