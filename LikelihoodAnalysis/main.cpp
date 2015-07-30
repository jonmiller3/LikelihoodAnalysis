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
    int nexp = 1;

    double nevents = bpdf->Integral();
    int nev = (int)nevents+1;

    
    
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
