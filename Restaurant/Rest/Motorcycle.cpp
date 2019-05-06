#include "Motorcycle.h"

int Motorcycle::Mcount = 0;

Motorcycle::Motorcycle()
{
}

Motorcycle::Motorcycle(ORD_TYPE motoType, int s, REGION R)
	:ID(++Mcount), type(motoType), speed(s), region(R), status(IDLE), returnTime(0)
{
	Ord = nullptr;
}

bool Motorcycle::checkIfArrived(int timeStep)
{ 
	if (status == SERV) {
		if (timeStep >= returnTime) {
			status = IDLE;
			returnTime = 0;
			Ord = nullptr;
			return true;
		}
	}
	return false;
}

void Motorcycle::assign(Order * pOrd)
{
	if (status == IDLE)
	{
		Ord = pOrd;
		status = SERV;
		returnTime = pOrd->getFinishTime() + pOrd->getServTime();
	}
}

bool Motorcycle::isIdle() const
{
	if (status == IDLE)
		return true;
	return false;
}
