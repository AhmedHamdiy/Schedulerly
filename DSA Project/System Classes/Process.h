#pragma once
#ifndef _PROCESSES
#define _PROCESSES
#include<iostream>
#include"../DS Implementation/Queue/LinkedQueue.h"
#include"../DS Implementation/Pair/Pair.h"

using namespace std;
enum state { NEW, READY, BLK, ORPH, TRM, RUNNING,Killed };
class Process
{
	//for Bonus In phase 2
	Process* LChild;
	Process* RChild;
	Process* Parent;
	int DeadLine;

	int PID;
	int TS;
	int AT;
	int WT;
	int RT;
	int CT;
	int RemainingCT;
	int StartC;
	int BLKduration;
	int IOduration;
	int TT;
	int IOcount;
	int KillTime;
	int startfirst;
	state State;
	LinkedQueue<Pair<int, int>> IOList;
public:
	//Constructor:
	Process(int at, int id, int ct, int dead_line,int n, Process* P = nullptr);
	
	//Data Members Getters: 
	int getID();
	int getTRT();
	int getAT();
	int getCT();
	int getRemainingCT();
	int getRT();
	int getTT();
	int getWT(int timeStep);
	int getTS();
	int get_DeadLine();

	//Data Members Setters: 
	void SetKillTime(int k);
	void setRemainingCT(int t);
	void setTT(int t);
	void updateState(state s);
	void setstart(int t);

	//I_O Handling:
	void AddIO(Pair<int, int> p);
	bool GetIO(Pair<int, int>& temp);
	int getstart();
	void inc_blktime();
	int getblktime();
	void resetblktime();
	void deqIO();
	void Inc_IOduration(int t);
	int getIOduration();

	//Time Handling:
	void incrementTT();
	void decrementCT();
	bool IncrementTS(int TS);
	void reset_TS();

	
	//Forking
	Process*& get_LChild();
	Process*& get_RChild();
	Process*& getParent(); 
	bool setForked(Process* forkedP); 
	bool canFork() const;

	//Printing:
	friend ostream& operator << (ostream& out, Process* p);
};
#endif
