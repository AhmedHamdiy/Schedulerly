#include"Process.h"

Process::Process(int at, int id, int ct, int n,Process* p) :AT(at), PID(id), CT(ct)
{
	IOcount = 0;
	KillTime = 0;
	State = NEW;
	RChild = nullptr;
	LChild = nullptr;
	TS = 0;
	WT = 0;
	TT = 0;
	RemainingCT = ct;
	Parent = p;
	IOduration = 0;
}



						//-------------------------------------( Getters )------------------------------------------------//

bool Process::canFork() const
{
	return (!RChild || !LChild);
}
int Process::getRT()
{
	if (State==Killed)
		return 0;
	return startfirst - AT;
}
int Process::getTT()
{
	return TT + 1;
}

int Process::getstart()
{
	return StartC;
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

	return RemainingCT;
}

int Process::getCT()
{
	return CT;
}

int Process::getID()
{
	return PID;
}


int Process::getWT()
{
	if (State == TRM)
		return getTRT() - CT;
	else
		return WT;
}

int Process::getTS()
{
	return TS;
}

int Process::getIOduration()
{
	return IOduration;
}


						//-------------------------------------( Setters )------------------------------------------------//

void Process::setTT(int t)
{
	TT = t;
}

void Process::setstart(int t)
{
	if (CT == RemainingCT)
		startfirst = t;
	StartC = t;
}

void Process::SetKillTime(int k)
{
	KillTime = k;
}


void Process::incrementTT()
{
	TT++;
}

void Process::decrementCT()
{
	RemainingCT--;
}

void Process::setRemainingCT(int T)
{
	RemainingCT = T;
}


void Process::updateState(state s)
{
	State = s;
}

void Process::updateWT()
{
	WT++;
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
		TS = 0;
		return true;
	}
}

void Process::Inc_IOduration(int t)
{
	IOduration += t;
}


						//-------------------------------------( Forking )------------------------------------------------//

Process*& Process::get_LChild()
{
	return LChild;
}

Process*& Process::get_RChild()
{
	return RChild;
}
Process* Process::getParent()
{
	return Parent;
}

bool Process::setForked(Process* forkedP)
{
	if (!LChild)
	{
		LChild = forkedP;
		return true;
	}
	else if (!RChild)
	{
		RChild = forkedP;
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
