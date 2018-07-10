#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Maxfiles.h"
#include "MaxSLiCInterface.h"


#define SIMTIME 1// in ms, for when no input file is provided
//Change the Time Mux Factor to change the number of simulated cells in the network
////////------Keep Netwotk size in multiples of 96 cells for channel alignment----////////
#define TIME_MUX_FACTOR 3840

///----DO NOT TOUCH defines after this point--///
#define HW_CELLS 1
//Number of parameters for states + filler
#define INI_PARAMETERS 24
//Number of parameters for Neuron Characteristics  + filler
#define NEURON_PARAMETERS 24
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


int main(int argc,char *argv[])
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
    long NetSize = 0;
    long TimeMuxFactor= 0;



	if (argc >1){
		NetSize = atoi(argv[1]);
		TimeMuxFactor = NetSize/HW_CELLS ;
		if (atoi(argv[1])%96!=0){
			printf("Network size must be a multiple of 96 cells");
			return 0;
		}
	}
	else{
		NetSize = IO_NETWORK_SIZE;
		TimeMuxFactor = TIME_MUX_FACTOR;
	}

    printf("Begin execution \n") ;
	const int IniVectorSize = INI_PARAMETERS* NetSize;



	int sizeBytesFloat = sizeof(float);
	float *IniVector = malloc(sizeBytesFloat * IniVectorSize);
	float *IniICVector = malloc(sizeBytesFloat * NetSize);
	float *NeuronCharCVector = malloc(sizeBytesFloat * NEURON_PARAMETERS*NetSize);
	float *ConnectivityVector = malloc(sizeBytesFloat * NetSize*NetSize);


	long ticks = 0;

	long MaxNSize = MAX_N_SIZE;


	//long MaxTimeMux = MAX_TIME_MUX;

	max_file_t *maxfile = infoli_init();
	long BurstLengthInBytes = max_get_burst_size(maxfile, "cmd_tolmem");
	printf("Burst size: %ld \n",  BurstLengthInBytes);


	// Cell State Initialization
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

	// Neuron Characteristics Initialization
	float CharactInit[] = {
				// Capacitance
	/*0*/		1.0, //CM
				// Somatic conductances (mS/cm2)
	/*1*/		150,// G_NA_S
	/*2*/		9.0,// G_KDR_S
	/*3*/		5.0,// G_K_S
	/*4*/		0.016,// G_LS
				// Dendritic conductances (mS/cm2)
	/*5*/		35.0,// G_K_CA
	/*6*/		4.5, //G_CAH
	/*7*/		0.016, // G_LD
	/*8*/		0.125,// G_H
				// Axon hillock conductances (mS/cm2)
	/*9*/		240.0, //G_NA_A
	/*10*/		20.0,// G_K_A
	/*11*/		0.016, // G_LA
				// Cell morphology
	/*12*/    	0.25,// P1
	/*13*/		0.15,// P2
	/*14*/		0.13,// G_INT
				// Reversal potentials
	/*15*/		55.0, // V_NA
	/*16*/		-75.0,// V_K
	/*17*/		120.0,// V_CA
	/*18*/		-43.0,// V_H
	/*19*/		10,//V_L
				//filler for channel alignment
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

		}
	}

//Define initial cell characteristics/conductances
	printf("Creating filler for neuron characteristics Vector\n");
	for(long i = 0; i< NetSize ; i++){
		//CharactInit[0]++; // This is for debugging
		for(long j = 0; j<NEURON_PARAMETERS; j++){

			NeuronCharCVector[(i*NEURON_PARAMETERS)+j] = CharactInit[j];
			//printf(" Ini Vector : %ld \n", i+j);
		}
	}

//Create Connectivity Vector
	printf("Creating Connectivity Vector\n");
	for(long k = 0; k<NetSize*NetSize; k++){
		ConnectivityVector[k] = 0.004;
	}

//Create Initial ICs (Gap Junction Influences)
	printf("Creating Initialization Gap Junction Vector\n");
	for(long k = 0; k< NetSize ; k++){
		for(long l = 0; l< NetSize ; l++){
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

    ticks = (simSteps * NetSize * NetSize);
//Malloc evoked input memory
    float *iApp = malloc(sizeBytesFloat * simSteps * NetSize);
//Initialise iApp trace
    printf("Creating Input Trace\n");
    for (long i = 0 ; i < simSteps * NetSize ; i++){
    	if (i > ((20000-1) * NetSize) + NetSize-1 && i < (20500-1) * NetSize){
    		iApp[i]= 6.0;

    	}
    	else{
    		iApp[i]= 0.0;
    	}
    }
//Malloc output memory
    long sizeBytesFloatOut = simSteps*NetSize* sizeBytesFloat;
    printf("Mallocing Output Space\n");
    float *Out = malloc(sizeBytesFloatOut);
//Write ini state and input memory RAM
	printf("Writing Initialization State to LMem.\n");
	infoli_writeLMem(INI_PARAMETERS * NetSize, 0, IniVector);
	printf("Writing Input Trace State to LMem.\n");
	infoli_writeLMem( simSteps*NetSize, INI_PARAMETERS * (MaxNSize), iApp);
	printf("Writing IC Initialization Trace State to LMem.\n");
	infoli_writeLMem( NetSize, (INI_PARAMETERS * (MaxNSize))+ (simSteps*(MaxNSize)),IniICVector);
	printf("Writing Neuron Characteristics Trace to LMem.\n");
	infoli_writeLMem( NEURON_PARAMETERS*NetSize, (INI_PARAMETERS * (MaxNSize))+ (simSteps*(MaxNSize)) + MaxNSize,NeuronCharCVector);
	printf("Writing Connectivity Vector Trace State to LMem.\n");
	infoli_writeLMem( NetSize*NetSize, (INI_PARAMETERS * (MaxNSize))+ simSteps*(MaxNSize)+ MaxNSize + (NEURON_PARAMETERS*MaxNSize) ,ConnectivityVector);

	printf("conductance Vector Address: %ld \n",  (INI_PARAMETERS * (MaxNSize))+ simSteps*(MaxNSize)+ MaxNSize );
//execute kernel
	printf("Running on DFE. Net Size = %ld and SimSteps = %ld\n", NetSize, simSteps);
	t0 = getTimestamp();
	infoli(BurstLengthInBytes, MaxNSize, ticks, NetSize, simSteps, TimeMuxFactor);
	t1 = getTimestamp();

	printf("DFE Runtime = %9.7f seconds\n", (t1-t0)/1000000.0 );

	printf("Reading memory\n");
	fflush(stdout);
	infoli_readLMem( simSteps * NetSize, (INI_PARAMETERS * (MaxNSize))+ simSteps*(MaxNSize)+ MaxNSize + (NEURON_PARAMETERS*MaxNSize)+ MaxNSize*MaxNSize, Out);


//Write output to file
	printf("Writing Output File\n");
	for (long i=0; i<simSteps ; i++){
		sprintf(temp, "%ld %.2f ", i+1, iApp[i* NetSize]);
    	fputs(temp, pOutFile);
    	int cell = -1;
		for (long j=i*NetSize; j<(i*NetSize)+NetSize; j++){
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
