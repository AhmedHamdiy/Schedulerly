#include"Process.h"
ostream& operator << (ostream& out, Process* p) //overloading stream operator to print id
{
	out << p->PID;
	return out;
}

Process::Process()
{
	LChild = nullptr;
	RChild = nullptr;
	Parent = nullptr;
	AT = 0;
	PID = 0;
	CT = 0;
	IOcount = 0;
	KillTime = 0;
	State = NEW;
	TT = 0;
	WT = 0;
	StartC = 0;
	BLKduration = 0;
	RemainingCT = 0;
}

Process::Process(int at, int id, int ct, int n,Process* p) :AT(at), PID(id), CT(ct)
{
	IOcount = 0;
	KillTime = 0;
	State = NEW;
	RChild = nullptr;
	LChild = nullptr;
	WT = 0;
	TT = 0;
	RemainingCT = ct;
	Parent = p;
}

Process::~Process()
{}

void Process::AddIO(Pair<int, int> p)
{
	IOList.enqueue(p);
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
	RemainingCT=T;
}

bool Process::isFinished()
{
	return CT==0;
}

int Process::getTRT()
{
	return TT+1 - AT;
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

void Process::setCT(int t)
{
	CT = t;
	RemainingCT = t;
}

int Process::getWT()
{
	if (State == TRM)
		return getTRT() - CT;
	else
		return WT;
}

void Process::updateState(state s)
{
		State = s;
}

void Process::updateWT()
{
	WT++;
}

void Process::setPID(int id)
{
	PID = id;
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
bool Process::canFork() const 
{
	return (!RChild || !LChild);
}
int Process::getRT()
{
	return startfirst - AT;
}
int Process::getTT()
{
	return TT+1;
}
void Process::setstart(int t)
{
	if (CT == RemainingCT)
		startfirst = t;
	StartC = t;
}

int Process::getstart()
{
	return StartC;
}

void Process::inc_blktime()
{
	BLKduration++;
}

void Process::resetblktime()
{
	BLKduration = 0;
}

int Process::getblktime()
{
	return BLKduration;
}

void Process::deqIO()
{
	Pair<int, int> p;
	IOList.dequeue(p);
}

void Process::setTT(int t)
{
	TT = t;
}


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

bool Process::GetIO(Pair<int, int>& temp)
{
	if (IOList.isEmpty())
		return 0;
	temp = IOList.peekFront();
	return 1; 
}

