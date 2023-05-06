#include"Scheduler.h"
#include"Processors/FCFS_Processor.h"
#include"Processors/SJF_Processor.h"
#include"Processors/RR_Processor.h"

#include"UI.h"
using namespace std;


				//--------------------------------( Constructor & Destructor )-------------------------------------//

Scheduler::Scheduler()
{
	DeadLine_Cntr = 0;
	TRMcount = 0;
	timestep = 0;
	MaxW_Mig_Cntr = 0;
	RTF_Mig_Cntr = 0;
	Fork_Cntr = 0;
	Stl_Cntr = 0;
	Kill_Cntr = 0;
	OverHeat_Processes = 0;
}
Scheduler::~Scheduler()
{
	//Deallocate TRMList
	TRMList.clear();
	//Deallocate Each Processor In ProcessorLiSt
	for (int i = 0; i < ProcessorNUM; i++)
		delete ProcessorList[i];
	//Deallocate The ProcessorList
	delete[] ProcessorList;
}


				//----------------------------------( Input & Output Files )-------------------------------------------//

void Scheduler::ReadFile(string FileName)
{
	Pair<int, int> IO_Piar;
	char ch, m;
	ifstream inFile;
	inFile.open(FileName+".txt");
	while (!inFile)
	{
		cout << "Error, Please Enter an existing File Name\n"; 
	}
	int x, n, i = 0;
	if (inFile >> x) NF = x;
	if (inFile >> x) NS = x;
	if (inFile >> x) NR = x;
	if (inFile >> x) ND = x;
	if (inFile >> x) RRT = x;
	ProcessorNUM = NF + NS + NR + ND;
	//Allocating The ProcessorList With Unique IDs:
	ProcessorList = new Processor * [ProcessorNUM];
	for (; i < NF; i++)
	{
		Processor* pro = new FCFS_Processor(i + 1, this);
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS; i++)
	{
		Processor* pro = new SJF_Processor(i + 1,this);
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS + NR; i++)
	{
		Processor* pro = new RR_Processor(i + 1, this, RRT);
		ProcessorList[i] = pro;
	}

	for (; i < ProcessorNUM; i++)
	{
		Processor* pro = new EDF_Processor(i + 1, this);
		ProcessorList[i] = pro;
	}

	//Getting The Percentages:
	if (inFile >> x) RTF = x;
	if (inFile >> x) MaxW = x;
	if (inFile >> x) STL = x;
	if (inFile >> x) ForkP = x;
	if (inFile >> x) StopTime = x;

	//Allocating The Processes With:
	if (inFile >> x) NumP = x;
	int Arrival_Time, PID, CPU_Time,DeadLine, IO_List;

	for (int i = 0; i < NumP; i++)
	{
		if (inFile >> x) Arrival_Time = x;
		if (inFile >> x) PID = x;
		if (inFile >> x) CPU_Time = x;
		if (inFile >> x) DeadLine = x;
		if (inFile >> x) IO_List = x;
		Process* p = new Process(Arrival_Time, PID, CPU_Time,DeadLine, IO_List);
		p->updateState(NEW);
		for (int j = 0; j < x; j++)
		{
			if (j > 0) if (inFile >> ch) m = ch;
			if (inFile >> ch) m = ch;
			if (inFile >> n)
				IO_Piar.first = n;
			if (inFile >> ch) m = ch;
			if (inFile >> n)
				IO_Piar.second = n;
			if (inFile >> ch) m = ch;
			p->AddIO(IO_Piar);
		}
		NewList.enqueue(p);
	}

	//Allocating KILLSIG:
	while (inFile >> x)
	{
		if (inFile >> n)
		{
			Pair<int, int > kill(x, n);
			KillList.enqueue(kill);
		}
	}

	inFile.close();
}

void Scheduler::OutputFile(string FileName)
{
	ofstream OutFile(FileName + ".txt");
	if (!OutFile.is_open())
		return;
	else
	{
		LinkedQueue<Process*> auxilary;
		Process* p;
		//Printing The Statistics For Each Process:
		OutFile << "TT PID AT CT IO_D WT RT TRT" << endl;
		for (int i = 0; i < NumP; i++)
		{
			TRMList.dequeue(p);
			auxilary.enqueue(p);
			OutFile << p->getTT() << "  " << p->getID() << "  " << p->getAT() << "  " << p->getCT() << "  ";
			OutFile << p->getIOduration() << "  " << p->getWT(timestep) << "  " << p->getRT() << "  " << p->getTRT() << endl;
		}
		for (int i = 0; i < NumP; i++)
		{
			auxilary.dequeue(p);
			TRMList.enqueue(p);
		}

		//Printing The Statistics For All Processes:
		int avWT, avRT, avTRT,tct(0);
		ProcessStatistics(avWT, avRT, avTRT,tct);
		OutFile << "Processes: " << NumP << endl;
		OutFile << "Avg WT = " << avWT << ",   Avg RT = " << avRT << ",   Avg TRT = " << avTRT << endl;
		OutFile << "Migration %:      RTF =" << 100 * float(RTF_Mig_Cntr)/NumP << "% ,      MaxW = " << 100 * float(MaxW_Mig_Cntr) / NumP << "%"<< endl;
		OutFile << "Work Steal %: " << 100 * float(Stl_Cntr) / NumP << "%" << endl;
		OutFile << "Forked Process %: " << 100 * float(Fork_Cntr)/NumP << "%" << endl;
		OutFile << "Killed Process %: " << 100 * float(Kill_Cntr) /NumP << "%" << endl;
		OutFile << "OverHeated Process %: " << 100 * float(OverHeat_Processes) / NumP << "%" << endl;
		OutFile << "Temination Before DeadLine %: " << 100 * float(DeadLine_Cntr) / NumP << "%" << endl;

		int totalTRT = avTRT * NumP;
		double u, totUti(0);
		OutFile << "Processors: " << ProcessorNUM << " [" << NF << " FCFS, " << NS << " SJF, " << NR << " RR," << ND << " EDF]" << endl;

		OutFile << "Processors Load" << endl;
		for (int i = 0; i < ProcessorNUM; i++)
		{
			OutFile << "p" << i+1 << "=" << ProcessorList[i]->processorLoad(totalTRT) << "%";
			if (i != ProcessorNUM - 1)
				OutFile << ",  ";
		}
		OutFile << endl << "Processor Utiliz" << endl;
		for (int i = 0; i < ProcessorNUM; i++)
		{
			u = ProcessorList[i]->processorUtilization(timestep);
			OutFile << "p" << i+1 << "=" << u << "%";
			if (i != ProcessorNUM)
				OutFile << ",  ";
			totUti += u;
		}
		OutFile << endl << "Avg utilization = " << totUti / (ProcessorNUM) << "%";
		OutFile.close();
	}

}

void Scheduler::ProcessStatistics(int& avWT, int& avRT, int& avTRT,int& TCT)
{
	LinkedQueue<Process*> auxilary;
	Process* item;
	int totalWT = 0, totalRT = 0, totalTRT = 0;
	for (int i = 0; i < NumP; i++)
	{
		if (!TRMList.isEmpty())
		{
			TRMList.dequeue(item);
			totalWT += item->getWT(timestep);
			totalRT += item->getRT();
			totalTRT += item->getTRT();
			TCT += item->getCT();
			auxilary.enqueue(item);
		}
	}
	TRMList = auxilary;
	avWT = totalWT / (float)NumP;
	avRT = totalRT / (float)NumP;
	avTRT = totalTRT / (float)NumP;
}


			//-----------------------------------------( Data Members Getters )------------------------------------------------//

Processor* Scheduler::Get_LongestRDY()
{
	Processor* Longest = ProcessorList[0];
	for (int i = 1; i < NF + NR + NS; i++)
	{
		if (ProcessorList[i]->get_Finishtime() > Longest->get_Finishtime())
		{
			Longest = ProcessorList[i];
		}
	}
	return Longest;
}

void Scheduler::Inc_DeadLineCntr()
{
	DeadLine_Cntr++;
}

Processor* Scheduler::Get_ShortestRDY(int Processors_Type)
{
	Processor* shortest = nullptr;
	if (Processors_Type == 1)  //looking for shortest RDY in FCFS Processors only
	{
		shortest = ProcessorList[0];
		for (int i = 0; i < NF; i++)
		{
			if (ProcessorList[i]->get_Finishtime() < shortest->get_Finishtime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 2)  //looking for shortest RDY in SJF Processors only
	{
		shortest = ProcessorList[NF];
		for (int i = NF; i < NF + NS; i++)
		{
			if (ProcessorList[i]->get_Finishtime() < shortest->get_Finishtime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 3)   //looking for shortest RDY in RR Processors
	{
		shortest = ProcessorList[NF + NS];
		for (int i = NF + NS; i < ProcessorNUM; i++)
		{
			if (ProcessorList[i]->get_Finishtime() < shortest->get_Finishtime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 0) //looking for shortest RDY in All Processors
	{
		shortest = ProcessorList[0];
		for (int i = 0; i < ProcessorNUM; i++)
		{
			if (ProcessorList[i]->get_Finishtime() < shortest->get_Finishtime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	return shortest;
}


			//-----------------------------------------( Moving Between Lists )------------------------------------------------//

void Scheduler::MoveToTRM(Process* p)
{
	if (p) {
		TRMList.enqueue(p);
		p->updateState(TRM);
		p->setTT(timestep);
		TRMcount++;
		Process* leftChild = p->get_LChild();
		Process* rightChild = p->get_RChild();
		bool orphanKilled;

		if (leftChild)
		{
			orphanKilled = killOrphan(leftChild);
			//killOrphan calls killProcess=>search for process and remove from run or ready
			if (orphanKilled)
			{
				MoveToTRM(leftChild);
				leftChild->updateState(Killed);
				Kill_Cntr++;
			}
			//otherwise the orphan has already terminated before its Parent
		}

		if (rightChild)
		{
			orphanKilled = killOrphan(rightChild);
			if (orphanKilled)
			{
				MoveToTRM(rightChild);
				rightChild->updateState(Killed);
				Kill_Cntr++;
			}
		}
	}
	//kill orphans(remove from ready or run of fcfs before moving to trm)
}

void Scheduler::NEWtoRDY()
{
	//Moving Process from NEW to RDY at AT
	while (!(NewList.isEmpty()) && timestep == NewList.peekFront()->getAT())
	{
		Process* p;
		NewList.dequeue(p);
		Get_ShortestRDY(0)->AddProcess(p);
		p->updateState(READY);
	}
}

void Scheduler::RUNtoBLK(Process* p)
{
	p->updateState(BLK);
	BLKList.enqueue(p);
}


void Scheduler::BLKtoRDY()
{
	Process* p = nullptr;
	Process* p2 = nullptr;
	Pair<int, int> temp;
	if (!BLKList.isEmpty())
	{
		p = BLKList.peekFront();
		if (p->GetIO(temp))
		{
			if (temp.second == p->getblktime())
			{
				p->updateState(READY);
				Get_ShortestRDY(0)->AddProcess(p);
				p->deqIO();
				p->resetblktime();
				BLKList.dequeue(p);
				if (!BLKList.isEmpty())
				{
					p2 = BLKList.peekFront();
					p2->inc_blktime();
				}
			}
			else
			{
				p->inc_blktime();
			}
		}
	}
}


			//-----------------------------------------( Migration & Stealing )------------------------------------------------//


bool Scheduler::MigrationRRtoSJF(Process* p)
{
	if (p != nullptr && p->getRemainingCT() < RTF && NS != 0 && p->getParent() == nullptr)
	{
		Get_ShortestRDY(2)->AddProcess(p);
		p->updateState(READY);
		RTF_Mig_Cntr++;
		return true;
	}
	return false;
}

bool Scheduler::MigrationFCFStoRR(Process* p)
{
	if (p != nullptr && p->getWT(timestep) > MaxW && NR != 0 && p->getParent() == nullptr)
	{
		Get_ShortestRDY(3)->AddProcess(p);
		p->updateState(READY);
		MaxW_Mig_Cntr++;
		return true;
	}
	return false;
}

void Scheduler::TurnON_Off_Processors()
{
	for (int i = 0; i < NF; i++)
	{
		//If The Processor is FCFS Then Make Another FCFS Processor Steal It
		Processor* Shortest = Get_ShortestRDY(1);
		OverHeat_Processes+=ProcessorList[i]->OverHeat(Shortest, timestep, StopTime);
	}
	for (int i = NF; i < ProcessorNUM; i++)
	{
		Processor* Shortest = Get_ShortestRDY(0);
		OverHeat_Processes+=ProcessorList[i]->OverHeat(Shortest, timestep, StopTime);
	}
}

void Scheduler::Stealing()
{
	Processor* shortest = Get_ShortestRDY(0);
	Processor* longest = Get_LongestRDY();
	/*FCFS_Processor* FPro = dynamic_cast<FCFS_Processor*>(longest);
	//If The Processor is FCFS Then Make Another FCFS Processor Steal It
	if (FPro)
		shortest = Get_ShortestRDY(1);
	else
		shortest = Get_ShortestRDY(0);*/

	while (Calc_StealLimit(longest, shortest) > 40 && (!(longest->isRDYempty()))) // Check that the Steal limit is less than 40 
	{
		Process* stolen = longest->remove_Top();
		if(stolen)
		{
			shortest->AddProcess(stolen); //add the top process to the shortest 
			Stl_Cntr++;
		}
	}
}

double Scheduler::Calc_StealLimit(Processor* longest, Processor* shortest)
{
	if (longest->get_Finishtime() != 0)
		return 100 * ((longest->get_Finishtime() - shortest->get_Finishtime()) / double(longest->get_Finishtime()));
	return 100;
}


						//-----------------------------------( Killing & Forking )----------------------------------------//

void Scheduler::Killing()
{
	if (KillList.isEmpty())
		return;
	else
	{
		Pair<int, int> killPair = KillList.peekFront();
		if (killPair.first != timestep)
			return;
		else //SIGKILL is now
		{
			KillList.dequeue(killPair);
			int target_id = killPair.second;
			Process* targetProcess = nullptr;
			bool isDone = 0;
			for (int i = 0; i < NF; i++)
			{
				FCFS_Processor* FPro = dynamic_cast<FCFS_Processor*>(ProcessorList[i]);
				if (FPro)
					//search for process&remove it from rdy/run if exists
					isDone = FPro->KillProcess(target_id, targetProcess);
				if (isDone)
				{
					Kill_Cntr++;
					MoveToTRM(targetProcess);
					targetProcess->updateState(Killed);
					break;
				}
			} //if process not found ignore kill signal
		}

	}
}
int Scheduler::getForkP() const
{
	return ForkP;
}
void Scheduler::Fork(Process* runP)
{
	NumP++;
	Fork_Cntr++;
	Process* forkedProcess = new Process(timestep, NumP, runP->getRemainingCT(), 0,0, runP);
	runP->setForked(forkedProcess);
	//create a process forkedProcess
	//add to shortest FCFS
	Processor* shortest_FCFS = Get_ShortestRDY(1);
	shortest_FCFS->AddProcess(forkedProcess);
	forkedProcess->updateState(READY);
}

bool Scheduler::killOrphan(Process* orphan)
{
	bool isDone = 0;
	int target_id = orphan->getID();
	for (int i = 0; i < NF; i++)
	{
		FCFS_Processor* FPro = dynamic_cast<FCFS_Processor*>(ProcessorList[i]);
		if (FPro)
			//search for process&remove it from rdy/run if exists
			isDone = FPro->KillProcess(target_id, orphan);
		if (isDone)
			return true;
	}
	return false;
}


						//-----------------------------------( Simulation )----------------------------------------//

void Scheduler::Simulation()
{
	string FileName;
	UI* userInterface = new UI;
	
	//Get The Input File Name:
	FileName = userInterface->Get_FileName(1);

	//Read Input From File
	ReadFile(FileName);
	
	//Get The Screen Output Mode:
	int mode = userInterface->chooseMode();	
	
	//Get The Output File Name:
	FileName = userInterface->Get_FileName(0);

	//If The Mode Is Silent: 
	if (mode == 2)
		userInterface->printSilent(1);

	while (true)
	{
		//If AT Of Peekfront = Timestep , Dequeue And Move
		NEWtoRDY();
		//Loop On Processors And Call The Function ScheduleAlgo
		for (int i = 0; i < ProcessorNUM; i++)
			ProcessorList[i]->ScheduleAlgo(timestep);	
		Killing();
		BLKtoRDY();		
		 //Processor OverHeat:
		//TurnON_Off_Processors();
		/*
		if (timestep % STL == 0)
			Stealing();
		*/
		if (mode != 2) 
			userInterface->printOutput(mode, timestep, BLKList, TRMList, ProcessorList, ProcessorNUM);
		if (NumP == TRMcount)
		{ 
			//Create The OutPut File:
			OutputFile(FileName);
			if (mode == 2) {userInterface->printSilent(0);}
			break;
		}//Break Loop Condition
		timestep++;
	}

}
