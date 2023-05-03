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
	
public:
	Scheduler();
	void ReadFile();
	void MoveToTRM(Process* p,int t);
	int GetNumP();
	int GetTRMcount();
	int getMaxW();
	void NEWtoRDY(int t);
	void RUNtoBLK(Process* p);
	void UpdateWT();
	void IOreq(int t);
	void BLKtoRDY();
	Processor* Get_ShortestRDY(bool b);
	void updateRemainingCT();


	void RUNtoRDY(Process*p);
	bool MigrationRRtoSJF(Process* p);
	Processor* Get_LongestRDY();
	bool MigrationFCFStoRR(Process* p);
	void Killing(int timestep);
	void Stealing(int timestep);
	double Calc_StealLimit(Processor* longest, Processor* shortest);
	void simulation();
	void Fork(int timestep);
	void killOrphan(Process* orphan);

	void OutputFile();
};


#endif
