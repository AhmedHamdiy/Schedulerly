#include"Scheduler.h"
#include"Processors/FCFS_Processor.h"
#include"Processors/SJF_Processor.h"
#include"Processors/RR_Processor.h"
#include"UI.h"


Scheduler::Scheduler()
{
	TRMcount = 0;
	ProcessorCounter = 0;
	MinTime = nullptr;
	MaxTime = nullptr;
}

void Scheduler::ReadFile()


{
	Pair<int, int> IO_Piar;
	int Arrival_Time, PID, CPU_Time, IO_List;
	char ch, m;
	ifstream inFile;
	inFile.open("Input Sample.txt");
	if (!inFile)
	{
		cout << "Error"; return;
	}
	int x, n, i = 0;
	if (inFile >> x) NF = x;
	if (inFile >> x) NS = x;
	if (inFile >> x) NR = x;
	ProcessorList = new Processor * [NF + NS + NR];

	for (; i < NF; i++)
	{
		Processor* pro = new FCFS_Processor;
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS; i++)
	{
		Processor* pro = new SJF_Processor;
		ProcessorList[i] = pro;
	}
	for (; i < NF + NS + NR; i++)
	{
		Processor* pro = new RR_Processor;
		ProcessorList[i] = pro;
	}
	if (inFile >> x) RRT = x;
	if (inFile >> x) RTF = x;
	if (inFile >> x) MaxW = x;
	if (inFile >> x) STL = x;
	if (inFile >> x) ForkP = x;
	if (inFile >> x) NumP = x;
	for (int i = 0; i < NumP; i++)
	{
		if (inFile >> x) Arrival_Time = x;
		if (inFile >> x) PID = x;
		if (inFile >> x) CPU_Time = x;
		if (inFile >> x) IO_List = x;
		Process* p = new Process(Arrival_Time, PID, CPU_Time, IO_List);
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
	setTS();

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

void Scheduler::MoveToTRM(Process* p)
{
	if (p) {
		TRMList.enqueue(p);
		p->updateState(TRM);
		TRMcount++;
		Process* leftChild = p->get_LChild();
		Process* rightChild = p->get_RChild();

		if (leftChild)
		{
			killOrphan(leftChild); 
			//killOrphan calls killProcess=>search for process and remove from run or ready

			MoveToTRM(leftChild);
		}

		if (rightChild)
		{
			killOrphan(rightChild);
			MoveToTRM(rightChild);
		}
	}
	//kill orphans(remove from ready or run of fcfs before moving to trm)
}

int Scheduler::GetNumP()
{
	return NumP;
}

int Scheduler::GetTRMcount()
{
	return TRMcount;
}

void Scheduler::setTS()
{
	for (int i = NF + NS; i < NR + NF + NS; i++)
	{
		RR_Processor* pr = nullptr;
		pr = dynamic_cast<RR_Processor*>(ProcessorList[i]);
		pr->setTimeSlice(RRT);
	}
}


//Moving Process from NEW to RDY at AT
void Scheduler::NEWtoRDY(int t)
{
	while (!(NewList.isEmpty()) && t == NewList.peekFront()->getAT())
	{
		Process* p;
		NewList.dequeue(p);
		getshortestRDY(0)->AddProcess(p);
		//ProcessorList[ProcessorCounter]->AddProcess(p); //add to rdy
		p->updateState(READY);
		//ProcessorCounter = (ProcessorCounter + 1) % (NF + NS + NR);
	}
}

void Scheduler::RUNtoBLK(Process* p)
{
	p->updateState(BLK);
	BLKList.enqueue(p);
}

void Scheduler::IOreq(int t)
{
	Pair<int, int> temp;
	for (int i = 0; i < NF + NS + NR; i++)
	{
		if (ProcessorList[i]->GetRunProcess() && ProcessorList[i]->GetRunProcess()->GetIO(temp))
		{
			if (temp.first == ProcessorList[i]->GetRunProcess()->getCT()- ProcessorList[i]->GetRunProcess()->getRemainingCT())
			{
				RUNtoBLK(ProcessorList[i]->GetRunProcess());
				ProcessorList[i]->setRUN(nullptr);
			}
		}
	}
}

void Scheduler::updateRemainingCT()
{
	for (int i = 0; i < NF + NS + NR; i++)
	{
		if (ProcessorList[i]->GetRunProcess())
		{
			ProcessorList[i]->GetRunProcess()->decrementCT();
		}
	}
}

Processor* Scheduler::getshortestRDY(bool b)
{
	Processor* shortest = ProcessorList[0];
	if (b)  //looking for shortest RDY in FCFS Processors only
	{
		for (int i = 0; i < NF; i++)
		{
			if (ProcessorList[i]->getBusytime() < shortest->getBusytime())
			{
				shortest = ProcessorList[i];
			}
		}
	}
	else //looking for shortest RDY in All Processors
	{
		for (int i = 0; i < NF + NS + NR; i++)
		{
			if (ProcessorList[i]->getBusytime() < shortest->getBusytime())
			{
				shortest = ProcessorList[i];
			}
		}
	}
	return shortest;
}


void Scheduler::BLKtoRDY()
{
	Process* p = nullptr;
	Pair<int,int> temp;
	if (!BLKList.isEmpty())
	{
		p = BLKList.peekFront();
		if (p->GetIO(temp))
		{
			if (temp.second == p->getblktime())
			{
				Processor* shortest = getshortestRDY(0);
				p->updateState(READY);
				getshortestRDY(0)->AddProcess(p);
				p->deqIO();
				BLKList.dequeue(p);
				p->setblktime(0);
			}
			else
			{
				p->setblktime(p->getblktime() + 1);
			}
		}
		
	}
}

void Scheduler::RUNtoRDY(Process* p)
{
	ProcessorList[ProcessorCounter]->AddProcess(p);
	p->updateState(READY);
	ProcessorCounter = (ProcessorCounter + 1) % (NF + NS + NR);
}

void Scheduler::GetMinMax()
{
	for (int i = 0; i < NF + NR + NS; i++)
	{
		if (ProcessorList[i] < MinTime)
		{
			MinTime = ProcessorList[i];
		}
		if (ProcessorList[i] > MaxTime)
		{
			MaxTime = ProcessorList[i];
		}
	}
}

bool Scheduler::MigrationRRtoSJF(Process* p)
{
	if (p->getCT() < RTF && NS != 0)
	{
		ProcessorList[NF]->AddProcess(p);
		return true;
	}
	return false;
}

int Scheduler::getMaxW()
{
	return MaxW;
}

void Scheduler::MigrationFCFStoRR(Process* p)
{

}

void Scheduler::Killing(int timestep)
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
					MoveToTRM(targetProcess);
					break;
				}

			} //if process not found ignore kill signal
		}

	}
}

void Scheduler::simulation()
{
	UI* userInterface = new UI;
	int mode = userInterface->chooseMode();
	if (mode == 2)
		userInterface->printSilent(1);

	int timeStep = 0;
	ReadFile(); //read input from file
	while (true)
	{
		//if AT of peekfront=timestep,dequeue and move
		NEWtoRDY(timeStep); //(i)

		//loop on processors
		//move process from EACH rdy list to run
		for (int i = 0; i < NF + NS + NR; i++) //(ii)
		{
			ProcessorList[i]->RDYtoRUN(timeStep);
		}
		int probability; //(iii)
		Process* tempRUN = nullptr;

		updateRemainingCT();
		Process* TempProcess;
		for (int i = 0; i < NF + NS + NR; i++)
		{
			if (ProcessorList[i]->FinishRUN())
			{
				MoveToTRM(ProcessorList[i]->GetRunProcess());
				ProcessorList[i]->setRUN(nullptr);
			}
		}

		IOreq(timeStep);
		BLKtoRDY();


		for (int i = 0; i < NF + NS + NR; i++) //for each process in run state
		{
			tempRUN = ProcessorList[i]->GetRunProcess();

			probability = 1 + (rand() % 100); //generate random number

			if (!ProcessorList[i]->isIdle())//there is a RUN process
			{
				//1. Testing to BLK ( for request I/O resources in phase2)
				/*if (probability >= 1 && probability <= 15)
				{
					ProcessorList[i]->setRUN(nullptr); //remove from run
					RUNtoBLK(tempRUN);
				}*/


				//2. Testing From RUN to RDY (for RR Processor in phase 2)
				/* if (probability >= 20 && probability <= 30)
				{
					ProcessorList[i]->setRUN(nullptr);
					RUNtoRDY(tempRUN);
				}*/
				//3. Testing to TRM (the process has ended in phase2) 
				/*else if (probability >= 50 && probability <= 60)
				{
					ProcessorList[i]->setRUN(nullptr);
					MoveToTRM(tempRUN);
				}*/


			}
		}
		Killing(timeStep);
		Fork(timeStep);
		//v.Testing Killing (for FCFS in phase 2)
		//Killing();

		//iv.Testing BlK to RDY (the process has ended I/O resources in phase 2)
		//probability = 1 + (rand() % 100);
		/*probability = 5;
		if (probability < 10)
			BLKtoRDY();*/




		/////////------------PHASE 2-------------------////////

		//for (int i = 0; i < ProcessorCounter; i++)
		//{   for each run process
			//increment runtime 
			//if runtime=CT move to trm
		//}
		//while (!BLKList.isEmpty())
		//{
		//}
		//increament IO time 
		//if IOtime=IO duration move to rdy

		
		if(mode!=2)
		userInterface->printOutput(mode, timeStep, BLKList, TRMList, ProcessorList, NF + NS + NR);

		if (NumP == TRMcount) //break loop condition
			break;

		timeStep++;
	}

	if (mode == 2)
	{
		Sleep(100);
			userInterface->printSilent(0);
	}
}

void Scheduler::Fork(int timestep)
{
	Process* runProcess = nullptr;
	Process* forkedProcess = nullptr;
	for (int i = 0; i < NF; i++) //loop on FCFS processors only
	{
		FCFS_Processor* FPro = dynamic_cast<FCFS_Processor*>(ProcessorList[i]);
		if (FPro)
		{
			//check if rand within probability and RUN process can fork
			if (FPro->ForkProcess(runProcess, ForkP))
			{
				NumP++;
				forkedProcess = new Process(timestep, NumP, runProcess->getRemainingCT(), 0);
				runProcess->setForked(forkedProcess);
				//create a process forkedProcess

				//add to shortest FCFS
				Processor* shortest_FCFS = getshortestRDY(1);
				shortest_FCFS->AddProcess(forkedProcess);
				forkedProcess->updateState(READY);
				
			}
		}
	}
}

void Scheduler::killOrphan(Process* orphan)
{
	bool isDone = 0;
	int target_id = orphan->GetID();
	for (int i = 0; i < NF; i++)
	{
		FCFS_Processor* FPro = dynamic_cast<FCFS_Processor*>(ProcessorList[i]);
		if (FPro)
			//search for process&remove it from rdy/run if exists
			isDone = FPro->KillProcess(target_id,orphan);
		if (isDone)
			return;
	}
}


