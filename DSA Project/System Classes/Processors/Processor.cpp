#include"Processor.h"

Processor::Processor() {
	BusyTime = 0;
	RUN = nullptr;
	state = IDLE;
	FinishTime = 0;
}
bool Processor::setRUN(Process* p) {
	if (state == IDLE)
	{
		RUN = p;
		return true;
	}
	return false;
}
void Processor::setBusytime(int T) {
	BusyTime = T;
}
void Processor::Inc_Busytime(int T)
{
	BusyTime += T;
}
void Processor::Dec_Busytime(int T)
{
	BusyTime-=T;
}

bool Processor::FinishRUN()
{
	if (RUN)
	{
		if (RUN->getRemainingCT() == 0) {
			return true;
		}
		//return false;
	}
	return false;
}
void Processor::Dec_RUNCT()
{
	if (RUN)
		RUN->decrementCT();
}

int Processor::getBusytime() {
	return BusyTime;
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
bool Processor::operator<(Processor* p1)
{
	return (BusyTime < p1->BusyTime);
}
bool Processor::operator>(Processor* p1)
{
	return (BusyTime > p1->BusyTime);
}
bool Processor::isIdle()
{
	return (RUN == nullptr);
}

Process* Processor::GetRunProcess()
{
	return RUN;
}
bool Processor::removeProcess()
{
	if (state == BUSY)
	{
		RUN = nullptr;
		FlipState();
		return true;
	}
	return false;
}
