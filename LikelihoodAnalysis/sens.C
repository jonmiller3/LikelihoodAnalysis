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
    
    TH3D* bpdf = (TH3D*)signalfile->Get("finalpdf_sun");

    
    spdf.Ucore = (TH3D*)signalfile->Get("finalpdf_ucore");
    spdf.Umantle = (TH3D*)signalfile->Get("finalpdf_umantle");
    spdf.Ucrust = (TH3D*)signalfile->Get("finalpdf_ucrust");
    spdf.Uocean = (TH3D*)signalfile->Get("finalpdf_uocean");
    
    spdf.Thcore = (TH3D*)signalfile->Get("finalpdf_thcore");
    spdf.Thmantle = (TH3D*)signalfile->Get("finalpdf_thmantle");
    spdf.Thcrust = (TH3D*)signalfile->Get("finalpdf_thcrust");
    spdf.Thocean = (TH3D*)signalfile->Get("finalpdf_thocean");
    
    spdf.Kcore = (TH3D*)signalfile->Get("finalpdf_kcore");
    spdf.Kmantle = (TH3D*)signalfile->Get("finalpdf_kmantle");
    spdf.Kcrust = (TH3D*)signalfile->Get("finalpdf_kcrust");
    spdf.Kocean = (TH3D*)signalfile->Get("finalpdf_kocean");
    
  lmu* ltotal = new lmu(spdf,bpdf);


  map<model,double> rcrit;

  double nevents = bpdf->Integral();

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
    ss>>mu.Ucore;
      ss>>mu.Umantle;
      ss>>mu.Ucrust;
      ss>>mu.Uocean;

      ss>>mu.Thcore;
      ss>>mu.Thmantle;
      ss>>mu.Thcrust;
      ss>>mu.Thocean;
      
      ss>>mu.Kcore;
      ss>>mu.Kmantle;
      ss>>mu.Kcrust;
      ss>>mu.Kocean;
      
    ss>>rc;

    rcrit[mu]=rc;

  }

  myfile.close();

  TH1F* hup = new TH1F("hup","upper bound",100,0,100);
  TH1F* hlo = new TH1F("hlo","lower bound",100,0,100);

  ofstream outfile;
  outfile.open(outputfile.c_str(),ios::app);


  model mu90;
  model lb90;
  model* bound;
  list<model> upperbound;
  list<model> lowerbound;

model maxmu;

    
    maxmu.Ucore=0;
    maxmu.Umantle=163;
    maxmu.Ucrust=194;
    maxmu.Uocean=4;
    
    maxmu.Thcore=0;
    maxmu.Thmantle=653;
    maxmu.Thcrust=759;
    maxmu.Thocean=15;
    
    maxmu.Kcore=0;
    maxmu.Kmantle=163*1e4;
    maxmu.Kcrust=194*1e4;
    maxmu.Kocean=4*1e4;

  for (int i=0; i<numexp; i++){
    // this 500 should be number of events I am testing....
    // I get this from looking at the number of entries in one of the histograms?



    pseudo_experiment* pseudo = new pseudo_experiment(nev,maxmu,ltotal);
    pseudo->run();
    bound = pseudo->getrange(rcrit);

    //cout<<" bounds "<<bound[0]<<" "<<bound[1]<<" "<<pseudo->getlogr()<<endl;

    //hup->Fill(bound[0]);
    //hlo->Fill(bound[1]);
      
      mu90=bound[0];
      lb90=bound[1];
      
      outfile<<mu90.Ucore<<" ";
      outfile<<mu90.Umantle<<" ";
      outfile<<mu90.Ucrust<<" ";
      outfile<<mu90.Uocean<<" ";
      
      outfile<<mu90.Thcore<<" ";
      outfile<<mu90.Thmantle<<" ";
      outfile<<mu90.Thcrust<<" ";
      outfile<<mu90.Thocean<<" ";
      
      outfile<<mu90.Kcore<<" ";
      outfile<<mu90.Kmantle<<" ";
      outfile<<mu90.Kcrust<<" ";
      outfile<<mu90.Kocean<<"\n";

      outfile<<lb90.Ucore<<" ";
      outfile<<lb90.Umantle<<" ";
      outfile<<lb90.Ucrust<<" ";
      outfile<<lb90.Uocean<<" ";
      
      outfile<<lb90.Thcore<<" ";
      outfile<<lb90.Thmantle<<" ";
      outfile<<lb90.Thcrust<<" ";
      outfile<<lb90.Thocean<<" ";
      
      outfile<<lb90.Kcore<<" ";
      outfile<<lb90.Kmantle<<" ";
      outfile<<lb90.Kcrust<<" ";
      outfile<<lb90.Kocean<<"\n";
      
      
    //outfile<<bound[0]<<" "<<bound[1]<<"\n";

    upperbound.push_back(bound[0]);
    lowerbound.push_back(bound[1]);
  }

    // why do I sort just one?
    upperbound.sort();
    
    // added this, I think I did just one
    // because in IceCube I want just upperbound
    lowerbound.sort();

  int counter=0;
  list<model>::iterator ub_it=upperbound.begin();
  list<model>::iterator lb_it=lowerbound.begin();
  
  while(counter<numexp){

    counter++;
    ub_it++;
    lb_it++;
    if (counter<numexp*.5+1){
      mu90 = *ub_it;
      lb90 = *lb_it;
    }

  }

    cout<<mu90.Ucore<<endl;
    cout<<mu90.Umantle<<endl;
    cout<<mu90.Ucrust<<endl;
    cout<<mu90.Uocean<<endl;
    
    cout<<mu90.Thcore<<endl;
    cout<<mu90.Thmantle<<endl;
    cout<<mu90.Thcrust<<endl;
    cout<<mu90.Thocean<<endl;
    
    cout<<mu90.Kcore<<endl;
    cout<<mu90.Kmantle<<endl;
    cout<<mu90.Kcrust<<endl;
    cout<<mu90.Kocean<<endl;
    
    cout<<" now do loer "<<endl;
    
    cout<<lb90.Ucore<<endl;
    cout<<lb90.Umantle<<endl;
    cout<<lb90.Ucrust<<endl;
    cout<<lb90.Uocean<<endl;
    
    cout<<lb90.Thcore<<endl;
    cout<<lb90.Thmantle<<endl;
    cout<<lb90.Thcrust<<endl;
    cout<<lb90.Thocean<<endl;
    
    cout<<lb90.Kcore<<endl;
    cout<<lb90.Kmantle<<endl;
    cout<<lb90.Kcrust<<endl;
    cout<<lb90.Kocean<<endl;


  /*
  TCanvas c;
  hup->Draw();
  c.Print(Form("hup%s.eps",outputfile.c_str()));
  c.Clear();
  hlo->Draw();
  c.Print(Form("hlo%s.eps",outputfile.c_str()));
           
  */
}
