#include "FCFS_Processor.h"
#include"../Scheduler.h"

FCFS_Processor::FCFS_Processor(int Id)
{
	ID = Id; // Assign unique ID 
}

void FCFS_Processor::AddProcess(Process* p)
{
	RDY.insertEnd(p);
	Inc_Finishtime(p->getRemainingCT());
}

void FCFS_Processor::ScheduleAlgo()
{
}

bool FCFS_Processor::Excuete()
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

Process* FCFS_Processor::remove_Top()
{
	Process* p=nullptr;
	if (!RDY.isEmpty())
	{
		p = RDY.getEntry(1);
		RDY.remove(1);
		Dec_Finishtime(p->getRemainingCT());
	}
	return p;
}





bool FCFS_Processor::ForkProcess(Process*& runProcess, int forkP)
{
	if (isIdle()) //lw mafesh run aslan
		return false;
	srand(time(NULL));
	int random;
	random =(rand() % 100);
	runProcess = nullptr;
	if (random < forkP)
	{
		runProcess = GetRunProcess();
		if (runProcess->canFork())
		{
			return true;
		}
		else
		{
			runProcess = nullptr;
			return false;
		}
	}
	else
		return false;
}

bool FCFS_Processor::KillProcess(int ID, Process*& target)
//search for process,remove it from ready/run if exists then return a pointer to it
{
	if (GetRunProcess()&&GetRunProcess()->GetID() == ID)
	{
		target = GetRunProcess();
		setRUN(nullptr);
		return true;
	}
	int pos = -1;
	Process* KProcess = RDY.search_by_ID(ID, pos);
	if (KProcess)
	{
		target = KProcess;
		RDY.remove(pos);
		return true;
	}
	return false;
}


void FCFS_Processor::printRDY()
{
	RDY.Print();
}

void FCFS_Processor::printInfo()
{
	//cout << "processor " << ID << " [FCFS]: "<<RDY.getcount()<<" ";
	cout << " [FCFS]: " << RDY.getcount() << " ";
}

bool FCFS_Processor::isRDYempty()
{
	return RDY.getcount() == 0;
}

bool FCFS_Processor::RDYtoRUN(int t, Scheduler* scptr)
{
	if (isRDYempty() || !isIdle())
		return false;
	Scheduler* sc = scptr;
	Process* RDYprocess = RDY.getEntry(1);
	RDYprocess->setstart(t);
	Dec_Finishtime(RDYprocess->getRemainingCT());
	RDY.remove(1); //remove from ready

	while (sc->MigrationFCFStoRR(RDYprocess))
	{
		RDYprocess = RDY.getEntry(1);
		RDYprocess->setstart(t);
		Dec_Finishtime(RDYprocess->getRemainingCT());
		RDY.remove(1); //remove from ready
	}
	RDYprocess->updateState(RUNNING);
	setRUN(RDYprocess);
	return true;
}

void FCFS_Processor::Inc_WT()
{
	for (int i = 1; i <= RDY.getcount(); i++)
	{
		Process* p = RDY.getEntry(i);
		p->updateWT();
	}
}
