#pragma once
#include"Processor.h"
#include"../../DS Implementation/Queue/LinkedQueue.h"
class RR_Processor :
	public Processor
{
private:
	LinkedQueue<Process*> RDY;
	//static int timeSlice;
public:
	RR_Processor();
	static void setTimeSlice(int t);
	void AddProcess(Process* p);
	void ScheduleAlgo();
	bool Excuete();
	void printRDY();
	virtual void printInfo();
	bool isRDYempty();
	bool RDYtoRUN();
};

