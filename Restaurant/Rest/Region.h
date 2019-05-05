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
	ArrayList<Order*> NormalOrders;
	Queue<Order*> FrozenOrders;
	ArrayList<Order*> inService;
	Queue<Order*> Finished;
	int totalOrders;
	int waitingOrders;
	struct activeOrders {
		int ID;
		char TYPE;
	};
	//Order counts
	int nOrderCount;
	int fOrderCount;
	int vOrderCount;
	int inServiceCount;
	int finishedCount;
	activeOrders orderInRegion[100];
	//Motorcycles DS
	ArrayList<Motorcycle*> fastMotos;
	ArrayList<Motorcycle*> normalMotos;
	ArrayList<Motorcycle*> frozenMotos;
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
	int GetWaitingOrders();
	int GetNMotoCount();
	int GetFMotoCount();
	int GetVMotoCount();
	void AddOrder(Order* pOrd);
	bool RemoveOrder(int id);
	bool PromoteOrder(int id, int exm);
	void updateRegion(Restaurant* pRest, int timeStep);
	void updateMotorcycles(int T);
	void updateOrders(int T, Restaurant* pRest);
	void checkForAutoPromo(int timeStep);
	bool assign(Order* pOrd, Restaurant* pRest, int timeStep);
	bool getAvailableMotoN(Motorcycle* &pMoto);
	bool getAvailableMotoF(Motorcycle*& pMoto);
	bool getAvailableMotoV(Motorcycle*& pMoto);
};

#endif 