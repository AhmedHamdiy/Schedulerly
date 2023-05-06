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
	//for Bonus Forking In phase 2
	Process* LChild;
	Process* RChild;
	Process* Parent;
	
	int PID;
	int TS;
	int AT;
	int WT;
	int RT;
	int CT;
	int RemainingCT;
	int StartC;
	int BLKduration;
	int TT;
	int IOcount;
	int KillTime;
	int startfirst;
	state State;
	LinkedQueue<Pair<int, int>> IOList;
public:
	//Constructor:
	Process(int at, int id, int ct, int n,Process* P=nullptr);
	
	//Data Members Getters: 
	int getID();
	int getTRT();
	int getAT();
	int getCT();
	int getRemainingCT();
	int getRT();
	int getTT();
	int getWT();
	int getTS();

	//Data Members Setters: 
	void SetKillTime(int k);
	void setRemainingCT(int t);
	void setTT(int t);
	void updateState(state s);
	void setstart(int t);
	//I/O Handling:
	void AddIO(Pair<int, int> p);
	bool GetIO(Pair<int, int>& temp);
	int getstart();
	void inc_blktime();
	int getblktime();
	void resetblktime();
	void deqIO();

	//Time Handling:
	void incrementTT();
	void decrementCT();
	void updateWT();
	bool IncrementTS(int TS);

	
	//Forking
	Process*& get_LChild();
	Process*& get_RChild();
	Process* getParent(); 
	bool setForked(Process* forkedP); 
	bool canFork() const;

	//Printing:
	friend ostream& operator << (ostream& out, Process* p);
};
#endif
