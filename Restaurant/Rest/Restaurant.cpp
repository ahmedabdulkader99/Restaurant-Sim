#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"


Restaurant::Restaurant() 
{
	pGUI = NULL;

	ifstream inFile("inputfile.txt");

	int son, sof, sov;
	inFile >> son >> sof >> sov;
	int nomnA, nomnB, nomnC, nomnD, nomfA, nomfB, nomfC, nomfD, nomvA, nomvB, nomvC, nomvD;
	inFile >> nomnA >> nomfA >> nomvA >> nomnB >> nomfB >> nomvB >> nomnC >> nomfC >> nomvC >> nomnD >> nomfD >> nomvD;
	int autopromotionlimit;
	inFile >> autopromotionlimit;
	int noofevents;
	inFile >> noofevents;

	region[0] = new Region(nomvA, nomnA, nomfA, son, sov, sof, A_REG, autopromotionlimit);
	region[1] = new Region(nomvB, nomnB, nomfB, son, sov, sof, B_REG, autopromotionlimit);
	region[2] = new Region(nomvC, nomnC, nomfC, son, sov, sof, C_REG, autopromotionlimit);
	region[3] = new Region(nomvD, nomnD, nomfD, son, sov, sof, D_REG, autopromotionlimit);

	//not noevents but nof arrival events
	for (int i = 0; i < noofevents; i++)
	{
		char s;
		inFile >> s;
		if (s == 'R')
		{
			int id, arrivaltime, money, dist;
			char reg, type;
			REGION reg1;
			ORD_TYPE type1;
			inFile >> arrivaltime >> type >> id >> dist >> money >> reg;
			if (reg == 'A')
			{
				reg1 = A_REG;
			}
			else if (reg == 'B')
			{
				reg1 = B_REG;
			}
			else if (reg == 'C')
			{
				reg1 = C_REG;
			}
			else if (reg == 'D')
			{
				reg1 = D_REG;
			}
			if (type == 'N')
			{
				type1 = TYPE_NRM;
			}
			else if (type == 'F')
			{
				type1 = TYPE_FROZ;
			}
			else if (type == 'V')
			{
				type1 = TYPE_VIP;
			}

			Event* E = new ArrivalEvent(arrivaltime, id, dist, type1, reg1, money);
			EventsQueue.enqueue(E);

		}
		if (s == 'X')
		{
			int id, arrivaltime;
			inFile >> arrivaltime >> id;
			Event* E = new CancellationEvent(arrivaltime, id);
			EventsQueue.enqueue(E);

		}
		if (s == 'P')
		{
			int id, arrivaltime, ExtraMoney;
			inFile >> arrivaltime >> id >> ExtraMoney;
			Event* E = new PromotionEvent(arrivaltime, id, ExtraMoney);
			EventsQueue.enqueue(E);

		}
	}
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
		
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		Just_A_Demo();

	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////
void Restaurant::AddEvent(Event* pE)	//adds a new event to the queue of events
{
	EventsQueue.enqueue(pE);
}

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current time
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
		delete pGUI;
}




////////////////////////////////////////////////////////////////////////////////
/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
void Restaurant::Just_A_Demo()
{
	
	//
	// THIS IS JUST A DEMO FUNCTION
	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	
	int EventCnt;	
	Order* pOrd;
	Event* pEv;
	srand(time(NULL));

	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	pGUI->UpdateInterface();

	pGUI->PrintMessage("Generating orders randomly... In next phases, orders should be loaded from a file");
		
	int EvTime = 0;
	
	//Create Random events
	//All generated event will be "ArrivalEvents" for the demo
	for(int i=0; i<EventCnt; i++)
	{
		int O_id = i+1;
		
		//Rendomize order type
		int OType;
		if(i<EventCnt*0.2)	//let 1st 20% of orders be VIP (just for sake of demo)
			OType = TYPE_VIP;
		else if(i<EventCnt*0.5)	
			OType = TYPE_FROZ;	//let next 30% be Frozen
		else
			OType = TYPE_NRM;	//let the rest be normal

		
		int reg = rand()% REG_CNT;	//randomize region

		int dist = 0;
		int money = 0;

		//Randomize event time
		EvTime += rand()%4;
		pEv = new ArrivalEvent(EvTime, O_id, dist, (ORD_TYPE)OType, (REGION)reg, money);
		AddEvent(pEv);

	}	

	int CurrentTimeStep = 1;
	//as long as events queue is not empty yet
	while(!EventsQueue.isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep,timestep,10);	
		pGUI->PrintMessage(timestep);


		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		//The above line may add new orders to the DEMO_Queue

		//Let's draw all arrived orders by passing them to the GUI to draw

		while(DEMO_Queue.dequeue(pOrd))
		{
			pGUI->AddOrderForDrawing(pOrd);
			pGUI->UpdateInterface();
		}
		Sleep(1000);
		CurrentTimeStep++;	//advance timestep
	}


	pGUI->PrintMessage("generation done, click to END program");
	pGUI->waitForClick();

	
}
////////////////

void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}

Order* Restaurant::getDemoOrder()
{
	Order* pOrd;
	DEMO_Queue.dequeue(pOrd);
	return pOrd;

}


/// ==> end of DEMO-related function