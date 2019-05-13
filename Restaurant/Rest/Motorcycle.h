#ifndef __MOTORCYCLE_H_
#define __MOTORCYCLE_H_

#include "..\Rest\Order.h"
#include "..\Defs.h"

#pragma once
class Motorcycle	
{
	static int Mcount;
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding motorcycle type 
	int speed;		//meters it can move in one clock tick (in one timestep)
	REGION	region;	//region of the motorcycle
	STATUS	status;	//idle or in-service
	Order* Ord;

	int returnTime;

public:
	Motorcycle();
	Motorcycle(ORD_TYPE motoType, int s, REGION R);
	bool checkIfArrived(int timeStep);
	void assign(Order* pOrd);
	bool isIdle() const;
	int getOrderInfo(char &T);
	int getDistance();
	int getID();
	int getSpeed();
	char getType();
};

#endif