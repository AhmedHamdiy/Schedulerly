#include "SJF_Processor.h"

SJF_Processor::SJF_Processor(int Id)
{
	ID = Id; // Assign unique ID 
}

void SJF_Processor::ScheduleAlgo()
{
	/*Process* currentProcess = RDY.peekFront();
	setRUN(currentProcess);
	RDY.dequeue();
	Excuete();*/
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

bool SJF_Processor::Excuete()
{
	//if it returns true this means the process has ended and will be moved to the TRM list?? 
	Process* CurProcess = GetRunProcess();
	if (CurProcess == nullptr)
		return false;
	//else if -->>> requires I/O return false
	else {
		CurProcess->setCT(0);
		return true;
	}
}

void SJF_Processor::printRDY()
{
	RDY.Print();
}

void SJF_Processor::printInfo()
{
	//cout << "processor " << ID << " [SJF]: " << RDY.getsize() << " ";
	cout << " [SJF ]: " << RDY.getCount() << " ";
}

bool SJF_Processor::isRDYempty()
{
	return RDY.isEmpty();
}

bool SJF_Processor::RDYtoRUN(int t, Scheduler* scptr)
{
	if (isRDYempty() || !isIdle())
		return false;
	Process* RDYprocess = RDY.Peek();
	RDY.dequeue();
	RDYprocess->updateState(RUNNING);
	Dec_Finishtime(RDYprocess->getRemainingCT());
	setRUN(RDYprocess);
	return 1;
}


