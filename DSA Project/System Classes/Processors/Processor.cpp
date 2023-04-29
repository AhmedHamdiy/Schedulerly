#include"Processor.h"
int Processor::nextID = 1;

Processor::Processor() {
	busyTime = 0;
	RUN = nullptr;
	state = IDLE;
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
	busyTime = T;
}
void Processor::Inc_Busytime(int T)
{
	busyTime += T;
}
void Processor::Dec_Busytime(int T)
{
	busyTime-=T;
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
	return busyTime;
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
	return (busyTime < p1->busyTime);
}
bool Processor::operator>(Processor* p1)
{
	return (busyTime > p1->busyTime);
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
