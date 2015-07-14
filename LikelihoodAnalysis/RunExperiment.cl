//#include <OpenCL/OpenCL.h>

// a bit silly, I probably should just stick in the value below into the proper place
//inline float modelvalue( const float* pdf , int modelnumber, int numbins, float theoryval, int databin){
//    return (float)( pdf[modelnumber*numbins+databin]*theoryval);
//}

// I should put in the basic model in
//

// float or double?
kernel void RunExperiment(global const float* pdf, global const int* data, int numbins, int numdata)
{
    
    // data is bin number
    int iGD = get_global_id(0);
    
    // this is for 1024 workgroup size
    //int i8=iGD%8;
    //int iGD8=((iGD-i8)/8);
    //int i64=iGD8%8;
    //int i512=(iGD8-i64)/8)%8;

    
    
    
    // max should be -1000 or 0?
    float maxl=0;
    float tval[13];
    float t0val[13];

    t0val[0]=1;
    t0val[1]=1;
    t0val[2]=1;
    t0val[3]=1;
    t0val[4]=1;
    t0val[5]=1;
    t0val[6]=1;
    t0val[7]=1;
    t0val[8]=1;
    t0val[9]=1;
    t0val[10]=1;
    t0val[11]=1;
    t0val[12]=1;
    
    // background is strength 1 always
    tval[0]=1;
    
    float np=8.;
    
    for (int iUcore=-np; iUcore<np; iUcore++){
        tval[1]=(np+iUcore);
        for (int iUmantle=-np; iUmantle<np; iUmantle++){
            tval[2]=t0val[2]*(1+iUmantle/np);
            for (int iUcrust=-np; iUcrust<np; iUcrust++){
                tval[3]=t0val[3]*(1+iUcrust/np);
                for (int iUocean=-np; iUocean<np; iUocean++){
                    tval[4]=t0val[4]*(1+iUocean/np);
                    for (int iThcore=-np; iThcore<np; iThcore++){
                        tval[5]=(np+iThcore);
                        for (int iThmantle=-np; iThmantle<np; iThmantle++){
                            tval[6]=t0val[6]*(1+iThmantle/np);
                            for (int iThcrust=-np; iThcrust<np; iThcrust++){
                                tval[7]=t0val[7]*(1+iThcrust/np);
                                for (int iThocean=-np; iThocean<np; iThocean++){
                                    tval[8]=t0val[8]*(1+iThocean/np);
                                    for (int iKcore=-np; iKcore<np; iKcore++){
                                        tval[9]=(np+iKcore)*1e4;
                                        for (int iKmantle=-np; iKmantle<np; iKmantle++){
                                            tval[10]=t0val[10]*(1+iKmantle/np);
                                            for (int iKcrust=-np; iKcrust<np; iKcrust++){
                                                tval[11]=t0val[11]*(1+iKcrust/np);
                                                for (int iKocean=-np; iKocean<np; iKocean++){
                                                    
                                                    tval[12]=t0val[12]*(1+iKocean/np);
   
                                                    float l=0;
   
    
                                                    for (int i=0; i<numdata; i++){
        
                                                        int databin=data[i];
                                                        for (int modelnumber=0; modelnumber<13; modelnumber++){
                                                            l+=(pdf[modelnumber*numbins+databin]*tval[modelnumber]);
                                                        }
                                                        
                                                    }
                                                    
                                                    if (l<maxl) maxl=l;
                                                    
                                                    
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
    
    
    
    
    
    
    
}