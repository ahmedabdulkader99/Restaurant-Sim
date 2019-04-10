#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
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
	int eventsCount;
	int activeCount;
	int totalMotoCount;
	int VIPMotoCount;
	int normalMotoCount;
	int frozenMotoCount;
	
	//
	// TODO: Add More Data Members As Needed
	//

	

public:
	
	Restaurant();
	~Restaurant();
	void AddEvent(Event* pE);	//adds a new event to the queue of events
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	void Interactive();

	//Implementation funcs
	void AddOrder(Order* pOrd);
	void Remove(int id);
	void PrintOrder(Order* pOrd);
	void unPrintOrder(int id);

};

#endif