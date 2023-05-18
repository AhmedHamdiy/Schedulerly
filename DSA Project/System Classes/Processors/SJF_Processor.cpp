#include "SJF_Processor.h"
#include"../Scheduler.h"


SJF_Processor::SJF_Processor(int Id, Scheduler* sc, int OVT):Processor(Id,sc,OVT)
{}

					//-------------------------------------( Scheduling )------------------------------------------------//

void SJF_Processor::turnOff(int timeStep)
{
	srand(time(0));
	bool Probability_Cond = (rand() % 100 < overHeatProbability);
	if (Probability_Cond && !stopTime && getHealingSteps(timeStep) > 0) //The Processor Isn't OverHeated 
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
			for (int i = 0; i < SJF_RDY.getCount(); i++)
			{
				Process* p = SJF_RDY.Peek();
				SJF_RDY.dequeue();
				schedulerPtr->moveToNew(p);
				decreaseFinishTime(p->getRemainingCT());
			}
		}
		stopTime = timeStep;
		updateState(STOP);
	}
}

Process* SJF_Processor::removeTop()
{
	Process* p = nullptr;
	if (!SJF_RDY.isEmpty())
	{
		p=SJF_RDY.Peek();
		SJF_RDY.dequeue();
		decreaseFinishTime(p->getRemainingCT());
	}
	return p;
}

void SJF_Processor::addProcess(Process* p)
{
	p->updateState(READY);
	SJF_RDY.enqueue(p,p->getCT());	
	increaseFinishTime(p->getRemainingCT());
}

bool SJF_Processor::isRDYEmpty()
{
	return SJF_RDY.isEmpty();
}

void SJF_Processor::scheduleAlgo(int timeStep)
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
	if (!isIdle())
	{
		increaseBusyTime();	
		decreaseRunRemainingCT();
		requestIO();
	}
	else{
		if (isRDYEmpty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess = SJF_RDY.Peek();
		SJF_RDY.dequeue();
		setRun(RDYprocess);
		RDYprocess->setstart(timeStep);
	}

}
 

					//-----------------------------------------( Printing )------------------------------------------------//

void SJF_Processor::printRDY()
{
	SJF_RDY.Print();
}

void SJF_Processor::printInfo()
{
	cout << " [SJF ]: " << SJF_RDY.getCount() << " ";
}
