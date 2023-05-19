#include "FCFS_Processor.h"
#include"../Scheduler.h"

FCFS_Processor::FCFS_Processor(int Id, Scheduler* sc, int OVT, int ForkingP) :Processor(Id, sc, OVT), forkingProbability(ForkingP)
{}


		//---------------------------------------( Scheduling )--------------------------------------------------//

//Add process to rdy queue
void FCFS_Processor::addProcess(Process* p)
{
	p->updateState(READY);
	FCFS_RDY.insertEnd(p);
	increaseFinishTime(p->getRemainingCT());
}

//Remove the peek unforked process of rdy queue
Process* FCFS_Processor::removeTop()
{
	Process* p = nullptr;
	int i = 1;
	if (!FCFS_RDY.isEmpty() && i <= FCFS_RDY.getcount())
	{
		p = FCFS_RDY.getEntry(i);
		if (p->getParent() == nullptr)
		{
			FCFS_RDY.remove(i);
			decreaseFinishTime(p->getRemainingCT());
			return p;
		}
		else
			i++;
	}
	return nullptr;
}

//Overheat the processor for overheat duration and move all the processes in it to the shortest rdy queue
void FCFS_Processor::turnOff(int timeStep)
{
	srand(time(nullptr));
	bool Probability_Cond = (rand() % 1000 < overHeatProbability);
	if (Probability_Cond && !stopTime) //The Processor Isn't OverHeated 
	{
		if (!isIdle())
		{
			// Moving The Run Process To Shortest RDY Queue
			Process* Rn = getRunProcess();
			if (Rn->getParent() != nullptr)
				schedulerPtr->moveToTRM(Rn);
			else
				schedulerPtr->moveToNew(Rn);
			setRun(nullptr);
		}
		if (!isRDYEmpty())
		{
			// Moving The RDY Processes To Shortest RDY Queue
			for (int i = 1; i <= FCFS_RDY.getcount(); i++)
			{
				Process* p = FCFS_RDY.getEntry(1);
				FCFS_RDY.remove(1);
				if (p->getParent() != nullptr)
					schedulerPtr->moveToTRM(p);
				else
					schedulerPtr->moveToNew(p);
				decreaseFinishTime(p->getRemainingCT());
			}
		}
		stopTime = timeStep;
		updateState(STOP);
	}
}

bool FCFS_Processor::isRDYEmpty()
{
	return FCFS_RDY.getcount() == 0;
}

//Handle the run process and top rdy processes
void FCFS_Processor::scheduleAlgo(int timeStep)
{
	//The Run Prcocess has Finished:
	if (isRunFinished()) 
	{
		schedulerPtr->moveToTRM(getRunProcess());
		setRun(nullptr);
	}

	//OverHeating Handling:
	overHeatHandling(timeStep);

	//Excueting The Run Process:
	if (!isIdle()) {
		increaseBusyTime();
		decreaseRunRemainingCT();
		requestIO();

		//Forking Handling:
		Process* runP = getRunProcess();
		bool v = forkProcess(runP);
		if (v)
			schedulerPtr->Fork(runP);
	}
	else
	{
		if (isRDYEmpty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess = FCFS_RDY.getEntry(1);
		FCFS_RDY.remove(1);

		//The Migration part:
		bool migrated = schedulerPtr->migrateFCFStoRR(RDYprocess);
		while (migrated && FCFS_RDY.getcount() != 0)
		{
			RDYprocess = FCFS_RDY.getEntry(1);
			FCFS_RDY.remove(1);
			decreaseFinishTime(RDYprocess->getRemainingCT());
			migrated = schedulerPtr->migrateFCFStoRR(RDYprocess);
		}
		if (migrated && FCFS_RDY.getcount() == 0)
		{
			//No More Processes In The RDY To Migrate Or Run:
			decreaseFinishTime(RDYprocess->getRemainingCT());
			setRun(nullptr);
			return;
		}
		else if (!migrated && isIdle())
		{
			//Just Set The Current Process As A Running Process:
			RDYprocess->setStart(timeStep);
			setRun(RDYprocess);
		}
	}

}


//---------------------------------------( FCFS Special )------------------------------------------------//

//Check if the running process should fork
bool FCFS_Processor::forkProcess(Process*& runProcess)
{
	if (isIdle())
		return false;
	srand(time(NULL));
	int random;
	random = (rand() % 100);
	runProcess = nullptr;
	if (random < forkingProbability)
	{
		runProcess = getRunProcess();
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

//Search for process & remove it from rdy/run if exists then return a pointer to it
bool FCFS_Processor::killProcess(int ID, Process*& target)
{
	if (getRunProcess() && getRunProcess()->getID() == ID)
	{
		target = getRunProcess();
		setRun(nullptr);
		return true;
	}
	int pos = -1;
	Process* KProcess = FCFS_RDY.search_by_ID(ID, pos);
	if (KProcess)
	{
		target = KProcess;
		FCFS_RDY.remove(pos);
		return true;
	}
	return false;
}


//-------------------------------------------( Printing )------------------------------------------------//

void FCFS_Processor::printRDY()
{
	FCFS_RDY.Print();
}

void FCFS_Processor::printInfo()
{
	cout << " [FCFS]: " << FCFS_RDY.getcount() << " ";
}
