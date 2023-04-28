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
	RR_Processor();
	void setTimeSlice(int t);
	void AddProcess(Process* p);
	void ScheduleAlgo();
	bool Excuete();
	void printRDY();
	virtual void printInfo();
	bool isRDYempty();
	bool RDYtoRUN(int t);
};

