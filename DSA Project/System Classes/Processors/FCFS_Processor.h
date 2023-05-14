#pragma once
#include "Processor.h"
#include<windows.h> 
#include"../../DS Implementation/List/PList.h"
class FCFS_Processor :public Processor
{
private:
	PList RDY;
	int Forking_Probability;
public:
	//Constructor:
	FCFS_Processor(int Id, Scheduler* sc, int OVT,int ForkingP);

	//Processes Handling:
	void AddProcess(Process* p);
	virtual Process* remove_Top();
	virtual void OverHeat(Processor* Shortest, int TimeStep, int TStop) ;
	bool isRDYempty();
	virtual void ScheduleAlgo(int t);

	//Printing:
	void printRDY();
	virtual void printInfo();

	//FCFS Special Functions:
	bool ForkProcess(Process*& runProcess);
	bool KillProcess(int ID,Process* &target);
};


