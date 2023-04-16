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
		AT = 0;
		PID = 0;
		CT = 0;
		IOcount = 0;
		KillTime = 0;
		State = NEW;
		TT = 0;
	
}

Process::Process(int a, int id, int c, int n) :AT(a), PID(id), CT(c)
{//IOarr = new pair<int, int>[n];
	IOcount = 0;
	KillTime = 0;
	State = NEW;
	RChild = nullptr;
	LChild = nullptr;
	TT = 0;
}

Process::~Process()
{
	if(LChild)
	{
		delete LChild;
		LChild=nullptr;
	}
	
	if(RChild)
	{
		delete RChild;
		RChild=nullptr;
	}
}

void Process::AddIO(Pair<int, int> p)
{
	IOList.enqueue(p);
}

void Process::SetKillTime(int k)
{
	KillTime = k;
}

int Process::GetID()
{
	return PID;
}

void Process::incrementTT()
{
	TT++;
}

int Process::getTRT()
{
	return TT - AT;
}

int Process::getAT()
{
	return AT;
}

int Process::getCT()
{
	return CT;
}

void Process::setCT(int t)
{
	CT = t;
}

int Process::getWT()
{
	return getTRT() - CT;
}

bool Process::updateState(state s)
{
	if (!isUpdated)
	{
		State = s;
		isUpdated = true;
		return true;
	}
	return false;
}

void Process::setPID(int id)
{
	PID = id;
}

void Process::Forking(Process*& firstChild, Process*& secondChild)
{
	RChild = firstChild;
	LChild = secondChild;
}

bool Process::operator<(const Process* other) const
{
	return (CT < other->CT);
}
