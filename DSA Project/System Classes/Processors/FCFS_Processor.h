#pragma once
#include "Processor.h"
#include<windows.h> 
#include"../../DS Implementation/List/PList.h"
class FCFS_Processor :public Processor
{
private:
	PList RDY;
public:
	FCFS_Processor(int Id);
	void AddProcess(Process* p);
	void ScheduleAlgo();
	bool Excuete();
	
	
	void printRDY();
	virtual void printInfo();
	
	bool isRDYempty();
	bool RDYtoRUN(){};
	bool ForkProcess(Process*& runProcess, int forkP);
	bool KillProcess(int ID,Process* &target);
	bool RDYtoRUN(int t);
};


