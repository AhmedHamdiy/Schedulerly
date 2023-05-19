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
	PriorityQueue<Process*> NewList;
	LinkedQueue<Process*> BLKList;
	LinkedQueue<Process*> TRMList;
	LinkedQueue<Pair<int, int>> KillList;
	
	//Counters Data Members:
	int NF;
	int NS;
	int NR;
	int ND;
	int processorCount;
	int processCount;
	int terminatedCount;

	//Special Boundaries Data Members:
	int RRT;
	int RTF;
	int MaxW;
	int STL;
	int stopTime;
	
	//Statistics Data Members:
	int stealCounter;
	int forkingProbability;
	int forkingCounter;
	int RTFCounter;
	int MaxWCounter;
	int killCounter;
	int deadlineCounter;
	
	//UI Pointer:
	UI* programUI;

public:
	//Constructor And Destructor:

	Scheduler();
	~Scheduler();

	//File Handling:

	void readFile(string FileName);
	void outputFile(string FileName);
	void processStatistics(float& avWT, float& avRT, float& avTRT, int& Tct);
	
	//Data Members Getters:

	Processor* getShortestRDY(int b);
	Processor* getLongestRDY();
	void increaseDeadLineCounter();
	
	//Moving Between The Lists:

	void moveToTRM(Process* p);
	void moveFromNew();
	void moveToBLK(Process* p);
	void moveFromBLK();
	void moveToNew(Process* p);

	//Steal And Migration:

	void Steal();
	bool stealCondition(Processor* longest, Processor* shortest);
	bool migrateRRtoSJF(Process* p);
	bool migrateFCFStoRR(Process* p);
	
	//Killing And Forking:

	void Kill();
	void Fork(Process* runP);
	bool killOrphan(Process* orphan);
	
	//Simulation:

	void Simulate();
	void Schedule();
};


#endif
