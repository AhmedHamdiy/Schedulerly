#pragma once
#include"Processor.h"
#include"../../DS Implementation/Queue/LinkedQueue.h"
class RR_Processor :
	public Processor
{
private:
	LinkedQueue<Process*> RDY;
	int timeSlice;
public:

	//Constructor:
	RR_Processor(int Id, Scheduler* sc, int TS);

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

