#pragma once
#include<iostream>
#include"../Process.h"
//#include"Scheduler.h"
using namespace std;
class Processor
{
protected:
	static int nextID; // Static data member to keep track of next available ID
	int ID;
private:
	Process* RUN;
	int busyTime;

public:

	Processor(); //Constructor
	virtual void ScheduleAlgo() = 0; //For PHASE2 deteremine how to decide the RUN process
	void setRUN(Process* p);
	int getID();
	bool isIdle(); // returns true if there isn't RUN process
	Process* GetRunProcess();
	virtual void AddProcess(Process* p) = 0;
	void setBusytime(int T);
	int getBusytime();
	virtual bool Excuete() = 0;
	virtual void printRDY() = 0;
	virtual void printInfo() = 0;
	virtual bool isRDYempty() = 0; // returns true if there isn't RUN process

	// Overload the comparsion operators by busytime to determine the shortest and longest queue
	bool operator < (Processor* p1);
	bool operator >(Processor* p1);
	// Overload the << operator as a friend function
	friend ostream& operator<<(std::ostream& os, const Processor& p);
	
	virtual bool RDYtoRUN() = 0;//FOR PHASE 1 ONLY(alternative for scheduelalgo.)//
	virtual Process* KillRand(int ID);

};
