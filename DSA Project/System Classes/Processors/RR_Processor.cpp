#include "RR_Processor.h"
RR_Processor::RR_Processor()
{
	ID = nextID++; // Assign unique ID 
}

void RR_Processor::setTimeSlice(int t)
{ // Use it while reading the input file
	timeSlice = t;
}

void RR_Processor::AddProcess(Process* p)
{
	RDY.enqueue(p);
	// because BusyTime is Rvalue
	int oldBusyTime = getBusytime();
	setBusytime(oldBusyTime += p->getCT());
}

void RR_Processor::ScheduleAlgo()
{
	/*Process* currentProcess = RDY.peekFront();
	setRUN(currentProcess);
	RDY.dequeue(currentProcess);
	if (Excuete())
	{
		sc = nullptr;
		sc->MoveToTRM(currentProcess);
	}
	else //if (!sc->MigrationRRtoSJF(currentProcess))   -> Phase 2
	{
		RDY.enqueue(currentProcess);
	}*/
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
		//CurProcess->setCT(CurProcess->getCT() - timeSlice);
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
	if (isRDYempty() || !isIdle())
		return false;
	Process* RDYprocess;
	RDY.dequeue(RDYprocess);
	RDYprocess->updateState(RUNNING);
	setRUN(RDYprocess);
	RDYprocess->setstart(t);
	return 1;
}


