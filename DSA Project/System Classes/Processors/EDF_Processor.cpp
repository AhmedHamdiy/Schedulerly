#include "SJF_Processor.h"
#include"../Scheduler.h"


EDF_Processor::EDF_Processor(int Id, Scheduler* sc, int OVT):Processor(Id,sc,OVT)
{}


					//---------------------------------------( Scheduling )-------------------------------------------------//

void EDF_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
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
			for (int i = 0; i < RDY.getCount(); i++)
			{
				Process* p = RDY.Peek();
				RDY.dequeue();
				Shortest->AddProcess(p);
				Dec_Finishtime(p->getRemainingCT());
			}
		}
		StopTime = TimeStep;
		UpdateState(STOP);
	}
	else if (get_remainingOverHeat(TimeStep) <= 0)
	{
		if (getState() == BUSY)
			UpdateState(BUSY);
		else
			UpdateState(IDLE);
		StopTime = 0;
	}
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
	p->updateState(READY);
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
