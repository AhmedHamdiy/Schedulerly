#ifndef _SCHEDULER
#define _SCHEDULER


#include"../DS Implementation/Pair/pair.h"
#include"Processors/FCFS_Processor.h"
#include"Processors/SJF_Processor.h"
#include"Processors/RR_Processor.h"
#include<fstream>
using namespace std;



class Scheduler 
{
	Processor** ProcessorList;
	LinkedQueue<Process*> NewList;
	LinkedQueue<Process*> BLKList;
	LinkedQueue<Process*> TRMList;
	LinkedQueue<Pair<int, int>> KillList;
	int Output_Mode;
	int NF;
	int NS;
	int NR;
	int RRT;
	int RTF;
	int MaxW;
	int STL;
	int ForkP;
	int NumP;
	int TRMcount;
	int ProcessorCounter;
	Processor* MinTime;
	Processor* MaxTime;

public:
	Scheduler();
	void ReadFile();
	void MoveToTRM(Process* p);
	int GetNumP();
	int GetTRMcount();
	int getMaxW();
	void NEWtoRDY(int t);
	void RUNtoBLK(Process* p);
	void BLKtoRDY();
	void RUNtoRDY(Process*p);
	bool MigrationRRtoSJF(Process* p);
	void MigrationFCFStoRR(Process* p);
	void WorkStealing(){};
	void CreateForked(){};
	void GetMinMax();
	void Killing();
	void simulation();
};


#endif
