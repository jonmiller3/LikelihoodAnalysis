#include "lmu.h"
#include "pseudo_experiment.h"
#include "Helper.cpp"

#include <list>

#include <TFile.h>

#include <TH1.h>

#include <TH1D.h>
#include <string>
#include <fstream>

#include <iostream>

#include <TH3.h>

#include <TH3D.h>
#include <TH3F.h>

using namespace std;

double calculatercrit(int nobs, model mu_true, lmu* l, int nexp, string output){

  double out=0;
  list<double> rlist;
  ofstream ofile;
  ofile.open(output.c_str(),ios::app);
  pseudo_experiment* pseudo=0;
  for (int i=0; i<nexp; i++){
    pseudo=0;
    pseudo = new pseudo_experiment(nobs,mu_true,l);
    pseudo->run();
    double r = pseudo->getlogr();
    ofile<<mu_true.Ucore<<" "<<mu_true.Umantle<<" "<<mu_true.Ucrust<<" "<<mu_true.Uocean<<" "<<mu_true.Thcore<<" "<<mu_true.Thmantle<<" "<<mu_true.Thcrust<<" "<<mu_true.Thocean<<" "<<mu_true.Kcore<<" "<<mu_true.Kmantle<<" "<<mu_true.Kcrust<<" "<<mu_true.Kocean<<" "<<r<<" \n";
    rlist.push_back(r);
    //delete pseudo;

  }
  ofile.close();
  rlist.sort();
  list<double>::iterator r_it; // = rlist.begin(); 
  int counter=0;
  for (r_it=rlist.begin(); r_it!=rlist.end(); ++r_it,++counter){
    if (counter>rlist.size()*.1){
      out=*r_it;
      return out;
    }

  }
  return out;
}

int main(int argc, char **argv){


  char* inputsignal = argv[1];
  char* inputtest = argv[5];

  string outputrfile = argv[2];
  string outputcritfile = argv[3];
  
  double testback = atof(argv[4]);

    double testUcrust = atof(argv[6]);
    double testUocean = atof(argv[7]);
    double testUmantle = atof(argv[8]);
    double testUcore = atof(argv[9]);
    
    double testThcrust = atof(argv[10]);
    double testThocean = atof(argv[11]);
    double testThmantle = atof(argv[12]);
    double testThcore = atof(argv[13]);

    double testKcrust = atof(argv[14]);
    double testKocean = atof(argv[15]);
    double testKmantle = atof(argv[16]);
    double testKcore = atof(argv[17]);
    
  // this will change in the 'near' future
  TFile* signalfile = TFile::Open(inputsignal);
  //TFile* testfile = TFile::Open(inputtest);

    modelpdf spdf;
    
    spdf.Ucore = (TH3FSpec*)signalfile->Get("finalpdf_ucore");
    spdf.Umantle = (TH3FSpec*)signalfile->Get("finalpdf_umantle");
    spdf.Ucrust = (TH3FSpec*)signalfile->Get("finalpdf_ucrust");
    spdf.Uocean = (TH3FSpec*)signalfile->Get("finalpdf_uocean");
    
    spdf.Thcore = (TH3FSpec*)signalfile->Get("finalpdf_thcore");
    spdf.Thmantle = (TH3FSpec*)signalfile->Get("finalpdf_thmantle");
    spdf.Thcrust = (TH3FSpec*)signalfile->Get("finalpdf_thcrust");
    spdf.Thocean = (TH3FSpec*)signalfile->Get("finalpdf_thocean");

    spdf.Kcore = (TH3FSpec*)signalfile->Get("finalpdf_kcore");
    spdf.Kmantle = (TH3FSpec*)signalfile->Get("finalpdf_kmantle");
    spdf.Kcrust = (TH3FSpec*)signalfile->Get("finalpdf_kcrust");
    spdf.Kocean = (TH3FSpec*)signalfile->Get("finalpdf_kocean");
    
  TH3FSpec* bpdf = (TH3FSpec*)signalfile->Get("finalpdf_sun");
  
  lmu* ltotal = new lmu(spdf,bpdf);
  int numexp = 1;

  ofstream myfile;
  myfile.open(outputcritfile.c_str(),ios::app);

  double nevents = bpdf->Integral();
    
    // nev should be the null number of events
  int nev = (int)nevents+1;


    
  //bpdf->Scale(1./nevents);

    // make things easier
    model testmu;
    testmu.Ucore=testUcore;
    testmu.Umantle=testUmantle;
    testmu.Ucrust=testUcrust;
    testmu.Uocean=testUocean;

    testmu.Thcore=testThcore;
    testmu.Thmantle=testThmantle;
    testmu.Thcrust=testThcrust;
    testmu.Thocean=testThocean;
    
    testmu.Kcore=testKcore;
    testmu.Kmantle=testKmantle;
    testmu.Kcrust=testKcrust;
    testmu.Kocean=testKocean;
    
  // was 100
    double rcrit = calculatercrit(nev,testmu,ltotal,numexp,outputrfile);

  myfile<<testmu.Ucore<<" "<<testmu.Umantle<<" "<<testmu.Ucrust<<" "<<testmu.Uocean<<" "<<testmu.Thcore<<" "<<testmu.Thmantle<<" "<<testmu.Thcrust<<" "<<testmu.Thocean<<" "<<testmu.Kcore<<" "<<testmu.Kmantle<<" "<<testmu.Kcrust<<" "<<testmu.Kocean<<" "<<rcrit<<" \n";

  myfile.close();

}
