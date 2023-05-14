#pragma once
#include "Processor.h"
#include"../../DS Implementation/Priority Queue/PriorityQueue.h"
class SJF_Processor : public Processor
{
private:
	PriorityQueue<Process*> RDY;
public:

	//Constructor:
	SJF_Processor(int Id, Scheduler* sc, int OVT);

	//Processes Handling:
	void AddProcess(Process* p);
	virtual Process* remove_Top();
	virtual void OverHeat(Processor* Shortest, int TimeStep, int TStop);
	bool isRDYempty();
	virtual void ScheduleAlgo(int t);

	//Printing:
	void printRDY();
	virtual void printInfo();
};

