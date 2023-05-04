#include"Processor.h"

Processor::Processor() {
	RUN = nullptr;
	state = IDLE;
	FinishTime = 0;
	BusyTime = 0;
}
bool Processor::setRUN(Process* p) {
	if (state == IDLE)
	{
		RUN = p;
		return true;
	}
	return false;
}
int Processor::getBusytime() {
	return BusyTime;
}
void Processor::Inc_Finishtime(int T)
{
	FinishTime += T;
}
void Processor::Dec_Finishtime(int T)
{
	FinishTime -=T;
}

void Processor::Inc_BusyTime()
{
	if(!isIdle())
	BusyTime++;
}

bool Processor::FinishRUN()
{
	if (RUN)
	{
		if (RUN->getRemainingCT() == 0) 
		{
			return true;
		}
	}
	return false;
}
void Processor::Dec_RUNCT()
{
	if (RUN)
		RUN->decrementCT();
}

int Processor::get_Finishtime() {
	return FinishTime;
}

ostream& operator<<(std::ostream& os, const Processor& p) {
	os << "(P" << p.ID << ")";
	return os;
}

int Processor::getID() {
	return ID;
}
void Processor::FlipState()
{
	if (state = BUSY)
		state = IDLE;
	else
		state = BUSY;
}

bool Processor::isIdle()
{
	return (RUN == nullptr);
}

Process* Processor::GetRunProcess()
{
	return RUN;
}

double Processor::processorLoad(int totalTRT)
{
	return (BusyTime/(float)totalTRT)*100;
}

double Processor::processorUtilization(int timeStep)
{
	return (BusyTime/(float)timeStep)*100;
}
