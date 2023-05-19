#pragma once
#include"Processor.h"
#include"../../DS Implementation/Queue/LinkedQueue.h"
class RR_Processor :
	public Processor
{
private:
	LinkedQueue<Process*> RR_RDY;
	int timeSlice;
public:

	//Constructor:

	RR_Processor(int Id, Scheduler* sc,int OVT, int TS);

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

