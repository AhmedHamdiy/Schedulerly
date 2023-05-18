#include"Processor.h"
#include "../Scheduler.h"
Processor::Processor(int Id, Scheduler* sc,int OVT)
{
	overHeatProbability = 1;
	overHeatTime = OVT;
	ID = Id;
	schedulerPtr = sc;
	RUN = nullptr;
	State = IDLE;
	stopTime = 0;
	finishTime = 0;
	busyTime = 0;
}


			//-------------------------------------( Getters )------------------------------------------------//

int Processor::getFinishTime()
{
	return finishTime;
}

processorState Processor::getState()
{
	return State;
}

bool Processor::isIdle()
{
	return (RUN == nullptr);
}

int Processor::getID()
{
	return ID;
}

Process* Processor::getRunProcess()
{
	return RUN;
}


			//-------------------------------------( Setters )------------------------------------------------//

bool Processor::setRun(Process* p)
{
	if (State == IDLE)
	{
		if (p!=nullptr)
		{
			p->updateState(RUNNING);
			decreaseFinishTime(p->getRemainingCT());
		}
		RUN = p;
		return true;
	}
	return false;
}

void Processor::increaseFinishTime(int Time)
{
	finishTime += Time;
}

void Processor::decreaseFinishTime(int Time)
{
	finishTime -= Time;
}

void Processor::decreaseRunRemainingCT()
{
	if (RUN)
		RUN->decrementCT();
}

int Processor::getHealingSteps(int timeStep)
{
		return overHeatTime + stopTime - timeStep;
}

void Processor::TurnOn(int timeStep)
{
	if (getHealingSteps(timeStep) <= 0)
	{
		if (getState() == BUSY)
			updateState(BUSY);
		else
			updateState(IDLE);
		stopTime = 0;
	}
}

void Processor::increaseBusyTime()
{
	if(!isIdle())
	busyTime++;
}

void Processor::updateState(processorState st)
{
	State = st;
}


			//-------------------------------------( Statistics )------------------------------------------------//

int Processor::getBusytime()
{
	return busyTime;
}

double Processor::processorLoad(int totalTRT)
{
	return (busyTime / (float)totalTRT) * 100;
}

double Processor::processorUtilization(int timeStep)
{
	return (busyTime / (float)timeStep) * 100;
}


			//-------------------------------------( Scheduling )------------------------------------------------//

bool Processor::isRunFinished()
{
	if (RUN)
	{
		if (RUN->getRemainingCT() == 0)
		{
			return true;
		}
	}
	return false;
}

void Processor::requestIO()
{
	Pair<int, int> temp;
	if (getRunProcess() && getRunProcess()->GetIO(temp))
	{
		if (temp.first == (getRunProcess()->getCT() - getRunProcess()->getRemainingCT()))
		{
			schedulerPtr->moveToBLK(getRunProcess());
			setRun(nullptr);
		}
	}
}

void Processor::overHeatHandling(int timeStep)
{
	if (getState() == STOP)
		TurnOn(timeStep);
	else
		turnOff(timeStep);

}


			//-------------------------------------( Printing )------------------------------------------------//

ostream& operator<<(std::ostream& os, const Processor& p) 
{
	os << "(P" << p.ID << ")";
	return os;
}

