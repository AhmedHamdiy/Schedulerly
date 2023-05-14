#pragma once
#include<iostream>
#include"../Process.h"
#include<windows.h>
class Scheduler;

using namespace std;
enum PState { BUSY , IDLE , STOP };
class Processor
{
protected:
	Scheduler* MYSch;
	int StopTime;
	int OverHeat_time;

private:
	int ID;
	Process* RUN;
	int BusyTime;
	int FinishTime;
	PState state;
public:
	//Constructor:
	Processor(int Id, Scheduler* sc, int OVT);
	
	//Data Members Getters:
	Process* GetRunProcess();
	int get_Finishtime();
	PState getState();
	bool isIdle();
	virtual bool isRDYempty() = 0;
	int get_ID();

	//Processor Statistics:
	int getBusytime();
	double processorLoad(int totalTRT);
	double processorUtilization(int timeStep);
	
	//Data Members Setters:
	bool setRUN(Process* p);
	void UpdateState(PState st);

	//Processes Handling:
	virtual void AddProcess(Process* p) = 0;
	virtual void OverHeat(Processor* Shortest, int TimeStep, int TStop) = 0;
	virtual Process* remove_Top() = 0;
	virtual void ScheduleAlgo(int t) = 0;
	bool FinishRUN();
	void IO_Req();

	//Time Handling:
	void Inc_Finishtime(int T);
	void Dec_Finishtime(int T);
	void Inc_BusyTime();
	void Dec_RUNCT();
	int get_remainingOverHeat(int TimeStep);

	//Printing:
	virtual void printRDY() = 0;
	virtual void printInfo() = 0;
	friend ostream& operator<<(std::ostream& os, const Processor& p);
};
