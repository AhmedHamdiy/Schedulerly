#include "RR_Processor.h"
#include"../Scheduler.h"
RR_Processor::RR_Processor(int Id, Scheduler* sc, int OVT, int TS):Processor(Id,sc,OVT), timeSlice(TS)
{}

					//---------------------------------------( Scheduling )------------------------------------------------//

void RR_Processor::AddProcess(Process* p)
{
	p->updateState(READY);
	RDY.enqueue(p);
	Inc_Finishtime(p->getRemainingCT());
}

Process* RR_Processor::remove_Top()
{
	Process* p=nullptr;
	if (!RDY.isEmpty())
	{
		RDY.dequeue(p);
		Dec_Finishtime(p->getRemainingCT());
	}
	return p;
}

void RR_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
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
			for (int i = 0; i < RDY.getcount(); i++)
			{
				Process* p = nullptr;
				RDY.dequeue(p);
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

bool RR_Processor::isRDYempty()
{
	return RDY.isEmpty();
}

void RR_Processor::ScheduleAlgo(int t)
{
	if (FinishRUN()) //The Run Prcocess has Finished
	{
		MYSch->MoveToTRM(GetRunProcess());
		setRUN(nullptr);
	}
	if(!isIdle()){
		Inc_BusyTime();
		Process* Rn = GetRunProcess();
		if (Rn->IncrementTS(timeSlice-1))
		{
			Inc_Finishtime(Rn->getRemainingCT());
			RDY.enqueue(Rn);
			setRUN(nullptr);
		}
		else
		{
			Dec_RUNCT();
			IO_Req();
			return;
		}
	}
	

		if (isRDYempty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess;
		RDY.dequeue(RDYprocess);
		Dec_Finishtime(RDYprocess->getRemainingCT());

		//The Migration part:
		 bool migrated = MYSch->MigrationRRtoSJF(RDYprocess);
		 while (migrated && RDY.getcount() != 0)
		 {
			 RDY.dequeue(RDYprocess);
			 Dec_Finishtime(RDYprocess->getRemainingCT());
			 migrated = MYSch->MigrationRRtoSJF(RDYprocess);
		 }
		 if (migrated && RDY.getcount() == 0)
		 {
			 setRUN(nullptr);
			 return;
		 }
		 else if (!migrated&& isIdle())
		 {
		 	//Just set The curent Process as A running process
			 Dec_Finishtime(RDYprocess->getRemainingCT());
			 RDYprocess->setstart(t);
			 setRUN(RDYprocess);
			RDYprocess->updateState(RUNNING);
		 }  
}


					//-----------------------------------------( Printing )------------------------------------------------//

void RR_Processor::printRDY()
{
	RDY.print();
}

void RR_Processor::printInfo()
{
	cout << " [RR  ]: " << RDY.getcount() << " ";
}

