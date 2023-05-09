#include"System Classes/Scheduler.h"

int main()
{
	Scheduler* s = new Scheduler;
	s->Simulation();
	delete s;
	return 0;
}
