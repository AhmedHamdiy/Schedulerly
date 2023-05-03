#include "RR_Processor.h"
#include"../Scheduler.h"
RR_Processor::RR_Processor(int Id, int TS)
{
	ID = Id;  // Assign unique ID
	timeSlice = TS; // Assign TimeSlice
	

}

void RR_Processor::AddProcess(Process* p)
{
	RDY.enqueue(p);
	Inc_Busytime(p->getRemainingCT());
}

void RR_Processor::ScheduleAlgo()
{
	Process* currentProcess = RDY.peekFront();
	//Scheduler* sc;
	setRUN(currentProcess);
	RDY.dequeue(currentProcess);
	if (Excuete())
	{
		//sc->MoveToTRM(currentProcess);
	}
	else //if (!sc->MigrationRRtoSJF(currentProcess))  
	{
		RDY.enqueue(currentProcess);
	}
}

bool RR_Processor::Excuete()
{
	//if it returns true this means the process has ended and will be moved to the TRM list?? 
	Process* CurProcess = GetRunProcess();
	if (CurProcess ==nullptr)
	return false;
	//else if -->>> requires I/O return false
	else { 
		//Decrement the process CT by Time slice
		CurProcess->setRemainingCT(CurProcess->getRemainingCT() - timeSlice);
		if (CurProcess->getCT() == 0)
			return true;
		else
			return false;
	}
}

void RR_Processor::printRDY()
{
	RDY.print();
}

void RR_Processor::printInfo()
{
	//cout << "processor " << ID << " [RR]: " << RDY.getcount() << " ";
	cout << " [RR  ]: " << RDY.getcount() << " ";
}

bool RR_Processor::isRDYempty()
{
	return RDY.isEmpty();
}

bool RR_Processor::RDYtoRUN(int t)
{
	//Scheduler* sc;
	if (isRDYempty() || !isIdle())
		return false;
	Process* RDYprocess;
	RDY.dequeue(RDYprocess);
	Dec_Busytime(RDYprocess->getRemainingCT());
	RDYprocess->updateState(RUNNING);
	setRUN(RDYprocess);
	return 1;
}


