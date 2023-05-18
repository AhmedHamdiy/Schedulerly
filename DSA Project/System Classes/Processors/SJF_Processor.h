#pragma once
#include "Processor.h"
#include"../../DS Implementation/Priority Queue/PriorityQueue.h"
class SJF_Processor : public Processor
{
private:
	PriorityQueue<Process*> SJF_RDY;
public:

	//Constructor:
	SJF_Processor(int Id, Scheduler* sc, int OVT);

	//Processes Handling:
	void addProcess(Process* p);
	virtual Process* removeTop();
	virtual void turnOff(int timeStep);
	bool isRDYEmpty();
	virtual void scheduleAlgo(int timeStep);

	//Printing:
	void printRDY();
	virtual void printInfo();
};

