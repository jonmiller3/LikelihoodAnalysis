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
    ofile<<mu_true.signal<<" "<<r<<" \n";
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
    
    string outputfile = argv[2];
    string extrafile = argv[3];
    
    double testsignal = atof(argv[4]);
    
    // this will change in the 'near' future
    TFile* signalfile = TFile::Open(inputsignal);
    TFile* testfile = TFile::Open(inputtest);
    
    
    
    modelpdf spdf;
 
    TH1FSpec* bpdf = (TH1FSpec*)testfile->Get("pdf40");
    spdf.signal = (TH1FSpec*)signalfile->Get("pdf40");

    lmu* ltotal = new lmu(spdf,bpdf);
    
    int nexp = 100;
    
  ofstream myfile;
  myfile.open(extrafile.c_str(),ios::app);

    // make things easier
    model testmu;
    testmu.signal=testsignal;
    
    
    double nevents = bpdf->Integral();
    double sevents = spdf.signal->Integral();
    
    nevents = ltotal->getintegral(testmu);
    int nev = (int)nevents+1;
    // nev should be the null number of events
    
    
    // I think I changed it so this is not necessary
    //spdf.signal->Scale(1./sevents);
    //bpdf->Scale(1./nevents);
    
    cout<<" number of events (combined) "<<nevents<<" signal events "<<testmu.signal<<endl;
    
    double rcrit = calculatercrit(nev,testmu,ltotal,nexp,outputfile);

  myfile<<testmu.signal<<" "<<rcrit<<" \n";

  myfile.close();

}
