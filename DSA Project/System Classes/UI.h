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
		short mode;
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col, 7); // sets Console color to Grey
		cout << "Please Choose Output Mode :\n";

		SetConsoleTextAttribute(col, 4); // sets Console color to Red 
		cout << "[0] -----> Step By Step\n";
		SetConsoleTextAttribute(col, 6); // sets Console color to Yellow 
		cout << "[1] -----> Interactive\n";
		SetConsoleTextAttribute(col, 2); // sets Console color to Green 
		cout << "[2] -----> Silent\n";

		SetConsoleTextAttribute(col, 1); // sets Console color to Blue 
		cin >> mode;
		return mode;
	}

	string Get_FileName(bool flag)
	{
		string name;
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col, 15); // sets Console color to White

		if (flag)
			cout << "Please Enter Input File Name :\n";
		else
			cout << "Please Enter Output File Name :\n";
		cin >> name;
		return name;
	}


	//Print Output For Step By Step And Interactive 
	//Works For Step By Step And Interactive Since They Are Almost Similar
	void printOutput(bool mode, int timestep, const LinkedQueue<Process*>& blk, const LinkedQueue<Process*>& trm, Processor* Parr[], int pCount)
	{
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(col, 1); // sets Console color to Blue
		cout << "Current Timestep:" << timestep << endl;

		SetConsoleTextAttribute(col, 6); // sets Console color to Yellow
		cout << "--------------     RDY PROCESSES     --------------" << endl;
		// loop on list of processors
		//call printRDY (virtual func in processors)

		for (int i = 0; i < pCount; i++)
		{
			cout << "processor " << i + 1;
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
				cout << Parr[i]->GetRunProcess() << *Parr[i] << " ";
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
			Sleep(10);
		SetConsoleTextAttribute(col, 7); // sets Console color to Grey
		cout << "------------------------------------------------------------------------------------------------------------------------" << endl << endl;
	}

	//Called By Passing(1) At Start Of The Simulation
	//Called By Passing(0) At End Of The Simulation
	void printSilent(bool start)
	{
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);

		if (start)
		{
			SetConsoleTextAttribute(col, 6); // sets Console color to Yellow
			cout << "Silent Mode......  Simulation Starts..." << endl;
		}
		else
		{
			SetConsoleTextAttribute(col, 10); // sets Console color to LightGreen
			cout << "Similation ends,Output file is created" << endl;

			SetConsoleTextAttribute(col, 7); // sets Console color to Grey

		}

	}
};
