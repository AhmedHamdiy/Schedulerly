#include "FCFS_Processor.h"
#include"../Scheduler.h"

FCFS_Processor::FCFS_Processor(int Id, Scheduler* sc, int OVT,int ForkingP) :Processor(Id, sc,OVT), Forking_Probability(ForkingP)
{}


					//---------------------------------------( Scheduling )--------------------------------------------------//
	
void FCFS_Processor::AddProcess(Process* p)
{
	p->updateState(READY);
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

void FCFS_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
{
	if (!StopTime) //The Processor Isn't OverHeated 
	{
		if (!isIdle())
		{
			// Moving The Run Process To Shortest RDY Queue
			Process* Rn = GetRunProcess();
			Shortest->AddProcess(Rn);
			setRUN(nullptr);
		}
		if (!isRDYempty())
		{
			// Moving The RDY Processes To Shortest RDY Queue
			for (int i = 1; i <= RDY.getcount(); i++)
			{
				Process* p = RDY.getEntry(1);
				RDY.remove(1);
				Shortest->AddProcess(p);
				Dec_Finishtime(p->getRemainingCT());
			}
		}
		StopTime = TimeStep;
		UpdateState(STOP);
	}
	else if (get_remainingOverHeat(TimeStep)<=0)
	{
		if (getState() == BUSY)
			UpdateState(BUSY);
		else
			UpdateState(IDLE);
		StopTime = 0;
	}
}

bool FCFS_Processor::isRDYempty()
{
	return RDY.getcount() == 0;
}

void FCFS_Processor::ScheduleAlgo(int t)
{
	if (FinishRUN()) //The Run Prcocess has Finished
	{
		MYSch->MoveToTRM(GetRunProcess());
		setRUN(nullptr);
	}

	
	
	if (!isIdle()) {
		Inc_BusyTime();
		Dec_RUNCT();
		IO_Req();
		Process* runP = GetRunProcess();
		bool v = ForkProcess(runP);
		if (v)
			MYSch->Fork(runP);
	}
	else
	{

		if (isRDYempty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess = RDY.getEntry(1);
		RDY.remove(1);
		Dec_Finishtime(RDYprocess->getRemainingCT());

		//The Migration part:
		bool migrated = MYSch->MigrationFCFStoRR(RDYprocess);
		while (migrated && RDY.getcount() != 0)
		{
			RDYprocess = RDY.getEntry(1);
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

bool FCFS_Processor::ForkProcess(Process*& runProcess)
{
	if (isIdle())
		return false;
	srand(time(NULL));
	int random;
	random = (rand() % 100);
	runProcess = nullptr;
	if (random < Forking_Probability)
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


					//-------------------------------------------( Printing )------------------------------------------------//

void FCFS_Processor::printRDY()
{
	RDY.Print();
}

void FCFS_Processor::printInfo()
{
	cout << " [FCFS]: " << RDY.getcount() << " ";
}
