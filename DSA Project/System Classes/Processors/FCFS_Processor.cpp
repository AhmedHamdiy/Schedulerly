#include "FCFS_Processor.h"
#include"../Scheduler.h"

FCFS_Processor::FCFS_Processor(int Id, Scheduler* sc) :Processor(Id, sc)
{}



					//---------------------------------------( Scheduling )------------------------------------------------//
	
void FCFS_Processor::AddProcess(Process* p)
{
	RDY.insertEnd(p);
	Inc_Finishtime(p->getRemainingCT());
}

Process* FCFS_Processor::remove_Top()
{
	Process* p = nullptr;
	int i = 1;
	if (!RDY.isEmpty() && i <= RDY.getcount())
	{
		p = RDY.getEntry(i);
		if (p->getParent() == nullptr)
		{
			RDY.remove(i);
			Dec_Finishtime(p->getRemainingCT());
			return p;
		}
		else 
			i++;
	}
	return p;
}

int FCFS_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
{
	int Travelled_Proces(0);
	int randNum = rand() % 100;
	if (TimeStep - StopTime >= TStop)
	{
		if (getState() == BUSY)
			UpdateState(BUSY);
		if (getState() == IDLE)
			UpdateState(IDLE);
	}
	else
		if (randNum < 25)
		{
			if (!isIdle())
			{
				Process* Rn = GetRunProcess();
				Travelled_Proces++;
				Shortest->AddProcess(Rn);
				setRUN(nullptr);
			}
			if (!isRDYempty())
			{
				for (int i = 1; i <= RDY.getcount(); i++)
				{
					Process* p = RDY.getEntry(i);
					RDY.remove(i);
					Shortest->AddProcess(p);
					Dec_Finishtime(p->getRemainingCT());
					Travelled_Proces++;
				}
			}
			UpdateState(STOP);
			StopTime = TimeStep;
		}
	return Travelled_Proces;
}


bool FCFS_Processor::isRDYempty()
{
	return RDY.getcount() == 0;
}

void FCFS_Processor::ScheduleAlgo(int t)
{
	Inc_WT();
	if (FinishRUN()) //The Run Prcocess has Finished
	{
		MYSch->MoveToTRM(GetRunProcess());
		setRUN(nullptr);
	}

	Process* runP = GetRunProcess();
	bool v = ForkProcess(runP, MYSch->getForkP());
	if(v)
	MYSch->Fork(runP);

	
	if (!isIdle()) {
		Inc_BusyTime();
		Dec_RUNCT();
		IO_Req();

	}
	else
	{

		if (isRDYempty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess = RDY.getEntry(1);
		RDY.remove(1);

		//The Migration part:
		bool migrated = MYSch->MigrationFCFStoRR(RDYprocess);
		while (migrated && RDY.getcount() != 0)
		{
			RDY.remove(1);
			Dec_Finishtime(RDYprocess->getRemainingCT());
			migrated = MYSch->MigrationFCFStoRR(RDYprocess);
		}
		if (migrated && RDY.getcount() == 0)
		{
			setRUN(nullptr);
			return;
		}
		else if (!migrated && isIdle())
		{
			//Just set The curent Process as A running process
			Dec_Finishtime(RDYprocess->getRemainingCT());
			RDYprocess->setstart(t);
			setRUN(RDYprocess);
			RDYprocess->updateState(RUNNING);
		}
	}

}

					//---------------------------------------( FCFS Special )------------------------------------------------//

bool FCFS_Processor::ForkProcess(Process*& runProcess, int forkP)
{
	if (isIdle())
		return false;
	srand(time(NULL));
	int random;
	random = (rand() % 100);
	runProcess = nullptr;
	if (random < forkP)
	{
		runProcess = GetRunProcess();
		if (runProcess->canFork())
		{
			return true;
		}
		else
		{
			runProcess = nullptr;
			return false;
		}
	}
	else
		return false;
}

bool FCFS_Processor::KillProcess(int ID, Process*& target)
{
	//search for process,remove it from ready/run if exists then return a pointer to it
	if (GetRunProcess() && GetRunProcess()->getID() == ID)
	{
		target = GetRunProcess();
		setRUN(nullptr);
		return true;
	}
	int pos = -1;
	Process* KProcess = RDY.search_by_ID(ID, pos);
	if (KProcess)
	{
		target = KProcess;
		RDY.remove(pos);
		return true;
	}
	return false;
}

void FCFS_Processor::Inc_WT()
{
	for (int i = 1; i <= RDY.getcount(); i++)
	{
		Process* p = RDY.getEntry(i);
		//p->updateWT();
	}
}

				//-----------------------------------------( Printing )------------------------------------------------//

void FCFS_Processor::printRDY()
{
	RDY.Print();
}

void FCFS_Processor::printInfo()
{
	cout << " [FCFS]: " << RDY.getcount() << " ";
}
