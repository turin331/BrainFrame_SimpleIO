#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"


#define SIMTIME 6000// in ms, for when no input file is provided

//Change the Time Mux Factor to change the number of simulated cells in the network
////////------Keep Netwotk size in multiples of 96 cells for channel alignment----////////
#define TIME_MUX_FACTOR 384

///----DO NOT TOUCH defines after this point--///
#define HW_CELLS 1
//Number of parameters for states
#define INI_PARAMETERS 24
//Maximum Number of cells and time-multiplexing
#define MAX_N_SIZE 7680 //Make sure this has the same value in the kernel code (NetMaxSize)
//#define MAX_TIME_MUX 10
#define DELTA 0.05
#define CONDUCTANCE 0.004
//IO network size is IO_NETWORK_DIM1*IO_NETWORK_DIM2 times the TIME_MUX_FACTOR
#define IO_NETWORK_SIZE HW_CELLS*TIME_MUX_FACTOR


typedef unsigned long long timestamp;
//Time stamp function for measuring DFE runtime
static timestamp getTimestamp(){

	struct timeval now;
	gettimeofday(&now, NULL);
	return now.tv_usec + (timestamp)now.tv_sec * 1000000;

}


int main(void)
{

    char *outFileName = "InferiorOlive_Output.txt";
    FILE *pOutFile;
    char temp[100];//warning: this buffer may overflow
    long simSteps = 0;
    long simTime = 0;
    //Timing variables
    timestamp t0,t1;
    float V,f, IC;
    float Facc = 0.0;
    float Vacc = 0.0;



    printf("Begin execution \n") ;
	const int IniVectorSize = INI_PARAMETERS* IO_NETWORK_SIZE;


	int sizeBytesFloat = sizeof(float);
	float *IniVector = malloc(sizeBytesFloat * IniVectorSize);
	float *IniICVector = malloc(sizeBytesFloat * IO_NETWORK_SIZE);


	long ticks = 0;
	long TimeMuxFactor = TIME_MUX_FACTOR;
	long MaxNSize = MAX_N_SIZE;
	long NetSize = IO_NETWORK_SIZE;
	//long MaxTimeMux = MAX_TIME_MUX;


	// Cell Parameters Initialization
	float initOneCell[] = {
	/*0*/		-60, //V_dend
	/*1*/		0.0112788,// High-threshold calcium_r
	/*2*/		0.0049291,// Calcium-dependent potassium_s
	/*3*/		0.0337836,// H current_q
	/*4*/		3.7152,// Calcium concentration Ca2Plus
	/*5*/		0.5,// High-threshold calcium current I_CaH
				//Initial somatic parameters
	/*6*/		0.68, //default arbitrary value but it should be randomized per cell g_CaL
	/*7*/		-60, // V_soma
	/*8*/		1.0127807,// Sodium (artificial) Sodium_m
	/*9*/		0.3596066, //Sodium_h
	/*10*/		0.2369847,// Potassium (delayed rectifier) Potassium_n
	/*11*/		0.2369847, // Potassium_p
	/*12*/    	0.1,// Potassium (voltage-dependent) Potassium_x_s
	/*13*/		0.7423159,// Low-threshold calcium Calcium_k
	/*14*/		0.0321349,// Calcium_l
				// Initial axonal parameters
	/*15*/		-60, // v_Axon
				//sisaza: Sodium_m_a doesn't have a state, therefore this assignment doesn'thave any effect
	/*16*/		0.003596066,// Sodium (thalamocortical)  // Sodium_m_a
	/*17*/		0.9,// Sodium_h_a
	/*18*/		0.2369847,// Potassium (transient) // Potassium_x_a
	//filler for channel alignment
				0,
				0,
				0,
				0,
				0

			 };

//Create initial state input for all cells
	printf("Creating State Initialization Vector\n");
	for(long i = 0; i< IniVectorSize ; i=i+INI_PARAMETERS){
		for(long j = 0; j<INI_PARAMETERS; j++){
			IniVector[i+j] = initOneCell[j];
			//printf(" Ini Vector : %ld \n", i+j);
		}
	}

//Create Initial ICs (Gap Junction Influences)
	printf("Creating Initialization Gap Junction Vector\n");
	for(long k = 0; k< IO_NETWORK_SIZE ; k++){
		for(long l = 0; l< IO_NETWORK_SIZE ; l++){
			V = IniVector[k*24] - IniVector[l*24];
			f = V * exp(-0.01 * V * V);
			Facc = f + Facc;
			Vacc = V + Vacc;
		}
		IC = CONDUCTANCE * (8 * Facc + 2 * Vacc);
		IniICVector[k] = IC;		//printf("%ld : %.2f ",k, IniICVector[k]);
	}
	//printf("\n ");


//Create output file
    pOutFile = fopen(outFileName,"w");
    if(pOutFile==NULL){
        printf("Error: Couldn't create %s\n", outFileName);
        exit(EXIT_FAILURE);
    }
    sprintf(temp, "#simSteps iApp #Neuron Output(V_axon)\n");
    fputs(temp, pOutFile);


    simTime = SIMTIME; // in miliseconds
    simSteps = ceil(simTime/DELTA);
    ticks = (simSteps * IO_NETWORK_SIZE * IO_NETWORK_SIZE);
//Malloc evoked input memory
    float *iApp = malloc(sizeBytesFloat * simSteps * IO_NETWORK_SIZE);
//Initialise iApp trace
    printf("Creating Input Trace\n");
    for (long i = 0 ; i < simSteps * IO_NETWORK_SIZE ; i++){
    	if (i > ((20000-1) * IO_NETWORK_SIZE) + IO_NETWORK_SIZE-1 && i < (20500-1) * IO_NETWORK_SIZE){
    		iApp[i]= 6.0;

    	}
    	else{
    		iApp[i]= 0.0;
    	}
    }
//Malloc output memory
    long sizeBytesFloatOut = simSteps*IO_NETWORK_SIZE* sizeBytesFloat;
    printf("Mallocing Output Space\n");
    float *Out = malloc(sizeBytesFloatOut);
//Write ini state and input memory RAM
	printf("Writing Initialization State to LMem.\n");
	infoli_writeLMem(INI_PARAMETERS * IO_NETWORK_SIZE, 0, IniVector);
	printf("Writing Input Trace State to LMem.\n");
	infoli_writeLMem( simSteps*IO_NETWORK_SIZE, INI_PARAMETERS * (MaxNSize), iApp);
	printf("Writing IC Initialization Trace State to LMem.\n");
	infoli_writeLMem( IO_NETWORK_SIZE, (INI_PARAMETERS * (MaxNSize))+ (simSteps*(MaxNSize)+(simSteps * (MaxNSize))),IniICVector);


//execute kernel
	printf("Running on DFE. Net Size = %ld and SimSteps = %ld\n", NetSize, simSteps);
	t0 = getTimestamp();
	infoli(MaxNSize, ticks, NetSize, simSteps, TimeMuxFactor);
	t1 = getTimestamp();

	printf("DFE Runtime = %9.7f seconds\n", (t1-t0)/1000000.0 );

	printf("Reading memory\n");
	fflush(stdout);
	infoli_readLMem( simSteps * IO_NETWORK_SIZE ,(INI_PARAMETERS * (MaxNSize))+ (simSteps*(MaxNSize)), Out);

//Write output to file
	printf("Writing Output File\n");
	for (long i=0; i<simSteps ; i++){
		sprintf(temp, "%ld %.2f ", i+1, iApp[i* IO_NETWORK_SIZE]);
    	fputs(temp, pOutFile);
    	int cell = -1;
		for (long j=i*IO_NETWORK_SIZE; j<(i*IO_NETWORK_SIZE)+IO_NETWORK_SIZE; j++){
			cell++;
			sprintf(temp, " %d : %.8f ",cell, Out[j]);
			fputs(temp, pOutFile);
		}
		sprintf(temp, "\n ");
    	fputs(temp, pOutFile);

	}


	//fclose (pOutFile);
	printf("Done.\n");
	return 0;
}
