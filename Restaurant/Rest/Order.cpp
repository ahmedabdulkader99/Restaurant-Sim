#include "Order.h"


Order::Order(int id,ORD_TYPE r_Type, REGION r_region, int d, double m, int at)
	:ID(id), type(r_Type), Region(r_region), Distance(d), totalMoney(m), ArrTime(at), ServTime(0), FinishTime(0), isAssigned(false)
{
	if (type == TYPE_VIP)
		priority = totalMoney;
	else
		priority = -1;
}


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

void Order::promote(int exm)
{
	if (type == TYPE_NRM)
	{
		type = TYPE_VIP;
		totalMoney += exm;
		priority = totalMoney;
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

bool Order::operator<=(const Order& Ord) const
{
	if (priority <= Ord.priority)
		return true;
	else
		return false;
}
