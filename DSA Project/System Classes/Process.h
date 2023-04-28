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
	int AT;
	int WT;
	int PID;
	int CT;
	int StartC;
	int BLKduration;
	int TT;
	int IOcount;
	int KillTime;
	LinkedQueue<Pair<int,int>> IOList;
	state State;
public:
	Process();
	Process(int at, int id, int ct, int n);
	~Process();
	void AddIO(Pair<int, int> p);
	void SetKillTime(int k);
	int GetID();
	void incrementTT();
	int getTRT();
	int getAT();
	int getCT();
	int getID();
	void setCT(int t);
	int getWT();

	bool GetIO(Pair<int, int>& temp);
	void setstart(int t);
	int getstart();
	void setblktime(int t);
	int getblktime();
	void deqIO();

	void updateState(state s);
	void updateWT();
	void setPID(int id);
	void Forking(Process* &firstChild, Process* &secondChild);
	//overloading << operator to Print The ID
	friend ostream& operator << (ostream& out, Process *p);
	Process*& get_LChild();
	Process*& get_RChild();

};

#endif
