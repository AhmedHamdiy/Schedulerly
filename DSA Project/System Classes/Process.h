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
	int PID;
	int CT;
	int TT;
	int IOcount;
	int KillTime;
	LinkedQueue<Pair<int,int>> IOList;
	state State;
	bool isUpdated;
public:
	Process();
	Process(int a, int id, int c, int n);
	~Process();
	void AddIO(Pair<int, int> p);
	void SetKillTime(int k);
	int GetID();
	void incrementTT();
	int getTRT();
	int getAT();
	int getCT();
	void setCT(int t);
	int getWT();
	bool updateState(state s);
	void setPID(int id);
	void Forking(Process* &firstChild, Process* &secondChild);
	//overloading << operator to Print The ID
	friend ostream& operator << (ostream& out, Process *p);
	//overloading < operator to compare  CT of two processes to sort them in priority queue
	bool operator<(const Process* other) const;
};


#endif
