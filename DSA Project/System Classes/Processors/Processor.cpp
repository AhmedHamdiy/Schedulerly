#include"Processor.h"
#include "../Scheduler.h"
Processor::Processor(int Id, Scheduler* sc,int OVT)
{
	OverHeat_time = OVT;
	ID = Id;
	MYSch = sc;
	RUN = nullptr;
	state = IDLE;
	StopTime = 0;
	FinishTime = 0;
	BusyTime = 0;
}


			//-------------------------------------( Getters )------------------------------------------------//

int Processor::get_Finishtime()
{
	return FinishTime;
}

PState Processor::getState()
{
	return state;
}

bool Processor::isIdle()
{
	return (RUN == nullptr);
}

int Processor::get_ID()
{
	return ID;
}

Process* Processor::GetRunProcess()
{
	return RUN;
}


			//-------------------------------------( Setters )------------------------------------------------//

bool Processor::setRUN(Process* p)
{
	if (state == IDLE)
	{
		RUN = p;
		return true;
	}
	return false;
}

void Processor::Inc_Finishtime(int T)
{
	FinishTime += T;
}

void Processor::Dec_Finishtime(int T)
{
	FinishTime -= T;
}

void Processor::Dec_RUNCT()
{
	if (RUN)
		RUN->decrementCT();
}

int Processor::get_remainingOverHeat(int TimeStep)
{
		return OverHeat_time + StopTime - TimeStep;
}

void Processor::Inc_BusyTime()
{
	if(!isIdle())
	BusyTime++;
}

void Processor::UpdateState(PState st)
{
	state = st;
}


			//-------------------------------------( Statistics )------------------------------------------------//

int Processor::getBusytime()
{
	return BusyTime;
}

double Processor::processorLoad(int totalTRT)
{
	return (BusyTime / (float)totalTRT) * 100;
}

double Processor::processorUtilization(int timeStep)
{
	return (BusyTime / (float)timeStep) * 100;
}


			//-------------------------------------( Scheduling )------------------------------------------------//

bool Processor::FinishRUN()
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

void Processor::IO_Req()
{
	Pair<int, int> temp;
	if (GetRunProcess() && GetRunProcess()->GetIO(temp))
	{
		if (temp.first == (GetRunProcess()->getCT() - GetRunProcess()->getRemainingCT()))
		{
			MYSch->RUNtoBLK(GetRunProcess());
			setRUN(nullptr);
		}
	}
}


			//-------------------------------------( Printing )------------------------------------------------//

ostream& operator<<(std::ostream& os, const Processor& p) {
	os << "(P" << p.ID << ")";
	return os;
}

