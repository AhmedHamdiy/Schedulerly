#include"Processor.h"
int Processor::nextID = 1;

Processor::Processor() {
	busyTime = 0;
	RUN = nullptr;
}
void Processor::setRUN(Process* p){
	RUN = p; 
}
void Processor::setBusytime(int T) { 
	busyTime = T;
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
	 
		return (GetRunProcess() == nullptr);
	
}

Process* Processor::GetRunProcess()
{
	return RUN;
}

Process* Processor::KillRand(int ID)
{
	return NULL;
}
