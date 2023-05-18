#include"Process.h"

Process::Process(int at, int id, int ct, int dead_line, int n,  Process* P) :AT(at), PID(id), CT(ct)
{
	Deadline = dead_line;
	IOduration = 0;
	IOCount = 0;
	killTime = 0;
	State = NEW;
	rightChild = nullptr;
	leftChild = nullptr;
	TS = 0;
	WT = 0;
	TT = 0;
	remainingCT = ct;
	Parent = P;
}


						//-------------------------------------( Getters )------------------------------------------------//

bool Process::canFork() const
{
	return (!rightChild || !leftChild);
}

int Process::getRT()
{
	if (State==Killed)
		return 0;
	return startFirst - AT;
}

int Process::getTT()
{
	return TT + 1;
}

int Process::getstart()
{
	return startProcessing;
}

int Process::getTRT()
{
	return TT - AT;
}

int Process::getAT()
{
	return AT;
}

int Process::getRemainingCT()
{

	return remainingCT;
}

int Process::getCT()
{
	return CT;
}

int Process::getID()
{
	return PID;
}

int Process::getWT(int timeStep)
{
	if (State == TRM|| State==Killed)
		return TT - AT;
	return timeStep - AT - (CT - remainingCT);
}

int Process::getTS()
{
	return TS;
}

int Process::getDeadline()
{
	return Deadline;
}

int Process::getIOduration()
{
	return IOduration;
}

void Process::clear_IOList()
{
	IOList.clear();
}


						//-------------------------------------( Setters )------------------------------------------------//
void Process::Inc_IOduration(int t)
{
	IOduration += t;
}

void Process::setTT(int t)
{
	TT = t;
}

void Process::setstart(int t)
{
	if (CT == remainingCT)
		startFirst = t;
	startProcessing = t;
}

void Process::reset_TS()
{
	TS = 0;
	updateState(READY);
}

void Process::setKillTime(int k)
{
	killTime = k;
}

void Process::incrementTT()
{
	TT++;
}

void Process::decrementCT()
{
	remainingCT--;
}

void Process::setRemainingCT(int T)
{
	remainingCT = T;
}

void Process::updateState(processState s)
{
	State = s;
}

bool Process::IncrementTS(int TSlice)
{
	if (TS < TSlice)
	{
		TS++;
		return false;
	}
	else
	{
		reset_TS();
		return true;
	}
}


						//-------------------------------------( Forking )------------------------------------------------//

Process*& Process::get_LChild()
{
	return leftChild;
}

Process*& Process::get_RChild()
{
	return rightChild;
}

Process*& Process::getParent()
{
	return Parent;
}

bool Process::setForked(Process* forkedP)
{
	if (!leftChild)
	{
		leftChild = forkedP;
		return true;
	}
	else if (!rightChild)
	{
		rightChild = forkedP;
		return true;
	}
	else
		return false;
}


						//-----------------------------------( I/O Handling )----------------------------------------//

void Process::AddIO(Pair<int, int> p)
{
	IOList.enqueue(p);
}

void Process::inc_blktime()
{
	BLKduration++;
}

void Process::resetblktime()
{
	BLKduration = 0;
}

void Process::deqIO()
{
	Pair<int, int> p;
	IOList.dequeue(p);
	Inc_IOduration(p.second);
}

int Process::getblktime()
{
	return BLKduration;
}

bool Process::GetIO(Pair<int, int>& temp)
{
	if (IOList.isEmpty())
		return false;
	temp = IOList.peekFront();
	return true;
}


						//-----------------------------------( Printing )----------------------------------------//

ostream& operator << (ostream& out, Process* p) //overloading stream operator to print id
{
	out << p->PID;
	return out;
}
