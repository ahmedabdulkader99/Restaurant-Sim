#pragma once
#ifndef __Region_h
#define __Region_h

#include "../Generic_DS/PriorityQueue.h"
#include "../Generic_DS/ArrayList.h"
#include "../Generic_DS/Queue.h"
#include "Motorcycle.h"
#include "Order.h"
#include "../Defs.h"
#include "Restaurant.h"

class Region
{
private:
	REGION region;
	//Orders DS
	PriorityQueue<Order*> VIPOrders;
	Queue<Order*> NormalOrders;
	Queue<Order*> FrozenOrders;
	Queue<Order*> inService;
	int totalOrders;
	struct activeOrders {
		int ID;
		char TYPE;
	};
	//Order counts
	int nOrderCount;
	int fOrderCount;
	int vOrderCount;
	activeOrders orderInRegion[100];
	//Motorcycles DS
	ArrayList<Motorcycle> fastMotos;
	ArrayList<Motorcycle> normalMotos;
	ArrayList<Motorcycle> frozenMotos;
	int fstCount;
	int norCount;
	int frzCount;
	//Speeds
	int fastSpeed;
	int normalSpeed;
	int frozenSpeed;
	//auto promotion
	int autoPromotion;
public:
	Region(int fc, int nc, int fzc, int ns, int fss, int fzs, REGION R, int autoP);
	bool HasOrder(int id);
	void AddOrder(Order* pOrd);
	bool RemoveOrder(int id);
	void assign(Restaurant* pRest);
};

#endif 