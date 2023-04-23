#include "FCFS_Processor.h"

FCFS_Processor::FCFS_Processor()
{
	ID = nextID++; // Assign unique ID 
}

void FCFS_Processor::AddProcess(Process* p)
{
	RDY.insertEnd(p);
	// because BusyTime is Rvalue
	int oldBusyTime = getBusytime();
	//set_Processor_Busytime(oldBusyTime += p->getCT());
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

bool FCFS_Processor::Kill(int ID)
{
	/*Process* p;
	//p = get_Process(ID);
	//We need a Function to get the process by using its ID???
	bool RDYCondition=1;
	//= (RDY.getPointerto(p) == nullptr);
	bool RUNCondition = (GetRunProcess()->GetID() == ID);
	if (RDYCondition || RUNCondition)
	{
		//move the process to TRM 
		return true;
	}*/
	//ignore it
	return false;

}

Process* FCFS_Processor::KillRand(int RandID)
{
	Process* killed=nullptr;
	if(RDY.getEntry(RandID,killed))//checking if RandID in RDY
	{
		killed->updateState(TRM);
		RDY.remove(RandID);
	}
	//else if (GetRunProcess() &&GetRunProcess()->GetID() == RandID)//checking if RandID is the RUN Process
	//{
	//	killed = GetRunProcess();
	//	setRUN(nullptr);
	//}
	return killed; //returning pointer to to-be-killed process "NULL if not found"
}

bool FCFS_Processor::Fork(int ID)
{
	return 0;
}

void FCFS_Processor::printRDY()
{
	RDY.print();
}

void FCFS_Processor::printInfo()
{
	//cout << "processor " << ID << " [FCFS]: "<<RDY.getcount()<<" ";
	cout<<" [FCFS]: " << RDY.getcount() << " ";
}

bool FCFS_Processor::isRDYempty()
{
	return RDY.getcount() == 0;
}

bool FCFS_Processor::RDYtoRUN()
{
	if(isRDYempty()||!isIdle()) 
	return false;
	Process* RDYprocess;
	RDY.getEntry(1,RDYprocess);
	setRUN(RDYprocess);
	RDYprocess->updateState(RUNNING);
	RDY.remove(1); //remove from ready
	return true;

}


