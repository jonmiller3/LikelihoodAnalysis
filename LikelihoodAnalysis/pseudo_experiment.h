#ifndef PSEUDO_EXPERIMENT_H
#define PSEUDO_EXPERIMENT_H

#include <vector>
#include <map>
#include <thread>
#include "lmu.h"

#include "Helper.h"
//class lmu;

//#define USECPU
#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#define DOUBLEFAIL
#endif

#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#else
#include <stdio.h>
#include <string.h>
#include "CL/cl.h"
#define FALSE false
#define TRUE true
#endif

using namespace std;


float* float_from_model(model mu){
  
    float* res = new float[12];
    
    res[0]=mu.Ucore;
    res[1]=mu.Umantle;
    res[2]=mu.Ucrust;
    res[3]=mu.Uocean;
    res[4]=mu.Ucore;
    res[5]=mu.Umantle;
    res[6]=mu.Ucrust;
    res[7]=mu.Uocean;
    res[8]=mu.Ucore;
    res[9]=mu.Umantle;
    res[10]=mu.Ucrust;
    res[11]=mu.Uocean;
    
    return res;
    
};


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
    float* model_in_bins;
    
    

 public:
  pseudo_experiment(int, model, lmu*);
  void run(bool);
    void rungpu();
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

void pseudo_experiment::rungpu(){
    
    #ifdef DOUBLEFAIL
    std::cout<<" no doubles "<<std::endl;
    float* data_out;
#else
    double* data_out;
    std::cout<<" doubles! "<<std::endl;
#endif
    
    // I could do everything here
    // I think that I hardcode it to do
    // n=4 loops
    
    
    // I am just putting everything here
    cl_command_queue commandQueue;
    
    // new CL variables
    cl_context       cxGPUContext;
    cl_kernel        kernel[MAX_GPU_COUNT];
    cl_program       program[MAX_GPU_COUNT];
    
    char (*cDevicesName)[256];
    char (*cDevicesExt)[256];
    cl_ulong cDevicesGlobalMem;
    
    cl_platform_id  cpPlatform;
    cl_device_id   *cdDevices;
    cl_int          ciErrNum;
    cl_uint         ciDeviceCount;

    
    cl_event gpudone_event;
    
    // initialize
    cpPlatform    = NULL;
    cdDevices     = NULL; // should that be *cdDevices ??
    ciErrNum      = CL_SUCCESS;
    
    ciDeviceCount = 0;
    
    bool bEnableProfile = false; // This is to enable/disable OpenCL based profiling

    int ncells=plmu->getncells();
#ifdef DOUBLEFAIL
    size_t  mem_size_input_data = sizeof(cl_float)*ncells*13;
#else
    size_t  mem_size_input_data = sizeof(cl_double)*ncells*13;
#endif
    
    cl_uint num_platforms;
    ciErrNum = clGetPlatformIDs (0, NULL, &num_platforms);
    if (ciErrNum != CL_SUCCESS)
    {
        std::cout<<" failed "<<std::endl;

    }
    
    // this is to select..
    cl_platform_id* clPlatformIDs;
    (clPlatformIDs = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id)));
    ciErrNum = clGetPlatformIDs (num_platforms, clPlatformIDs, NULL);
    //ciErrNum = oclGetPlatformIDs(&cpPlatform);
    if (ciErrNum != CL_SUCCESS)
    {
        std::cout<<" failed "<<std::endl;
        //shrLog("Error: Failed to create OpenCL context!\n");
    }
    
    // only one platform?
    cpPlatform=clPlatformIDs[0];
    
    
#ifdef USECPU
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_CPU, 0, NULL, &ciDeviceCount);
    cdDevices = (cl_device_id *)malloc(ciDeviceCount * sizeof(cl_device_id) );
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_CPU, ciDeviceCount, cdDevices, NULL);
#else
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 0, NULL, &ciDeviceCount);
    cdDevices = (cl_device_id *)malloc(ciDeviceCount * sizeof(cl_device_id) );
    ciErrNum = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, ciDeviceCount, cdDevices, NULL);
#endif
    // Allocate a buffer array to store the names GPU device(s)
    cDevicesName = new char[ciDeviceCount][256];
    
    std::cout<<" device count "<<ciDeviceCount<<std::endl;
    
    
    if (ciErrNum != CL_SUCCESS)
    {
        std::cout<<" failed to get IDs"<<ciErrNum<<std::endl;

    } else
    {
        for (int i=0; i<(int)ciDeviceCount; i++) {
            clGetDeviceInfo(cdDevices[i], CL_DEVICE_NAME, sizeof(cDevicesName[i]), &cDevicesName[i], NULL);
            clGetDeviceInfo(cdDevices[i], CL_DEVICE_EXTENSIONS, sizeof(cDevicesExt[i]), &cDevicesExt[i], NULL);
            clGetDeviceInfo(cdDevices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cDevicesGlobalMem), &cDevicesGlobalMem, NULL);
            std::cout<<"> OpenCL Device "<<cDevicesName[i]<<", cl_device_id: "<<cdDevices[i]<<std::endl;
            std::cout<<"> OpenCL Ext "<<cDevicesExt[i]<<", global mem: "<<cDevicesGlobalMem<<std::endl;
        }
    }
    
    //Create the OpenCL context
    cxGPUContext = clCreateContext(0, ciDeviceCount, cdDevices, NULL, NULL, &ciErrNum);
    if (ciErrNum != CL_SUCCESS)
    {
        std::cout<<" failed to create context"<<ciErrNum<<std::endl;
    }
    
    std::cout<<" I think that I have produced all the contexts that I need "<<std::endl;
    
    
    
    
    
    if (ciErrNum != CL_SUCCESS)
    {
        std::cout<<" problem creating output buffer "<<std::endl;
        //shrLog("Error: clCreateBuffer\n");
    }
    
    
    for (int i=1; i<ciDeviceCount; i++){
        
        commandQueue = 0;
        
        commandQueue = clCreateCommandQueue(cxGPUContext, cdDevices[i], (bEnableProfile ? CL_QUEUE_PROFILING_ENABLE : 0), &ciErrNum);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem creating max command queue "<<ciErrNum<<std::endl;
        }
        
        char* kernelname;
        
        std::string pPath = getenv ("HOME");
        std::string basename = pPath+"/Dropbox/Geo_neutrinos/likelihood_analysis/LikelihoodAnalysis/";
        
        
#ifdef __APPLE__
        CompileOCLKernel(cdDevices[i], cxGPUContext, (basename+"/LikelihoodAnalysis/RunExperiment.cl").c_str(), &program[i]);
#else
        CompileOCLKernel(cdDevices[i], cxGPUContext, "LikelihoodAnalysis/RunExperiment.cl", &program[i]);
#endif
        kernelname="RunExperiment";
        
  
      
        
        
        kernel[i]= clCreateKernel(program[i], kernelname, &ciErrNum);
        if (ciErrNum != CL_SUCCESS) {
            std::cout<<" problem with creating kernel "<<std::endl;
        }
        
        


        // now handle constant memory
        size_t mem_size_const_in = sizeof(cl_int)*nobs;
        
        cl_mem input_data_in_bins = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                                          mem_size_const_in, data_in_bins, &ciErrNum);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem creating data buffer "<<ciErrNum<<std::endl;
        }
        // this will be done in general
        int* nums= new int[2];
        nums[0]=nobs;
        nums[1]=ncells;
        cl_mem input_nums = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                                          sizeof(cl_int)*2, nums, &ciErrNum);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem creating inputs buffer "<<std::endl;
        }


        
        // define the arguments
        ciErrNum=clSetKernelArg(kernel[i], 1, sizeof(cl_mem), (void *) &input_data_in_bins);
        
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem setting argument 1 "<<std::endl;
        }
        ciErrNum=clSetKernelArg(kernel[i], 2, sizeof(cl_mem), (void *) &input_nums);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem setting argument 2 "<<std::endl;
        }
        
        

        
        
        cl_mem dest_data = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY,
                                         mem_size_const_in, NULL, NULL);
        
        cl_mem dest_num = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY,
                                        2* sizeof(cl_int), NULL, NULL);

  
        
        // set the constant memory (done once per kernel)
        
        ciErrNum = clEnqueueCopyBuffer(commandQueue,
                                       input_data_in_bins, // src_buffer
                                       dest_data, // dst_buffer
                                       0, // src_offset
                                       0, // dst_offset
                                       mem_size_const_in,  // size_of_bytes to copy
                                       0,  // number_events_in_waitlist
                                       NULL, /// event_wait_list
                                       NULL); // event
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: Failure to copy max buffer "<<ciErrNum<<std::endl;
        }

        
        ciErrNum = clEnqueueCopyBuffer(commandQueue,
                                       input_nums, // src_buffer
                                       dest_num, // dst_buffer
                                       0, // src_offset
                                       0, // dst_offset
                                       sizeof(cl_int)*2,  // size_of_bytes to copy
                                       0,  // number_events_in_waitlist
                                       NULL, /// event_wait_list
                                       NULL); // event
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: Failure to copy min buffer "<<std::endl;
            //shrLog("clEnqueueCopyBuffer() Error: Failed to copy buffer!\n");
        }


        
        clFlush(commandQueue);

        
        
        
        
        // sets the buffers
        cl_mem input_data = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, mem_size_input_data, NULL, &ciErrNum);
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem creating buffer "<<std::endl;
            //shrLog("Error: clCreateBuffer\n");
        }

        // sets the buffers
        cl_mem input_model = clCreateBuffer(cxGPUContext, CL_MEM_READ_ONLY, sizeof(cl_float)*13, NULL, &ciErrNum);
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem creating buffer "<<std::endl;
            //shrLog("Error: clCreateBuffer\n");
        }
        
        // I am going to think that there is 1024 total workgroups
#ifdef DOUBLEFAIL
        size_t  mem_size_output_data=sizeof(cl_float)*1024;
#else
        size_t  mem_size_output_data=sizeof(cl_double)*1024;
#endif
        
        
        cl_mem output_data =
        clCreateBuffer(cxGPUContext, CL_MEM_WRITE_ONLY,  mem_size_output_data, NULL, &ciErrNum);
        
        
        
        
        // this sets the arguments (ned to change it)
        ciErrNum = clSetKernelArg(kernel[i], 0, sizeof(cl_mem), (void *) &input_data);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem setting argument 0 "<<std::endl;
        }
        ciErrNum = clSetKernelArg(kernel[i], 3, sizeof(cl_mem), (void *) &input_model);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem setting argument 3 "<<std::endl;
        }

        ciErrNum = clSetKernelArg(kernel[i], 4, sizeof(cl_mem), (void *) &output_data);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" problem setting argument 4 "<<std::endl;
        }
        
        
        //set the global memory (done multiple times per kernel, but only once here)
        
        
        ciErrNum = clEnqueueWriteBuffer(commandQueue, input_data,  // que and clmem
        //ciErrNum = clEnqueueCopyBuffer(commandQueue, model_in_bins,input_data,0,0,mem_size_input_data,0,NULL,NULL);
                                       CL_TRUE, 0, // blocking? offset
                                        mem_size_input_data, model_in_bins, // size? and input mem
                                        0, NULL, NULL); // event wait list, events in wait list, event
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: Failure to copy buffer "<<std::endl;

        }

        ciErrNum = clEnqueueWriteBuffer(commandQueue, input_model,  // que and clmem
                                        //ciErrNum = clEnqueueCopyBuffer(commandQueue, model_in_bins,input_data,0,0,mem_size_input_data,0,NULL,NULL);
                                        CL_TRUE, 0, // blocking? offset
                                        sizeof(cl_float)*13, float_from_model(maxmu), // size? and input mem
                                        0, NULL, NULL); // event wait list, events in wait list, event
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: Failure to copy buffer "<<std::endl;
            
        }
        
        
        // let's get the size for the work items
        
        size_t workitem_size[3];
        ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL);
        if (ciErrNum != CL_SUCCESS)
        {
            // there is a problem
            std::cout<<" problem with work item size "<<ciErrNum<<std::endl;
        }
        
        size_t maxworkitem_size;
        ciErrNum = clGetDeviceInfo(cdDevices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(maxworkitem_size), &maxworkitem_size, NULL);
        if (ciErrNum != CL_SUCCESS)
        {
            // there is a problem
            std::cout<<" problem max size "<<ciErrNum<<std::endl;
        }
        printf("what is max %lu",maxworkitem_size);
        
        size_t testworkgroup;
        ciErrNum = clGetKernelWorkGroupInfo(kernel[i], cdDevices[i], CL_KERNEL_WORK_GROUP_SIZE, sizeof(testworkgroup), &testworkgroup, NULL);
        if (ciErrNum != CL_SUCCESS)
        {
            printf("Error: Failed to retrieve kernel work group info! %d\n", ciErrNum);
            
        }
        printf(" what is workgroup test %lu \n",testworkgroup);
        size_t globalWorkSize[] = {maxworkitem_size, 1, 1};
        size_t localWorkSize[] = {testworkgroup,1,1};
        
        
        printf("this is the worksize %lu %lu %lu \n",workitem_size[0],workitem_size[1],workitem_size[2]);
        printf("this is the local worksize %lu %lu %lu \n",
               localWorkSize[0],localWorkSize[1],localWorkSize[2]);
        printf("this is the global worksize %lu %lu %lu \n",
               globalWorkSize[0],globalWorkSize[1],globalWorkSize[2]);
        
        
        // now we come to the point where it runs the kernel and gets the result
        std::cout<<" go device "<<i<<std::endl;
        
        clFinish(commandQueue);
        ciErrNum=clEnqueueNDRangeKernel(commandQueue, kernel[i], 3, 0, globalWorkSize, localWorkSize,
                                        0, NULL, NULL);
        
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: in running kernel "<<ciErrNum<<std::endl;
        }
        
        std::cout<<" ready to finish "<<i<<std::endl;
        
        clFinish(commandQueue);
        
        //std::cout<<" ready to read out "<<i<<std::endl;

        
        ciErrNum = clEnqueueReadBuffer(commandQueue, output_data, CL_FALSE, 0, mem_size_output_data, data_out, NULL, NULL, &gpudone_event);
        if (ciErrNum != CL_SUCCESS)
        {
            std::cout<<" Error: in reading buffer "<<std::endl;
            
        }
        
        //std::cout<<" ready to finish  "<<i<<std::endl;

        
        // this crashes it... why?
        clFinish(commandQueue);
        
        
    }
    
    


    
    
    
    
}

void pseudo_experiment::run(bool createvector=false){
  
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
                                                        
                                                        float content=0;
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
                                                        
                                                        l+=(double)content;
                                                        
                                                    }
                  // these are 50% of the use?
                                                    // all I need is the maximum, I do not need the full set
                                                    // I can even just have a setting that I turn on/off
                                                    if (createvector){
			      lt_set.push_back(l);
			      mu_set.push_back(mutest);
                                                    }
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
