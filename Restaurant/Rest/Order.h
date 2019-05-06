#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	static int Ocount;
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, Frozen, VIP
	REGION Region;  //Region of this order
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	double priority;

	int ArrTime, ServTime, WaitTime, FinishTime;	//arrival, service start, and finish times
	
	bool isAssigned;
	
	

public:
	Order(int id, ORD_TYPE r_Type, REGION r_region, int d, double m, int at);

	int GetID();

	void assignOrd(int T, int S);

	int GetType() const;
	REGION GetRegion() const;
	void promote(int exm);
	void SetDistance(int d);
	int GetDistance() const;
	int getServTime() const;
	int getFinishTime() const;
	int getArrivalTime() const;
	bool operator <=(const Order& Ord) const;

	
};

#endif