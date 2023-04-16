#pragma once
#include "Processor.h"
#include"../../DS Implementation/List/List.h"
class FCFS_Processor :public Processor
{
private:
	List<Process*> RDY;
public:
	FCFS_Processor();
	void AddProcess(Process* p);
	void ScheduleAlgo();
	bool Excuete();
	bool Kill(int ID);
	bool Fork(int ID);
	void printRDY();
	virtual void printInfo();
	virtual Process* KillRand(int ID);
	bool isRDYempty();
	bool RDYtoRUN();
};


