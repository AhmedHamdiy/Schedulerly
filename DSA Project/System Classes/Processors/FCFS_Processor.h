#pragma once
#include "Processor.h"
#include<windows.h> 
#include"../../DS Implementation/List/PList.h"
class FCFS_Processor :public Processor
{
private:
	PList RDY;
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
<<<<<<< HEAD
	bool ForkProcess(Process*& runProcess, int forkP);
	bool KillProcess(int ID,Process* &target);
=======
>>>>>>> parent of 83618ff (.)
	bool RDYtoRUN(int t);
};


