#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Events\Event.h"

#include "Order.h"

class Region; //Foreward Declaration

// it is the maestro of the project

class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	Region* region[4];
	struct finishedOrderStr {
		int ID;
		int FT;
		int ST;
		int WT;
		int AT;
		void output();
		bool operator<=(const finishedOrderStr& fOrd);
	};
	PriorityQueue<finishedOrderStr*> fOrderList;
	int eventsCount;
	int finishedOrderCount;
	int inServiceOrderCount;
	int activeCount;
	int totalMotoCount;
	int VIPMotoCount;
	int normalMotoCount;
	int frozenMotoCount;
	int vFinishedCount;
	int nFinishedCount;
	int fFinishedCount;



public:

	Restaurant();
	~Restaurant();
	void AddEvent(Event* pE);	//adds a new event to the queue of events
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void Interactive();
	void StepByStep();
	void Silent();

	//Implementation funcs
	void AddOrder(Order* pOrd);
	void RemoveOrder(int id);
	void AddToFinished(Order* pOrd);
	void DecrementCount();
	void PrintOrder(Order* pOrd);
	void unPrintOrder(int id);
	void promoteOrder(int id, int exm);	
	
};

#endif