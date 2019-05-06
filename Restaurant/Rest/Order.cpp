#include "Order.h"


Order::Order(int id,ORD_TYPE r_Type, REGION r_region, int d, double m, int at)
	:ID(id), type(r_Type), Region(r_region), Distance(d), totalMoney(m), ArrTime(at), ServTime(0), FinishTime(0), isAssigned(false)
{
	if (type == TYPE_VIP)
		priority = 10 * totalMoney / (ArrTime * Distance);
	else
		priority = -1;
}


int Order::GetID()
{
	return ID;
}

void Order::assignOrd(int T, int S)
{
	ServTime = Distance / S;
	WaitTime = T - ArrTime;
	FinishTime = ArrTime + WaitTime + ServTime;

	isAssigned = true;
}


int Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return Region;
}

void Order::promote(int exm)
{
	if (type == TYPE_NRM)
	{
		type = TYPE_VIP;
		totalMoney += exm;
		priority = 10 * totalMoney / (ArrTime * Distance);
	}		
}

void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}

int Order::getServTime() const
{
	return ServTime;
}

int Order::getFinishTime() const
{
	return FinishTime;
}

int Order::getArrivalTime() const
{
	return ArrTime;
}

bool Order::operator<=(const Order& Ord) const
{
	if (priority <= Ord.priority)
		return true;
	else
		return false;
}
