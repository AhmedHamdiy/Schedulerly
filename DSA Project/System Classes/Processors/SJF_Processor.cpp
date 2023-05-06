#include "SJF_Processor.h"
#include"../Scheduler.h"


SJF_Processor::SJF_Processor(int Id,Scheduler* sc):Processor(Id,sc)
{}

					//-------------------------------------( Scheduling )------------------------------------------------//

int SJF_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
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
				Shortest->AddProcess(Rn);
				setRUN(nullptr);
				Travelled_Proces++;
			}
			if (!isRDYempty())
			{
				for (int i = 0; i < RDY.getCount(); i++)
				{
					Process* p = RDY.Peek();
					RDY.dequeue();
					Shortest->AddProcess(p);
					Dec_Finishtime(p->getRemainingCT());
					Travelled_Proces++;
				}
			}
			StopTime = TimeStep;
			UpdateState(STOP);
		}
	return Travelled_Proces;
}

Process* SJF_Processor::remove_Top()
{
	Process* p = nullptr;
	if (!RDY.isEmpty())
	{
		p=RDY.Peek();
		RDY.dequeue();
		Dec_Finishtime(p->getRemainingCT());
	}
	return p;
}

void SJF_Processor::AddProcess(Process* p)
{
	RDY.enqueue(p,p->getCT());	
	Inc_Finishtime(p->getRemainingCT());
}

bool SJF_Processor::isRDYempty()
{
	return RDY.isEmpty();
}

void SJF_Processor::ScheduleAlgo(int t)
{
	if (FinishRUN()) //The  Run Prcocess has Finished
	{
		MYSch->MoveToTRM(GetRunProcess());
		setRUN(nullptr);
	}
	IO_Req();
	if (!isIdle())
	{
		Inc_BusyTime();	
		Dec_RUNCT();
	}
	else{

		if (isRDYempty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess = RDY.Peek();
		RDY.dequeue();
		RDYprocess->updateState(RUNNING);
		Dec_Finishtime(RDYprocess->getRemainingCT());
		setRUN(RDYprocess);
		RDYprocess->setstart(t);
	}
}
 
			//-----------------------------------------( Printing )------------------------------------------------//

void SJF_Processor::printRDY()
{
	RDY.Print();
}

void SJF_Processor::printInfo()
{
	cout << " [SJF ]: " << RDY.getCount() << " ";
}
