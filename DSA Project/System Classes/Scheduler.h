#ifndef _SCHEDULER
#define _SCHEDULER


#include"../DS Implementation/Pair/pair.h"
#include"Processors/FCFS_Processor.h"
#include"Processors/SJF_Processor.h"
#include"Processors/RR_Processor.h"
#include"Processors/EDF_Processor.h"
#include"UI.h"
#include<fstream>
using namespace std;


class Scheduler 
{
private:
	int timestep;

	//Scheduler Lists:
	Processor** ProcessorList;
	LinkedQueue<Process*> NewList;
	LinkedQueue<Process*> BLKList;
	LinkedQueue<Process*> TRMList;
	LinkedQueue<Pair<int, int>> KillList;

	//Counters Data Members:
	int NF;
	int NS;
	int NR;
	int ND;
	int ProcessorNUM;
	int NumP;
	int TRMcount;

	//Special Boundaries Data Members:
	int RRT;
	int RTF;
	int MaxW;
	int STL;
	int StopTime;
	int Overheat_prop;
	//Statistics Data Members:
	int Stl_Cntr;
	int ForkP;
	int Fork_Cntr;
	int RTF_Mig_Cntr;
	int MaxW_Mig_Cntr;
	int Kill_Cntr;
	int DeadLine_Cntr;

	//UI Pointer:
	UI* Program_UI;
public:
	//Constructor And Destructor:
	Scheduler();
	~Scheduler();

	//File Handling:
	void ReadFile(string FileName);
	void OutputFile(string FileName);
	void ProcessStatistics(int& avWT, int& avRT, int& avTRT, int& Tct);
	//Data Members Getters:
	Processor* Get_ShortestRDY(int b);
	Processor* Get_LongestRDY();
	void Inc_DeadLineCntr();

	//Moving Between The Lists:
	void MoveToTRM(Process* p);
	void NEWtoRDY();
	void RUNtoBLK(Process* p);
	void BLKtoRDY();

	//Stealing And Migration:
	void Stealing();
	double Calc_StealLimit(Processor* longest, Processor* shortest);
	bool MigrationRRtoSJF(Process* p);
	bool MigrationFCFStoRR(Process* p);
	void TurnON_Off_Processors();

	//Killing And Forking:
	void Killing();
	void Fork(Process* runP);
	bool killOrphan(Process* orphan);

	//Simulation
	void Simulation();
};


#endif
