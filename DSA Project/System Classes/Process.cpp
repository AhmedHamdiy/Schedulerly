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

//Check if process has empty child pointer to fork
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

int Process::getStart()
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

int Process::getBlkDuration()
{
	return IOduration;
}

void Process::clearIOList()
{
	IOList.clear();
}


						//-------------------------------------( Setters )------------------------------------------------//

void Process::IncreaseBlkDuration(int t)
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

void Process::removeChildrenPtr()
{
		leftChild = nullptr;
		rightChild = nullptr;
}

void Process::resetTimeSlice()
{
	TS = 0;
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
		resetTimeSlice();
		return true;
	}
}


						//-------------------------------------( Forking )------------------------------------------------//

Process*& Process::getLeftChild()
{
	return leftChild;
}

Process*& Process::getRightChild()
{
	return rightChild;
}

Process*& Process::getParent()
{
	return Parent;
}

//Assign the forked process to the empty child pointer
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


void Process::addIO(Pair<int, int> p)
{
	IOList.enqueue(p);
}

void Process::increaseBlkTime()
{
	BLKduration++;
}

void Process::resetBlkTime()
{
	BLKduration = 0;
}

void Process::deqeueIORequest()
{
	Pair<int, int> p;
	IOList.dequeue(p);
	IncreaseBlkDuration(p.second);
}

int Process::getBlkTime()
{
	return BLKduration;
}

bool Process::getIO(Pair<int, int>& temp)
{
	if (IOList.isEmpty())
		return false;
	temp = IOList.peekFront();
	return true;
}


						//-----------------------------------( Printing )----------------------------------------//

//Overloading stream operator to print id
ostream& operator << (ostream& out, Process* p) 
{
	out << p->PID;
	return out;
}
