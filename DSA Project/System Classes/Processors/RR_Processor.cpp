#include "RR_Processor.h"
#include"../Scheduler.h"
RR_Processor::RR_Processor(int Id, Scheduler* sc, int OVT, int TS):Processor(Id,sc,OVT), timeSlice(TS)
{}

					//---------------------------------------( Scheduling )------------------------------------------------//

//Add process to rdy queue
void RR_Processor::addProcess(Process* p)
{
	p->updateState(READY);
	RR_RDY.enqueue(p);
	increaseFinishTime(p->getRemainingCT());
}

//Remove the peek process of rdy queue
Process* RR_Processor::removeTop()
{
	Process* p=nullptr;
	if (!RR_RDY.isEmpty())
	{
		RR_RDY.dequeue(p);
		decreaseFinishTime(p->getRemainingCT());
	}
	return p;
}

//Overheat the processor for overheat duration and move all the processes in it to the shortest rdy queue
void RR_Processor::turnOff(int TimeStep)
{
	srand(time(nullptr));
	bool Probability_Cond = (rand() % 1000 < overHeatProbability);
	if (Probability_Cond && !stopTime) //The Processor Isn't OverHeated 
	{
		if (!isIdle())
		{
			// Moving The Run Process To Shortest RDY Queue
			Process* Rn = getRunProcess();
			schedulerPtr->moveToNew(Rn);
			setRun(nullptr);
		}
		if (!isRDYEmpty())
		{
			// Moving The RDY Processes To Shortest RDY Queue
			for (int i = 0; i < RR_RDY.getcount(); i++)
			{
				Process* p = nullptr;
				RR_RDY.dequeue(p);
				schedulerPtr->moveToNew(p);
				decreaseFinishTime(p->getRemainingCT());
			}
		}
		stopTime = TimeStep;
		updateState(STOP);
	}
}

bool RR_Processor::isRDYEmpty()
{
	return RR_RDY.isEmpty();
}

//Handle the run process and top rdy processes
void RR_Processor::scheduleAlgo(int timeStep)
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
	if(!isIdle()){
		Process* Rn = getRunProcess();
		if (Rn->IncrementTS(timeSlice-1))
		{
			addProcess(Rn);
			setRun(nullptr);
		}
		else
		{
			increaseBusyTime();
			decreaseRunRemainingCT();
			requestIO();
			return;
		}
	}
	

		if (isRDYEmpty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess;
		RR_RDY.dequeue(RDYprocess);

		//The Migration part:
		 bool migrated = schedulerPtr->migrateRRtoSJF(RDYprocess);
		 while (migrated && RR_RDY.getcount() != 0)
		 {
			 RR_RDY.dequeue(RDYprocess);
			 decreaseFinishTime(RDYprocess->getRemainingCT());
			 migrated = schedulerPtr->migrateRRtoSJF(RDYprocess);
		 }
		 if (migrated && RR_RDY.getcount() == 0)
		 {
			 //No More Processes In The RDY To Migrate Or Run:
			 decreaseFinishTime(RDYprocess->getRemainingCT());
			 setRun(nullptr);
			 return;
		 }
		 else if (!migrated&& isIdle())
		 {
		 	//Just set The curent Process as A running process
			 RDYprocess->setStart(timeStep);
			 setRun(RDYprocess);
		 }  
}


					//-----------------------------------------( Printing )------------------------------------------------//

void RR_Processor::printRDY()
{
	RR_RDY.print();
}

void RR_Processor::printInfo()
{
	cout << " [RR  ]: " << RR_RDY.getcount() << " ";
}

