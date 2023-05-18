#include"Scheduler.h"
#include"Processors/FCFS_Processor.h"
#include"Processors/SJF_Processor.h"
#include"Processors/RR_Processor.h"
#include"UI.h"
#include<iomanip>
using namespace std;


				//--------------------------------( Constructor & Destructor )-------------------------------------//

Scheduler::Scheduler()
{
	ND = 0;
	NF = 0;
	NS = 0;
	NR = 0;
	MaxW = 0;
	processCount = 0;
	processorCount = 0;
	ProcessorList=nullptr;
	RRT = 0;
	RTF = 0;
	STL = 1;
	stopTime = 0;
	forkingProbability = 0;
	programUI = new UI;
	deadlineCounter = 0;
	terminatedCount = 0;
	timestep = 0;
	MaxWCounter = 0;
	RTFCounter = 0;
	forkingCounter = 0;
	stealCounter = 0;
	killCounter = 0;
}

Scheduler::~Scheduler()
{
	//Deallocate Processes
	TRMList.clear();
	KillList.clear();

	//Deallocate Processors:

	//First :Deallocate Each Processor In ProcessorLiSt
	for (int i = 0; i < processorCount; i++)
		delete ProcessorList[i];
	//Second: Deallocate The ProcessorList
	delete[] ProcessorList;
	
	//Deallocate programUI:
	delete programUI;
}


				//----------------------------------( Input & Output Files )-------------------------------------------//

void Scheduler::readFile(string FileName)
{
	Pair<int, int> IO_Piar;
	char ch, m;
	ifstream inFile;
	inFile.open(FileName+".txt");
	if (!inFile)
	{
		programUI->printErrorMessage(1);
		exit(0);
	}
	int x, n, i = 0;
	if (inFile >> x) NF = x;
	if (inFile >> x) NS = x;
	if (inFile >> x) NR = x;
	if (inFile >> x) ND = x;
	if (inFile >> x) RRT = x;

	//Getting The Percentages:
	if (inFile >> x) RTF = x;
	if (inFile >> x) MaxW = x;
	if (inFile >> x) STL = x;
	if (inFile >> x) forkingProbability = x;
	if (inFile >> x) stopTime = x;
	processorCount = NF + NS + NR + ND;
	//Allocating The ProcessorList With Unique IDs:
	ProcessorList = new Processor * [processorCount];
	for (; i < NF; i++)
	{
		Processor* pro = new FCFS_Processor(i + 1, this, stopTime,forkingProbability);
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS; i++)
	{
		Processor* pro = new SJF_Processor(i + 1,this, stopTime);
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS + NR; i++)
	{
		Processor* pro = new RR_Processor(i + 1, this, stopTime, RRT);
		ProcessorList[i] = pro;
	}

	for (; i < processorCount; i++)
	{
		Processor* pro = new EDF_Processor(i + 1, this, stopTime);
		ProcessorList[i] = pro;
	}
	//Allocating The Processes With:
	if (inFile >> x) processCount = x;
	int Arrival_Time(0), PID(0), CPU_Time(0), DeadLine(0), IO_List(0);

	for (int i = 0; i < processCount; i++)
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
		NewList.enqueue(p,Arrival_Time);
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

void Scheduler::outputFile(string FileName)
{
	ofstream OutFile(FileName + ".txt");
	if (!OutFile.is_open())
		return;
	else
	{
		LinkedQueue<Process*> auxilary;
		Process* p;
		//Printing The Statistics For Each Process:
		OutFile << "TT" << setw(8) << "PID"<< setw(7)<< "AT"<< setw(7)<< "CT"<<setw(8)<< "IO_D "<< setw(7)<< "WT"<< setw(7)<< "RT"<< setw(7)<< "TRT" << endl;
		for (int i = 0; i < processCount; i++)
		{
			TRMList.dequeue(p);
			auxilary.enqueue(p);
			OutFile << p->getTT() << setw(7) << p->getID() << setw(7) << p->getAT() << setw(7) << p->getCT() << setw(7);
			OutFile << p->getIOduration() << setw(9) << p->getWT(timestep) << setw(7) << p->getRT() << setw(7) << p->getTRT() << endl;
		}
		for (int i = 0; i < processCount; i++)
		{
			auxilary.dequeue(p);
			TRMList.enqueue(p);
		}

		//Printing The Statistics For All Processes:
		float avWT, avRT, avTRT;
		int tct(0);
		processStatistics(avWT, avRT, avTRT,tct);
		OutFile << "Processes: " << processCount << endl;
		OutFile << "Avg WT = " << avWT << ",   Avg RT = " << avRT << ",   Avg TRT = " << avTRT << endl;
		OutFile << "Migration %:      RTF =" << 100 * float(RTFCounter)/processCount << "% ,      MaxW = " << 100 * float(MaxWCounter) / processCount << "%"<< endl;
		OutFile << "Work Steal %: " << 100 * float(stealCounter) / processCount << "%" << endl;
		OutFile << "Forked Process %: " << 100 * float(forkingCounter)/processCount << "%" << endl;
		OutFile << "Killed Process %: " << 100 * float(killCounter) /processCount << "%" << endl;
		OutFile << "Temination Before DeadLine %: " << 100 * float(deadlineCounter) / processCount << "%" << endl;

		int totalTRT = int(avTRT) * processCount;
		double u, totUti(0);
		OutFile << "Processors: " << processorCount << " [" << NF << " FCFS, " << NS << " SJF, " << NR << " RR, " << ND << " EDF]" << endl;

		OutFile << "Processors Load" << endl;
		for (int i = 0; i < processorCount; i++)
		{
			OutFile << "p" << i+1 << "=" << ProcessorList[i]->processorLoad(totalTRT) << "%";
			if (i != processorCount - 1)
				OutFile << ",  ";
		}
		OutFile << endl << "Processor Utiliz" << endl;
		for (int i = 0; i < processorCount; i++)
		{
			u = ProcessorList[i]->processorUtilization(timestep);
			OutFile << "p" << i+1 << "=" << u << "%";
			if (i != processorCount)
				OutFile << ",  ";
			totUti += u;
		}
		OutFile << endl << "Avg utilization = " << totUti / (processorCount) << "%";
		OutFile.close();
	}

}

void Scheduler::processStatistics(float& avWT, float& avRT, float& avTRT,int& TCT)
{
	LinkedQueue<Process*> auxilary;
	Process* item;
	int totalWT = 0, totalRT = 0, totalTRT = 0;
	for (int i = 0; i < processCount; i++)
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
	avWT = totalWT / (float)processCount;
	avRT = totalRT / (float)processCount;
	avTRT = totalTRT / (float)processCount;
}


			//-----------------------------------------( Data Members Getters )------------------------------------------------//

Processor* Scheduler::getLongestRDY()
{
	Processor* Longest = ProcessorList[0];
	for (int i = 1; i < processorCount; i++)
	{
		if (ProcessorList[i]->getFinishTime() > Longest->getFinishTime())
		{
			Longest = ProcessorList[i];
		}
	}
	return Longest;
}

void Scheduler::increaseDeadLineCounter()
{
	deadlineCounter++;
}

Processor* Scheduler::getShortestRDY(int Processors_Type)
{
	Processor* shortest = nullptr;
	if (Processors_Type == 1)  //looking for shortest RDY in FCFS Processors only
	{
		shortest = ProcessorList[0];
		for (int i = 1; i < NF; i++)
		{
			if (ProcessorList[i]->getFinishTime() < shortest->getFinishTime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 2)  //looking for shortest RDY in SJF Processors only
	{
		shortest = ProcessorList[NF];
		for (int i = NF+1; i < NF + NS; i++)
		{
			if (ProcessorList[i]->getFinishTime() < shortest->getFinishTime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 3)   //looking for shortest RDY in RR Processors
	{
		shortest = ProcessorList[NF + NS];
		for (int i = NF + NS + 1; i < NF + NS + NR; i++)
		{
			if (ProcessorList[i]->getFinishTime() < shortest->getFinishTime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 4)   //looking for shortest RDY in EDF Processors
	{
		shortest = ProcessorList[NF + NS + NR];
		for (int i = NF + NS + NR + 1; i < processorCount; i++)
		{
			if (ProcessorList[i]->getFinishTime() < shortest->getFinishTime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else if (Processors_Type == 0) //looking for shortest RDY in All Processors
	{
		shortest = ProcessorList[0];
		for (int i = 1; i < processorCount; i++)
		{
			if (ProcessorList[i]->getFinishTime() < shortest->getFinishTime() && ProcessorList[i]->getState() != STOP)
			{
				shortest = ProcessorList[i];
			}
		}
	}
	if (shortest->getState() != STOP)
		return shortest;
	else
		return nullptr;
}


			//-----------------------------------------( Moving Between Lists )------------------------------------------------//

void Scheduler::moveToTRM(Process* p)
{
	if (p) {
		TRMList.enqueue(p);
		p->updateState(TRM);
		p->clear_IOList();
		p->setTT(timestep);
		terminatedCount++;
		Process* leftChild = p->get_LChild();
		Process* rightChild = p->get_RChild();
		bool orphanKilled;

		if (leftChild)
		{
			orphanKilled = killOrphan(leftChild);
			//killOrphan calls killProcess=>search for process and remove from run or ready
			if (orphanKilled)
			{
				moveToTRM(leftChild);
				leftChild->updateState(Killed);
				killCounter++;
			}
			//otherwise the orphan has already terminated before its Parent
		}

		if (rightChild)
		{
			orphanKilled = killOrphan(rightChild);
			if (orphanKilled)
			{
				moveToTRM(rightChild);
				rightChild->updateState(Killed);
				killCounter++;
			}
		}
	}
	//kill orphans(remove from ready or run of fcfs before moving to trm)
}

void Scheduler::moveFromNew()
{
	//Moving Process from NEW to RDY at AT
	while (!(NewList.isEmpty()) && timestep >= NewList.Peek()->getAT())
	{
		Process* p= NewList.Peek();
		Processor* Shortest = getShortestRDY(0);
		if (Shortest)
		{
			NewList.dequeue();
			Shortest->addProcess(p);
			p->updateState(READY);
		}
		else
			break;
	}
}

void Scheduler::moveToBLK(Process* p)
{
	p->updateState(BLK);
	BLKList.enqueue(p);
}

void Scheduler::moveFromBLK()
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
				moveToNew(p);
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

void Scheduler::moveToNew(Process* p)
{
	NewList.enqueue(p, p->getAT());
}


			//-----------------------------------------( Migration & Stealing )------------------------------------------------//


bool Scheduler::migrationRRtoSJF(Process* p)
{
	if (p != nullptr && p->getRemainingCT() < RTF && NS != 0 && p->getParent() == nullptr)
	{
		Processor* Shortest= getShortestRDY(2);
		if (Shortest)
		{
			Shortest->addProcess(p);
			RTFCounter++;
			return true;
		}
	}
	return false;
}

bool Scheduler::migrationFCFStoRR(Process* p)
{
	if (p != nullptr && p->getWT(timestep) > MaxW && NR != 0 && p->getParent() == nullptr)
	{
		Processor* Shortest = getShortestRDY(3);
		if (Shortest)
		{
			Shortest->addProcess(p);
			MaxWCounter++;
			return true;
		}
	}
	return false;
}

void Scheduler::Stealing()
{
	Processor* shortest = getShortestRDY(0);
	Processor* longest = getLongestRDY();
	if (longest->isRDYEmpty())return;
	bool Steal_Condition = calculateStealLimit(longest, shortest) > 40;
	while (Steal_Condition && (!(longest->isRDYEmpty()))) // Check that the Steal limit is less than 40 
	{
		Process* stolen = longest->removeTop();
		if (stolen)
		{
			shortest->addProcess(stolen); //add the top process to the shortest 
			stealCounter++;
			Steal_Condition = calculateStealLimit(longest, shortest) > 40;
		}
		else
			return;
	}
}

double Scheduler::calculateStealLimit(Processor* longest, Processor* shortest)
{
	if (longest->getFinishTime() != 0)
		return 100 * ((longest->getFinishTime() - shortest->getFinishTime()) / double(longest->getFinishTime()));
	return 0;
}


						//-----------------------------------( Killing & Forking )----------------------------------------//

void Scheduler::Killing()
{
	while (1)
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
						isDone = FPro->killProcess(target_id, targetProcess);
					if (isDone)
					{
						killCounter++;
						moveToTRM(targetProcess);
						targetProcess->updateState(Killed);
						break;
					}
				} //if process not found ignore kill signal
			}
		}
	}
}

void Scheduler::Fork(Process* runP)
{
	processCount++;
	forkingCounter++;
	Process* forkedProcess = new Process(timestep, processCount, runP->getRemainingCT(), 0, 0, runP);
	runP->setForked(forkedProcess);
	//create a process forkedProcess
	//add to shortest FCFS
	Processor* shortest_FCFS = getShortestRDY(1);
	if (shortest_FCFS)
	{
		shortest_FCFS->addProcess(forkedProcess);
		forkedProcess->updateState(READY);
	}
	else
		moveToTRM(forkedProcess);
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
			isDone = FPro->killProcess(target_id, orphan);
		if (isDone)
			return true;
	}
	return false;
}

	
					//-----------------------------------( Simulation )----------------------------------------//

void Scheduler::Scheduling()
{
	//Loop On Processors And Call The Function ScheduleAlgo
	for (int i = 0; i < processorCount; i++)
		ProcessorList[i]->scheduleAlgo(timestep);
}

void Scheduler::Simulation()
{
	string FileName;

	//Get The Input File Name:
	FileName = programUI->getFileName(1);
	
	//Read Input From File
	readFile(FileName);

	//Get The Screen Output Mode:
	int mode = programUI->chooseMode();	
	
	//Get The Output File Name:
	FileName = programUI->getFileName(0);
	
	//If The Mode Is Silent: 
	if (mode == 2)
		programUI->printSilent(1);

	while (true)
	{
		//If AT Of Peekfront = Timestep , Dequeue And Move
		moveFromNew();
		//If BlkDuration Of Peekfront =IO Duration Request , Dequeue And Move
		moveFromBLK();
		//If KillSIG Of Peekfront =Timestep , Dequeue And Move
		Killing();	
		//Loop And Call SchedulingAlgo
		Scheduling();
		//Stealing:
		if (timestep % STL == 0)
			Stealing();

		if (mode != 2)
			programUI->printOutput(mode, timestep, BLKList, TRMList, ProcessorList, processorCount);
		if (processCount == terminatedCount)
		{
			//Create The OutPut File:
			outputFile(FileName);
			if (mode == 2)
			{
				programUI->printSilent(0);
			}
			break;
		}//Break Loop Condition
		timestep++;
	}
}


