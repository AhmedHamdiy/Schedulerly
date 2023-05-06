#include "RR_Processor.h"
#include"../Scheduler.h"
RR_Processor::RR_Processor(int Id, Scheduler* sc, int TS):Processor(Id,sc), timeSlice(TS)
{}

				//-------------------------------------( Scheduling )------------------------------------------------//

void RR_Processor::AddProcess(Process* p)
{
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

int RR_Processor::OverHeat(Processor* Shortest, int TimeStep, int TStop)
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
	if (randNum < 35)
	{
		if (!isIdle())
		{
			Process* Rn = GetRunProcess();
			Shortest->AddProcess(Rn);
			Travelled_Proces++;
			setRUN(nullptr);
		}
		if (!isRDYempty())
		{
			for (int i = 0; i < RDY.getcount(); i++)
			{

				Process* p = nullptr;
				RDY.dequeue(p);
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
		if (Rn->IncrementTS(timeSlice))
		{
			Inc_Finishtime(Rn->getRemainingCT());
			RDY.enqueue(Rn);
			setRUN(nullptr);
		}
		else
		{
			Dec_RUNCT();
			IO_Req();

		}
	}
	
	else
	{

		if (isRDYempty())
			return;

		//Choose The Next Run Process 
		Process* RDYprocess;
		RDY.dequeue(RDYprocess);
		
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

