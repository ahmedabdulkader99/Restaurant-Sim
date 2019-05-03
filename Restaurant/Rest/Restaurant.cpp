#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"
#include "Region.h"

Restaurant::Restaurant()
	:activeCount(0)
{
	pGUI = NULL;

	ifstream inFile("inputfile.txt");

	int son, sof, sov;
	inFile >> son >> sof >> sov;
	int nomnA, nomnB, nomnC, nomnD, nomfA, nomfB, nomfC, nomfD, nomvA, nomvB, nomvC, nomvD;
	inFile >> nomnA >> nomfA >> nomvA >> nomnB >> nomfB >> nomvB >> nomnC >> nomfC >> nomvC >> nomnD >> nomfD >> nomvD;
	int autopromotionlimit;
	inFile >> autopromotionlimit;
	inFile >> eventsCount;

	normalMotoCount = nomnA + nomnB + nomnC + nomnD;
	VIPMotoCount = nomvA + nomvB + nomvC + nomvD;
	frozenMotoCount = nomfA + nomfB + nomfC + nomfD;
	
	totalMotoCount = normalMotoCount + frozenMotoCount + VIPMotoCount;

	region[0] = new Region(nomvA, nomnA, nomfA, son, sov, sof, A_REG, autopromotionlimit);
	region[1] = new Region(nomvB, nomnB, nomfB, son, sov, sof, B_REG, autopromotionlimit);
	region[2] = new Region(nomvC, nomnC, nomfC, son, sov, sof, C_REG, autopromotionlimit);
	region[3] = new Region(nomvD, nomnD, nomfD, son, sov, sof, D_REG, autopromotionlimit);

	//not noevents but nof arrival events
	for (int i = 0; i < eventsCount; i++)
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


void Restaurant::AddOrder(Order* pOrd)
{
	activeCount++;
	switch (pOrd->GetRegion())
	{
	case(A_REG):
		region[0]->AddOrder(pOrd);
		break;
	case(B_REG):
		region[1]->AddOrder(pOrd);
		break;
	case(C_REG):
		region[2]->AddOrder(pOrd);
		break;
	case(D_REG):
		region[3]->AddOrder(pOrd);
		break;
	}
}

void Restaurant::RemoveOrder(int id)
{
	activeCount--;
	pGUI->RemoveOrderFromDrawing(id);
	for (int i = 0; i < 4; i++) {
		if (region[i]->HasOrder(id)) {
			region[i]->RemoveOrder(id);
			return;
		}
	}
}

void Restaurant::DecrementCount()
{
	activeCount--;
}

void Restaurant::PrintOrder(Order* pOrd)
{
	pGUI->AddOrderForDrawing(pOrd);
}

void Restaurant::unPrintOrder(int id)
{
	pGUI->RemoveOrderFromDrawing(id);
}

void Restaurant::promoteOrder(int id, int exm)
{
	for (int i = 0; i < 4; i++) {
		if (region[i]->HasOrder(id)) {
			region[i]->PromoteOrder(id, exm);
			return;
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
		Interactive();
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	};

}

void Restaurant::Interactive()
{
	int CurrentTimeStep = 0;
	while (!EventsQueue.isEmpty() || activeCount>0)
	{

		for (int i = 0; i < 4; i++) {
			region[i]->assign(this, CurrentTimeStep);
		}

		ExecuteEvents(CurrentTimeStep);




		//print current timestep and extra details
		char msg[300] = { 0 };
		char timestep[5];
		char active[5];
		char fmoto[5];
		char vmoto[5];
		char nmoto[5];
		char counts[5];
		int TotalCount = 0;
		for (int i = 0; i < 4; i++) {
			TotalCount += region[i]->GetWaitingOrders();
		}
		itoa(CurrentTimeStep, timestep, 10);
		itoa(TotalCount, active, 10);
		itoa(frozenMotoCount, fmoto, 10);
		itoa(VIPMotoCount, vmoto, 10);
		itoa(normalMotoCount, nmoto, 10);
		strcat(msg, "Time: ");
		strcat(msg, timestep);
		strcat(msg, " // Active Orders: ");
		strcat(msg, active);
		strcat(msg, " == | A:");
		itoa(region[0]->GetWaitingOrders(),counts , 10);
		strcat(msg, counts);
		strcat(msg, " | B:");
		itoa(region[1]->GetWaitingOrders(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | C:");
		itoa(region[2]->GetWaitingOrders(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | D:");
		itoa(region[3]->GetWaitingOrders(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | // Normal Motos: ");
		strcat(msg, " A:");
		itoa(region[0]->GetNMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | B:");
		itoa(region[1]->GetNMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | C:");
		itoa(region[2]->GetNMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | D:");
		itoa(region[3]->GetNMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " // Frozen Motos: ");
		strcat(msg, " A:");
		itoa(region[0]->GetFMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | B:");
		itoa(region[1]->GetFMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | C:");
		itoa(region[2]->GetFMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | D:");
		itoa(region[3]->GetFMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " // Fast Motos: ");
		strcat(msg, " A:");
		itoa(region[0]->GetVMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | B:");
		itoa(region[1]->GetVMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | C:");
		itoa(region[2]->GetVMotoCount(), counts, 10);
		strcat(msg, counts);
		strcat(msg, " | D:");
		itoa(region[3]->GetVMotoCount(), counts, 10);
		strcat(msg, counts);

		pGUI->PrintMessage(msg);

		pGUI->UpdateInterface();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
	}
	pGUI->PrintMessage("Generated and assigned all orders ! Click to exit...");
	pGUI->waitForClick();
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
	//
	////
	//// THIS IS JUST A DEMO FUNCTION
	//// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2
	//
	//int EventCnt;	
	//Order* pOrd;
	//Event* pEv;
	//srand(time(NULL));

	//pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
	//EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

	//pGUI->UpdateInterface();

	//pGUI->PrintMessage("Generating orders randomly... In next phases, orders should be loaded from a file");
	//	
	//int EvTime = 0;
	//
	////Create Random events
	////All generated event will be "ArrivalEvents" for the demo
	//for(int i=0; i<EventCnt; i++)
	//{
	//	int O_id = i+1;
	//	
	//	//Rendomize order type
	//	int OType;
	//	if(i<EventCnt*0.2)	//let 1st 20% of orders be VIP (just for sake of demo)
	//		OType = TYPE_VIP;
	//	else if(i<EventCnt*0.5)	
	//		OType = TYPE_FROZ;	//let next 30% be Frozen
	//	else
	//		OType = TYPE_NRM;	//let the rest be normal

	//	
	//	int reg = rand()% REG_CNT;	//randomize region

	//	int dist = 0;
	//	int money = 0;

	//	//Randomize event time
	//	EvTime += rand()%4;
	//	pEv = new ArrivalEvent(EvTime, O_id, dist, (ORD_TYPE)OType, (REGION)reg, money);
	//	AddEvent(pEv);

	//}	



	 
