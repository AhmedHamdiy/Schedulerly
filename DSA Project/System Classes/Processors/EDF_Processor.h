#pragma once
#include "Processor.h"
#include"../../DS Implementation/Priority Queue/PriorityQueue.h"
class EDF_Processor : public Processor
{
private:
	PriorityQueue<Process*> EDF_RDY;

public:
	//Constructor:
	EDF_Processor(int Id, Scheduler* sc, int OVT);

	//Processes Handling:
	void addProcess(Process* p);
	virtual Process* removeTop();
	virtual void turnOff(int timeStep);
	bool isRDYEmpty();
	virtual void scheduleAlgo(int t);

	//Printing:
	void printRDY();
	virtual void printInfo();
};

