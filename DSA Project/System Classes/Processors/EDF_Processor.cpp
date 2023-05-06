#include "SJF_Processor.h"
#include"../Scheduler.h"


EDF_Processor::EDF_Processor(int Id,Scheduler* sc):Processor(Id,sc)
{}

					//-------------------------------------( Scheduling )------------------------------------------------//

int EDF_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
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
				Rn->updateState(READY);
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

Process* EDF_Processor::remove_Top()
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

void EDF_Processor::AddProcess(Process* p)
{
	RDY.enqueue(p,p->get_DeadLine());	
	Inc_Finishtime(p->getRemainingCT());
}

bool EDF_Processor::isRDYempty()
{
	return RDY.isEmpty();
}

void EDF_Processor::ScheduleAlgo(int t)
{
	Process* RunP = GetRunProcess();
	if (!isIdle())
	{
		if (FinishRUN()) //The  Run Prcocess has Finished
		{
			if (RunP->get_DeadLine() >= t)
				MYSch->Inc_DeadLineCntr();
			MYSch->MoveToTRM(RunP);
			setRUN(nullptr);
		}
		IO_Req();
		Inc_BusyTime();	
		Dec_RUNCT();
	}

	if (isRDYempty())
		return;

	else
	{
		Process* EarlyDeadLine = RDY.Peek();
		
		//Exchange The Run Process With A Process With Earlier DeadLine
		bool Exchange_Condition = (!isIdle()) && (EarlyDeadLine->get_DeadLine() < RunP->get_DeadLine()) ;

		if (isIdle() || Exchange_Condition)
		{
			if (Exchange_Condition)
			{
				AddProcess(RunP);
				RunP->updateState(READY);
				Inc_Finishtime(RunP->getRemainingCT());
				setRUN(nullptr);

			}
			RDY.dequeue();
			EarlyDeadLine->updateState(RUNNING);
			Dec_Finishtime(EarlyDeadLine->getRemainingCT());
			setRUN(EarlyDeadLine);
			EarlyDeadLine->setstart(t);
		}

	}
}
 
				//-----------------------------------------( Printing )------------------------------------------------//

void EDF_Processor::printRDY()
{
	RDY.Print();
}

void EDF_Processor::printInfo()
{
	cout << " [EDF ]: " << RDY.getCount() << " ";
}
