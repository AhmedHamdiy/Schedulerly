#include "SJF_Processor.h"
#include"../Scheduler.h"


SJF_Processor::SJF_Processor(int Id, Scheduler* sc, int OVT):Processor(Id,sc,OVT)
{}

					//-------------------------------------( Scheduling )------------------------------------------------//

void SJF_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
{
	if (!StopTime&& get_remainingOverHeat(TimeStep)> 0) //The Processor Isn't OverHeated 
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
	p->updateState(READY);
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
	TurnON(t);
	if (!isIdle())
	{
		Inc_BusyTime();	
		Dec_RUNCT();
		IO_Req();
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
