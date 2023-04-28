#pragma once

#include"Process.h"
#include"Processors/Processor.h"
#include<windows.h> 
#include<iostream>
#include"../DS Implementation/Queue/LinkedQueue.h"
using namespace std;
class UI
{
public:
	int chooseMode()
	{
		char c;
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col, 7); // sets Console color 
		cout << "Please Choose Output Mode ( Enter 's' For Silent,'i' For Interactive,'b' For Stepbystep )" << endl;
		SetConsoleTextAttribute(col, 1); // sets Console color 
		cin >> c;

		if (c == 'b') //stepbystep
			return 0;
		else if (c == 'i') //interactive
			return 1;
		else if (c == 's') //silent
			return 2;
		return -1; //invalid

	}
	//print output for step by step and interactive 
	void printOutput(bool mode, int timestep, LinkedQueue<Process*>& blk, LinkedQueue<Process*>& trm,Processor* Parr[],int pCount)
		//works for stepbystep and interactive since they are almost similar
	{
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col,1); // sets Console color 
		cout << "Current Timestep:" << timestep << endl;
		SetConsoleTextAttribute(col, 6); // sets Console color to Yellow
		cout << "--------------     RDY PROCESSES     --------------" << endl;
		// loop on list of processors
		//call printRDY (virtual func in processors)

		for (int i = 0; i < pCount; i++)
		{
			cout << "processor " << i+1  ;
			Parr[i]->printInfo();
			cout << "RDY: ";
			Parr[i]->printRDY();
		}
		SetConsoleTextAttribute(col, 4); // sets Console color ro Red
		cout << "--------------     BLK PROCESSES     --------------" << endl;
		cout << blk.getcount() << " BLK: ";
		if (blk.getcount() == 0)
			cout << endl;
		else 
			blk.print();

		SetConsoleTextAttribute(col, 2); // sets Console color to Green
		cout << "--------------     RUN PROCESSES     --------------" << endl;
		//loop on list of processors
		//print running process
		int count = 0;
		for (int i = 0; i < pCount; i++)
			if (!Parr[i]->isIdle())
				count++;
		cout << count << " RUN: ";
		for (int i = 0; i < pCount; i++)
		{
			if (!Parr[i]->isIdle())
			{
				cout << Parr[i]->GetRunProcess() << *Parr[i]<< " ";
			}
		}

		cout << endl;
		SetConsoleTextAttribute(col, 3); // sets Console color to Cyan
		cout << "--------------     TRM PROCESSES     --------------" << endl;
		cout << trm.getcount() << " TRM: ";
		trm.print();

		if (mode)
		{ 
			//interactive
			SetConsoleTextAttribute(col, 15); // sets Console color to White
			cout << "\nPRESS ANY KEY TO MOVE TO THE NEXT STEP!" << endl;
			cin.get();
		}
		else
			Sleep(1000);
		SetConsoleTextAttribute(col, 7); // sets Console color to Grey
		cout << "------------------------------------------------------------------------------------------------------------------------"<<endl<< endl;
	}
	//called by passing(1) at start of the simulation
	//called by passing(0) at end of the simulation
	void printSilent(bool start)
	{
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		
		if (start)
		{
			SetConsoleTextAttribute(col, 6); // sets Console color 
			cout << "Silent Mode......  Simulation Starts..." << endl;
		}
		else
		{
			SetConsoleTextAttribute(col, 10); // sets Console color 
			cout << "Similation ends,Output file is created" << endl;
			SetConsoleTextAttribute(col, 7); // sets Console color 

		}

	}
};
