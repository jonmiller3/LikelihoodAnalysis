#include "pseudo_experiment.h"
#include "lmu.h"

#include <TH1.h>

#include <TH3.h>
#include <TCanvas.h>
#include <TFile.h>
#include <list>
#include <map>

#include <fstream>
#include <sstream>
#include <iostream>

#include <string>

using namespace std;

int main(int argc, char **argv){

  string inputcritfile = argv[2];
  char* inputsignal = argv[1];
  char* inputtest = argv[3];
  string outputfile = argv[4];

  int numexp=100;

  //TFile* inputfile = TFile::Open("fpdf.root");
  // will change
  TFile* signalfile = TFile::Open(inputsignal);
  TFile* testfile = TFile::Open(inputtest);
  // for now it pulls from test, but when I do the final it will need to pull from 'good' for number of events

    modelpdf spdf;
    
    TH1FSpec* bpdf = (TH1FSpec*)signalfile->Get("finalpdf_back");

    
    spdf.signal = (TH1FSpec*)signalfile->Get("finalpdf_signal");
    
  lmu* ltotal = new lmu(spdf,bpdf);


  map<model,double> rcrit;

    model maxmu;
    
    
    maxmu.signal=0;
  
    double nevents = ltotal->getintegral(maxmu);

    
    
  int nev = (int)(nevents+0.999999999999999);


  ifstream myfile;			    
  myfile.open(inputcritfile.c_str());

  while (!myfile.eof()){

    string line;

    stringstream ss ( stringstream::in | stringstream::out );

    getline(myfile,line);

    ss<<line;

    cout<<" OK, this is where I am "<<line<<endl;

    model mu

      double rc;
      ss>>mu.signal;
      
    ss>>rc;

    rcrit[mu]=rc;

  }

  myfile.close();

  TH1F* hup = new TH1F("hup","upper bound",100,0,100);
  TH1F* hlo = new TH1F("hlo","lower bound",100,0,100);


    TFile *f = new TFile(name.c_str(),"recreate");
    
    TTree* datatree = new TTree("T","Tree");
    
    model mu;
    
    datatree->Branch("model",&mu,"signal/D");
  vector<model> bound;



  for (int i=0; i<numexp; i++){
    // this 500 should be number of events I am testing....
    // I get this from looking at the number of entries in one of the histograms?



    pseudo_experiment* pseudo = new pseudo_experiment(nev,maxmu,ltotal);
    pseudo->run(true);
    bound = pseudo->getrange(rcrit);


      
      
      vector<model>::iterator it_mu=bound.begin();
      
      
      
      for (it_mu!=bound.end(); ++it_mu){
          
          mu=*it_mu;
          //std::cout<<" r "<<r<<" "<<*it<<" "<<maxl<<std::endl;
          datatree->Fill();
          
      }
      
      
  }
      f->Write();
      f->Close();
      
 
}
