#pragma once
#include "Processor.h"
#include<windows.h> 
#include"../../DS Implementation/List/PList.h"
class FCFS_Processor :public Processor
{
private:
	PList FCFS_RDY;
	int forkingProbability;

public:
	//Constructor:

	FCFS_Processor(int Id, Scheduler* sc, int OVT,int forkingP);

	//Processes Handling:
	
	void addProcess(Process* p);
	virtual Process* removeTop();
	virtual void turnOff(int timeStep) ;
	bool isRDYEmpty();
	virtual void scheduleAlgo(int timeStep);

	//Printing:
	
	void printRDY();
	virtual void printInfo();

	//FCFS Special Functions:
	
	bool forkProcess(Process*& runProcess);
	bool killProcess(int ID,Process* &target);
};


