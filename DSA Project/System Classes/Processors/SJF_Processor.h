#pragma once
#include "Processor.h"
#include"../../DS Implementation/Priority Queue/PriorityQueue.h"
class SJF_Processor : public Processor
{
private:
	PriorityQueue<Process*> RDY;
public:
	SJF_Processor(int Id);
	void ScheduleAlgo();
	void AddProcess(Process* p);
	bool Excuete();
	void printRDY();
	virtual void printInfo();
	bool isRDYempty();
	bool RDYtoRUN(int t);
};

