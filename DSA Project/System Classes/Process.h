#pragma once
#ifndef _PROCESSES
#define _PROCESSES
#include<iostream>
#include"../DS Implementation/Queue/LinkedQueue.h"
#include"../DS Implementation/Pair/Pair.h"

using namespace std;
enum processState { NEW, READY, BLK, ORPH, TRM, RUNNING,Killed };
class Process
{
	//for Bonus In phase 2
	Process* leftChild;
	Process* rightChild;
	Process* Parent;
	int Deadline;

	int PID;
	int TS;
	int AT;
	int WT;
	int RT;
	int CT;
	int remainingCT;
	int startProcessing;
	int BLKduration;
	int IOduration;
	int TT;
	int IOCount;
	int killTime;
	int startFirst;
	processState State;
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
	int getDeadline();

	//Data Members Setters: 
	void setKillTime(int k);
	void setRemainingCT(int t);
	void setTT(int t);
	void updateState(processState s);
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
	void clear_IOList();

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
