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
	WT = 0;
}

Process::Process(int at, int id, int ct, int n) :AT(at), PID(id), CT(ct)
{
	IOcount = 0;
	KillTime = 0;
	State = NEW;
	RChild = nullptr;
	LChild = nullptr;
	WT = 0;
	TT = 0;
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

int Process::getID()
{
	return PID;
}

void Process::setCT(int t)
{
	CT = t;
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

void Process::Forking(Process*& firstChild, Process*& secondChild)
{
	RChild = firstChild;
	LChild = secondChild;
}


Process*& Process::get_LChild()
{
	return LChild;
}

Process*& Process::get_RChild()
{
	return RChild;
}
