#pragma once
#include "Processor.h"
#include<windows.h> 
#include"../../DS Implementation/List/PList.h"
class FCFS_Processor :public Processor
{
private:
	PList RDY;
public:

	//Constructor:
	FCFS_Processor(int Id, Scheduler* sc);

	//Processes Handling:
	void AddProcess(Process* p);
	virtual Process* remove_Top();
	virtual int OverHeat(Processor* Shortest, int TimeStep, int TStop) ;
	bool isRDYempty();
	virtual void ScheduleAlgo(int t);

	//Printing:
	void printRDY();
	virtual void printInfo();

	//FCFS Special Functions:
	void Inc_WT();
	bool ForkProcess(Process*& runProcess, int forkP);
	bool KillProcess(int ID,Process* &target);
};


