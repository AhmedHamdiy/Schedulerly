#include "SJF_Processor.h"
#include"../Scheduler.h"


EDF_Processor::EDF_Processor(int Id, Scheduler* sc, int OVT):Processor(Id,sc,OVT)
{}


					//---------------------------------------( Scheduling )-------------------------------------------------//

//Overheat the processor for overheat duration and move all the processes in it to the shortest rdy queue
void EDF_Processor::turnOff(int timeStep)
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
			for (int i = 0; i < EDF_RDY.getCount(); i++)
			{
				Process* p = EDF_RDY.Peek();
				EDF_RDY.dequeue();
				schedulerPtr->moveToNew(p);
				decreaseFinishTime(p->getRemainingCT());
			}
		}
		stopTime = timeStep;
		updateState(STOP);
	}
}

//Remove the peek process of rdy queue
Process* EDF_Processor::removeTop()
{
	Process* p = nullptr;
	if (!EDF_RDY.isEmpty())
	{
		p=EDF_RDY.Peek();
		EDF_RDY.dequeue();
		decreaseFinishTime(p->getRemainingCT());
	}
	return p;
}

//Add process to rdy queue
void EDF_Processor::addProcess(Process* p)
{
	p->updateState(READY);
	EDF_RDY.enqueue(p,p->getDeadline());	
	increaseFinishTime(p->getRemainingCT());
}

bool EDF_Processor::isRDYEmpty()
{
	return EDF_RDY.isEmpty();
}

//Handle the run process and top rdy processes
void EDF_Processor::scheduleAlgo(int timeStep)
{
	Process* RunP = getRunProcess();

	//The  Run Prcocess has Finished:
	if (isRunFinished())
	{
		if (RunP->getDeadline() >= timeStep)
			schedulerPtr->increaseDeadLineCounter();
		schedulerPtr->moveToTRM(RunP);
		setRun(nullptr);
	}

	//OverHeating Handling:
	overHeatHandling(timeStep);
	//Excuting The Current Run:
	if (!isIdle())
	{
		requestIO();
		increaseBusyTime();
		decreaseRunRemainingCT();
	}

	//Choosing The Next Run Process Or Exchange It: 
	if (isRDYEmpty())
		return;

	else
	{
		Process* EarlyDeadLine = EDF_RDY.Peek();
		
		//Exchange The Run Process With A Process With Earlier DeadLine
		bool Exchange_Condition = (!isIdle()) && (EarlyDeadLine->getDeadline() < RunP->getDeadline()) ;

		if (isIdle() || Exchange_Condition)
		{
			if (Exchange_Condition)
			{
				//Return The Run Process To RDY Queue:
				addProcess(RunP);
				setRun(nullptr);
			}
			EDF_RDY.dequeue();
			setRun(EarlyDeadLine);
			EarlyDeadLine->setstart(timeStep);
		}

	}
}
 

					//-----------------------------------------( Printing )------------------------------------------------//

void EDF_Processor::printRDY()
{
	EDF_RDY.Print();
}

void EDF_Processor::printInfo()
{
	cout << " [EDF ]: " << EDF_RDY.getCount() << " ";
}
