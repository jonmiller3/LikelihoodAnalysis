//
//  main.cpp
//  LikelihoodAnalysis
//
//  Created by Jonathan Miller on 7/10/15.
//  Copyright (c) 2015 Jonathan Miller. All rights reserved.
//
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

int main(int argc, char ** argv) {

    char* inputsignal = argv[1];
    char* inputtest = argv[5];
    
    string outputfile = argv[2];
    string extrafile = argv[3];
    
    double testsignal = atof(argv[4]);
    
    // this will change in the 'near' future
    TFile* signalfile = TFile::Open(inputsignal);
    //TFile* testfile = TFile::Open(inputtest);
    
    
    TH1FSpec* bpdf = (TH1FSpec*)signalfile->Get("finalpdf_back");
    spdf.signal = (TH1FSpec*)signalfile->Get("finalpdf_signal");
    
    lmu* ltotal = new lmu(spdf,bpdf);
    int nexp = 10000;

    double nevents = bpdf->Integral();
    int nev = (int)nevents+1;

    
    
    model testmu;
    testmu.signal=testsignal;
    
    
    // the idea is that I create a root file
    // with everything in it...
    
    std::cout<<" nev "<<nev<<std::endl;
    
    pseudo_experiment* pseudo=0;
    for (int i=0; i<nexp; i++){
        pseudo=0;
        pseudo = new pseudo_experiment(nev,testmu,ltotal);
        pseudo->run(true);
        pseudo->writerootfile(outputfile);
        //delete pseudo;
    }
    
    return 0;
}
