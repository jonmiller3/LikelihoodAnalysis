#ifndef PSEUDO_EXPERIMENT_H
#define PSEUDO_EXPERIMENT_H

#include <vector>
#include <map>
#include <thread>
#include "lmu.h"
//class lmu;

using namespace std;

class pseudo_experiment{

 private:
  double* phi;
    double* energy;
    double* theta;
  lmu* plmu;
  int nobs;
  model mu_true;
  vector<double> lt_set;
  vector<model> mu_set;
  double maxl;
  model maxmu;

    int* data_in_bins;
    
    // problem is that I would prefer double and not float?
    double* model_in_bins;
    
    

 public:
  pseudo_experiment(int, model, lmu*);
  void run();
  double getlogr(){double calcres; plmu->calc(phi,energy,theta,mu_true,nobs,calcres); return (calcres-maxl);}
    pseudo_experiment(){delete phi; delete plmu; delete energy; delete theta;}
  model* getrange(map<model,double>);

};

pseudo_experiment::pseudo_experiment(int n, model mt, lmu* l){
  nobs=n;
  mu_true=mt;
  plmu = l;
    
    // this is one way to do it
  energy = new double[nobs];
    phi = new double[nobs];
    theta = new double[nobs];

  plmu->getrandom(mu_true,nobs,phi,energy,theta);
    
    
    // this is another way
    data_in_bins=plmu->getrandom(mu_true, nobs);
    
    // another way to do memory
    model_in_bins=plmu->getmodelarray();
    
  maxl=0;
    
    
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
    
}

void pseudo_experiment::run(){
  
    model mu=maxmu;
    model mutest;
    // I think that I need to do 12 loops
    int i=0;
    
    double tval[13];

    double np=2.;
    
    tval[0]=1;
    
    for (int iUcore=-np; iUcore<np; iUcore++){
        mutest.Ucore=(np+iUcore);
        tval[1]=(np+iUcore);
        for (int iUmantle=-np; iUmantle<np; iUmantle++){
            mutest.Umantle=mu.Umantle*(1+iUmantle/np);
            tval[2]=mu.Umantle*(1+iUmantle/np);
            for (int iUcrust=-np; iUcrust<np; iUcrust++){
                mutest.Ucrust=mu.Ucrust*(1+iUcrust/np);
                tval[3]=mutest.Ucrust;
                for (int iUocean=-np; iUocean<np; iUocean++){
                    mutest.Uocean=mu.Uocean*(1+iUocean/np);
                    tval[4]=mutest.Uocean;
                    for (int iThcore=-np; iThcore<np; iThcore++){
                        mutest.Thcore=(np+iThcore);
                        for (int iThmantle=-np; iThmantle<np; iThmantle++){
                            mutest.Thmantle=mu.Thmantle*(1+iThmantle/np);
                            for (int iThcrust=-np; iThcrust<np; iThcrust++){
                                mutest.Thcrust=mu.Thcrust*(1+iThcrust/np);
                                for (int iThocean=-np; iThocean<np; iThocean++){
                                    mutest.Thocean=mu.Thocean*(1+iThocean/np);
                                    for (int iKcore=-np; iKcore<np; iKcore++){
                                        mutest.Kcore=(np+iKcore)*1e4;
                                        for (int iKmantle=-np; iKmantle<np; iKmantle++){
                                            mutest.Kmantle=mu.Kmantle*(1+iKmantle/np);
                                            for (int iKcrust=-np; iKcrust<np; iKcrust++){
                                                mutest.Kcrust=mu.Kcrust*(1+iKcrust/np);
                                                for (int iKocean=-np; iKocean<np; iKocean++){
                                                    
                                                    mutest.Kocean=mu.Kocean*(1+iKocean/np);
                                                    
                          tval[5]=mutest.Thcore;
                                                    tval[6]=mutest.Thmantle;
                                                    tval[7]=mutest.Thcrust;
                                                    tval[8]=mutest.Thocean;
                                                    tval[9]=mutest.Kcore;
                                                    tval[10]=mutest.Kmantle;
                                                    tval[11]=mutest.Kcrust;
                                                    tval[12]=mutest.Kocean;
                   
                                                    // slower way?
			      double lt;
			      //data_in_bins=plmu->calc(phi,energy,theta,mutest,nobs,lt);
                                          
                                                    
                                                    // faster way?
                                                    double l=0;
                                                    int ncells=plmu->getncells();
                                                    
                                                    for (int i=0; i<nobs; i++){
                                                        
                                                        double content=0;
                                                        for (int modelnumber=0; modelnumber<13; modelnumber++){
                                                            content+=(model_in_bins[modelnumber*ncells+data_in_bins[i]]*tval[modelnumber]);
                                                            //cout<<" l steps "<<content<<" step "<<modelnumber*ncells+data_in_bins[i]<<" modelval "<<tval[modelnumber]<<" mnum "<<modelnumber<<endl;
                                                        }
                                                        if (content!=0) {
                                                            //cout<<" l content "<<content<<" at bin "<<data_in_bins[i]<<endl;
                                                            content = TMath::Log(content);
                                                            
                                                        } else {
                                                            content = -1000;
                                                        }
                                                        
                                                        l+=content;
                                                        
                                                    }
                                                    
			      lt_set.push_back(l);
			      mu_set.push_back(mutest);
                              
                                                    //cout<<" here is lt "<<lt<<" here is l "<<l<<endl;
                                                    
			      if (lt>maxl||i==0){
				maxl=lt;
				maxmu=mutest;
			      }
			      i++;
                              
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
        }
    }

  return;

}

model* pseudo_experiment::getrange(map<model,double> rcrit){

  model* out = new model[2];
  vector<double>::iterator it;
  vector<model>::iterator it_mu=mu_set.begin();
    bool set=false;

  for (it=lt_set.begin(); it!=lt_set.end(); ++it,++it_mu){

      double r = *it - maxl;

    if (r>rcrit[*it_mu]){
      // the only condition that matters
      if (set){
          set=true;
	out[0]=*it_mu;
      }
      out[1]=*it_mu;

    }

  }
  return out;
}

#endif
