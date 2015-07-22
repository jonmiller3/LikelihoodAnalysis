#ifndef LMU_H
#define LMU_H

#include <TH1D.h>
#include <TH1F.h>
#include <TH1.h>

#include <TH2D.h>
#include <TH2F.h>
#include <TH2.h>

#include <TH3D.h>
#include <TH3F.h>
#include <TH3.h>

#include <TMath.h>
#include <TRandom3.h>
#include <TCanvas.h>

#include <iostream>
//#include <thread>


#include "Helper.h"

using namespace std;

struct model {
    double Ucrust;
    double Uocean;
    double Ucore;
    double Umantle;

    double Thcrust;
    double Thocean;
    double Thcore;
    double Thmantle;
    
    double Kcrust;
    double Kocean;
    double Kcore;
    double Kmantle;
    
};


struct modelpdf {
    TH3FSpec* Ucrust;
    TH3FSpec* Uocean;
    TH3FSpec* Ucore;
    TH3FSpec* Umantle;
    
    TH3FSpec* Thcrust;
    TH3FSpec* Thocean;
    TH3FSpec* Thcore;
    TH3FSpec* Thmantle;
    
    TH3FSpec* Kcrust;
    TH3FSpec* Kocean;
    TH3FSpec* Kcore;
    TH3FSpec* Kmantle;
    
    
};

bool operator<(model a, model m){

  double total=0;

  total+=abs(m.Ucore-0);
  total+=abs(m.Umantle-163);
total+=abs(m.Ucrust-194);
total+=abs(m.Uocean-4);

 total+=abs(m.Thcore-0);
 total+=abs(m.Thmantle-653);
 total+=abs(m.Thcrust-759);
 total+=abs(m.Thocean-15);

 total+=abs(m.Kcore-0);
 total+=abs(m.Kmantle-163*1e4);
 total+=abs(m.Kcrust-194*1e4);
 total+=abs(m.Kocean-4*1e4);



  double ta=0;

  ta+=abs(a.Ucore-0);
  ta+=abs(a.Umantle-163);
ta+=abs(a.Ucrust-194);
ta+=abs(a.Uocean-4);

 ta+=abs(a.Thcore-0);
 ta+=abs(a.Thmantle-653);
 ta+=abs(a.Thcrust-759);
 ta+=abs(a.Thocean-15);

 ta+=abs(a.Kcore-0);
 ta+=abs(a.Kmantle-163*1e4);
 ta+=abs(a.Kcrust-194*1e4);
 ta+=abs(a.Kocean-4*1e4);


  return ta<total;


}




class lmu{

private:
  modelpdf fs;
  TH3FSpec* fb;
  TH3FSpec* ft;

    
    int nbinsx;
    int nbinsy;
    int nbinsz;
    double maxx;
    double maxy;
    double maxz;

public:
  lmu(modelpdf hs, TH3FSpec* hb){fs=hs;fb=hb;
    nbinsx=fb->GetXaxis()->GetNbins();
    maxx=fb->GetXaxis()->GetXmax();
    nbinsy=fb->GetYaxis()->GetNbins();
    maxy=fb->GetYaxis()->GetXmax();
      nbinsz=fb->GetZaxis()->GetNbins();
      maxz=fb->GetZaxis()->GetXmax();}

    ~lmu(){delete fb;}

    int* calc(double*, double*, double*, model, int, double&);
  int getrandom(model,int,double*, double*, double*);
  int* getrandom(model,int);
    
    int getncells(){return fb->GetNCells();}
    
    // but I think that double* is better?
    float* getmodelarray();
    
    
};

// but I think I want double*
float* lmu::getmodelarray(){
    
    int ncells = fb->GetNCells();
    
    // I think that doing this is unncessary
    const float* barray = fb->GetArray();
    const float* s1array = fs.Ucore->GetArray();
    const float* s12array = fs.Kocean->GetArray();
    
    // res should be 13*nbins
    float* res = new float[ncells*13];
    
    copy(barray, barray+ncells, res);
    copy(s1array, s1array+ ncells, res + ncells);
    copy(fs.Umantle->GetArray(), fs.Umantle->GetArray()+ ncells, res + 2*ncells);

    copy(fs.Ucrust->GetArray(), fs.Ucrust->GetArray()+ ncells, res + 3*ncells);

    copy(fs.Uocean->GetArray(), fs.Uocean->GetArray()+ ncells, res + 4*ncells);

    copy(fs.Thcore->GetArray(), fs.Thcore->GetArray()+ ncells, res + 5*ncells);
    copy(fs.Thmantle->GetArray(), fs.Thmantle->GetArray()+ ncells, res + 6*ncells);
    
    copy(fs.Thcrust->GetArray(), fs.Thcrust->GetArray()+ ncells, res + 7*ncells);
    
    copy(fs.Thocean->GetArray(), fs.Thocean->GetArray()+ ncells, res + 8*ncells);
    
    copy(fs.Kcore->GetArray(), fs.Kcore->GetArray()+ ncells, res + 9*ncells);
    copy(fs.Kmantle->GetArray(), fs.Kmantle->GetArray()+ ncells, res + 10*ncells);
    
    copy(fs.Kcrust->GetArray(), fs.Kcrust->GetArray()+ ncells, res + 11*ncells);
    
    copy(s12array, s12array+ ncells, res + 12*ncells);
    
    return res;
    
}


int* lmu::calc(double* phi, double* energy,double* theta, model mu, int nobs, double& totalcontent){
  
    int* binres = new int[nobs];
    
  ft=0;

  ft = new TH3FSpec("ft","ft",nbinsx,0,maxx,nbinsy,0,maxy,nbinsz,0,maxz);

    ft->Add(fs.Ucore,fb,mu.Ucore,1);
    ft->AddFast(fs.Umantle,ft,mu.Umantle,1);
    ft->AddFast(fs.Ucrust,ft,mu.Ucrust,1);
    ft->AddFast(fs.Uocean,ft,mu.Uocean,1);

    
    ft->AddFast(fs.Thcore,ft,mu.Thcore,1);
    ft->AddFast(fs.Thmantle,ft,mu.Thmantle,1);
    ft->AddFast(fs.Thcrust,ft,mu.Thcrust,1);
    ft->AddFast(fs.Thocean,ft,mu.Thocean,1);
    
    ft->AddFast(fs.Kcore,ft,mu.Kcore,1);
    ft->AddFast(fs.Kmantle,ft,mu.Kmantle,1);
    ft->AddFast(fs.Kcrust,ft,mu.Kcrust,1);
    ft->AddFast(fs.Kocean,ft,mu.Kocean,1);

  totalcontent=0;
  for (int i=0; i<nobs; i++){

      // I need to check order on this
    int binx=1+(int)(phi[i]*nbinsx/maxx);
      int biny=1+(int)((energy[i])*nbinsy/(maxy));
      int binz=1+(int)((theta[i])*nbinsz/(maxz));
    int bin = ft->GetBin(binx,biny,binz);

      binres[i]=bin;
      
    double content = ft->GetBinContent(bin);

    if (content!=0) {
        //cout<<" test content "<<content<<" at bin "<<bin<<endl;
        
        content = TMath::Log(content);
        
        
    } else {
      content = -1000;
    }


    if (TMath::Abs(content)>100){
      cout<<" phi "<<phi[i]<<" energy "<<energy[i]<<endl;
      cout<<" bin x "<<binx<<" bin y "<<biny<<endl;
      cout<<" what is the content "<<ft->GetBinContent(bin)<<endl;
      cout<<bin<<" content "<<content<<" total "<<totalcontent<<endl;
    }
    totalcontent+=content;
  }

  delete ft;
  //return totalcontent;

    return binres;
    
}

int lmu::getrandom(model mu, int nobs, double* phi, double* energy, double* theta){


  ft=0;
  ft = new TH3FSpec("ft","ft",nbinsx,0,maxx,nbinsy,0,maxy,nbinsz,0,maxz);
    
    ft->Add(fs.Ucore,fb,mu.Ucore,1);
    ft->AddFast(fs.Umantle,ft,mu.Umantle,1);
    ft->AddFast(fs.Ucrust,ft,mu.Ucrust,1);
    ft->AddFast(fs.Uocean,ft,mu.Uocean,1);
    
    
    ft->AddFast(fs.Thcore,ft,mu.Thcore,1);
    ft->AddFast(fs.Thmantle,ft,mu.Thmantle,1);
    ft->AddFast(fs.Thcrust,ft,mu.Thcrust,1);
    ft->AddFast(fs.Thocean,ft,mu.Thocean,1);
    
    ft->AddFast(fs.Kcore,ft,mu.Kcore,1);
    ft->AddFast(fs.Kmantle,ft,mu.Kmantle,1);
    ft->AddFast(fs.Kcrust,ft,mu.Kcrust,1);
    ft->AddFast(fs.Kocean,ft,mu.Kocean,1);
    

  for (int i=0; i<nobs; i++){
    double out1=0;
      double out2=0;
      double out3=0;

    ft->GetRandom3Spec(out1,out2,out3,0);

    phi[i]=(out1);
      energy[i]=(out2);
      theta[i]=(out3);



  }

  delete ft;
  return 1;

}


int* lmu::getrandom(model mu, int nobs){
    
    int* res=new int[nobs];
    
    ft=0;
    ft = new TH3FSpec("ft","ft",nbinsx,0,maxx,nbinsy,0,maxy,nbinsz,0,maxz);
    
    ft->Add(fs.Ucore,fb,mu.Ucore,1);
    ft->AddFast(fs.Umantle,ft,mu.Umantle,1);
    ft->AddFast(fs.Ucrust,ft,mu.Ucrust,1);
    ft->AddFast(fs.Uocean,ft,mu.Uocean,1);
    
    
    ft->AddFast(fs.Thcore,ft,mu.Thcore,1);
    ft->AddFast(fs.Thmantle,ft,mu.Thmantle,1);
    ft->AddFast(fs.Thcrust,ft,mu.Thcrust,1);
    ft->AddFast(fs.Thocean,ft,mu.Thocean,1);
    
    ft->AddFast(fs.Kcore,ft,mu.Kcore,1);
    ft->AddFast(fs.Kmantle,ft,mu.Kmantle,1);
    ft->AddFast(fs.Kcrust,ft,mu.Kcrust,1);
    ft->AddFast(fs.Kocean,ft,mu.Kocean,1);
    
    
    for (int i=0; i<nobs; i++){
        int out1=ft->GetRandom3Spec(0);
        res[i]=(out1);
    }
    
    delete ft;
    return res;
    
}

#endif
