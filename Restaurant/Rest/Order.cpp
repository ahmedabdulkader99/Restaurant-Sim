#include "Order.h"

static int count = 0;

Order::Order(ORD_TYPE r_Type, REGION r_region, int d, double m, int at)
	:ID(++count), type(r_Type), Region(r_region), Distance(d), totalMoney(m), ArrTime(at), ServTime(0), FinishTime(0)
{}


int Order::GetID()
{
	return ID;
}


int Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return Region;
}

void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return Distance;
}
