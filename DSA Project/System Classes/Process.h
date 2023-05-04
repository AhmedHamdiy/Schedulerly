#pragma once
#ifndef _PROCESSES
#define _PROCESSES
#include<iostream>
#include"../DS Implementation/Queue/LinkedQueue.h"
#include"../DS Implementation/Pair/Pair.h"

using namespace std;
enum state { NEW, READY, BLK, ORPH, TRM,RUNNING };
class Process
{
	//for Bonus Forking In phase 2
	Process* LChild;
	Process* RChild;
	Process* Parent;
	
	int AT;
	int WT;
	int RT;
	int PID;
	int CT;
	int RemainingCT;
	int StartC;
	int BLKduration;
	int TT;
	int IOcount;
	int KillTime;
	LinkedQueue<Pair<int,int>> IOList;
	state State;
public:
	int startfirst;
	Process();
	Process(int at, int id, int ct, int n,Process* P=nullptr);
	~Process();
	void AddIO(Pair<int, int> p);
	void SetKillTime(int k);
	void incrementTT();
	void decrementCT();
	bool isFinished();
	int getTRT();
	int getAT();
	int getCT();
	int getRemainingCT();
	int getRT();
	int getTT();
	
	int getID();
	void setCT(int t);
	void setRemainingCT(int t);
	void setTT(int t);
	int getWT();

	//I/O
	bool GetIO(Pair<int, int>& temp);
	void setstart(int t);
	int getstart();
	void inc_blktime();
	int getblktime();
	void resetblktime();
	void deqIO();

	void updateState(state s); //? do we use it
	void updateWT(); //increments waiting time
	void setPID(int id); //can we set it in constructor w nsheel dy?

	//overloading << operator to Print The ID
	friend ostream& operator << (ostream& out, Process *p);

	//forking
	Process*& get_LChild();
	Process*& get_RChild();
	Process* getParent(); 
	bool setForked(Process* forkedP); //assign left or right child
	bool canFork() const; //has empty slot for a child?

	//void setParent(Process* p); //set in constructor 

};
#endif
