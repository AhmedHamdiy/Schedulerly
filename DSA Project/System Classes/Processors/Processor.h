#pragma once
#include<iostream>
#include"../Process.h"

class Scheduler;

using namespace std;
enum PState { BUSY, IDLE };
class Processor
{
protected:
	int ID;
	
private:
	Process* RUN;
	int BusyTime;
	int FinishTime;
	PState state;
public:
	Processor(); //Constructor
	virtual void ScheduleAlgo() = 0; //For PHASE2 deteremine how to decide the RUN process
	bool setRUN(Process* p);
	int getID();
	void FlipState();
	bool isIdle(); // returns true if there isn't RUN process
	Process* GetRunProcess();
	virtual void AddProcess(Process* p) = 0;
	int getBusytime();
	void Inc_Finishtime(int T);
	void Dec_Finishtime(int T);
	void Inc_BusyTime();
	bool FinishRUN();
	void Dec_RUNCT();
	int get_Finishtime();
	virtual bool Excuete() = 0;
	virtual Process* remove_Top()=0;
	virtual void printRDY() = 0;
	virtual void printInfo() = 0;
	virtual bool isRDYempty() = 0; // returns true if there isn't RUN process
	// Overload the << operator as a friend function:
	friend ostream& operator<<(std::ostream& os, const Processor& p);
	virtual bool RDYtoRUN(int t,Scheduler* scptr) = 0;//FOR PHASE 1 ONLY(alternative for scheduelalgo.)//
	double processorLoad(int totalTRT);
	double processorUtilization( int timeStep);
};
