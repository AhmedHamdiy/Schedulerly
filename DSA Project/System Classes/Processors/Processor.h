#pragma once
#include<iostream>
#include"../Process.h"
#include<windows.h>
class Scheduler;

using namespace std;
enum processorState { BUSY , IDLE , STOP };
class Processor
{
protected:
	Scheduler* schedulerPtr;
	int stopTime;
	int overHeatTime;
	int overHeatProbability;

private:
	int ID;
	Process* RUN;
	int busyTime;
	int finishTime;
	processorState State;

public:
	//Constructor:
	Processor(int Id, Scheduler* sc, int OVT);
	
	//Data Members Getters:
	Process* getRunProcess();
	int getFinishTime();
	processorState getState();
	bool isIdle();
	virtual bool isRDYEmpty() = 0;
	int getID();

	//Processor Statistics:
	int getBusytime();
	double processorLoad(int totalTRT);
	double processorUtilization(int timeStep);
	void updateState(processorState st);

	//Processes Handling:
	bool setRun(Process* p);
	virtual void addProcess(Process* p) = 0;
	virtual void turnOff(int TimeStep) = 0;
	void TurnOn(int timeStep);
	virtual Process* removeTop() = 0;
	virtual void scheduleAlgo(int t) = 0;
	bool isRunFinished();
	void requestIO();
	void overHeatHandling(int timeStep);

	//Time Handling:
	void increaseFinishTime(int Time);
	void decreaseFinishTime(int Time);
	void increaseBusyTime();
	void decreaseRunRemainingCT();
	int getHealingSteps(int timeStep);

	//Printing:
	virtual void printRDY() = 0;
	virtual void printInfo() = 0;
	friend ostream& operator<<(std::ostream& os, const Processor& p);
};
