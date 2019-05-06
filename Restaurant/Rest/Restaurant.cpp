#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"
#include "Region.h"

Restaurant::Restaurant()
	:activeCount(0), finishedOrderCount(0), inServiceOrderCount(0)
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

void Restaurant::AddToFinished(Order* pOrd)
{
	finishedOrderStr* F = new finishedOrderStr;
	F->AT = pOrd->getArrivalTime();
	F->FT = pOrd->getFinishTime();
	F->ID = pOrd->GetID();
	F->ST = pOrd->getServTime();
	F->WT = F->FT - (F->ST + F->AT);

	finishedOrderCount++;

	fOrderList.enqueue(F);
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
	while (!EventsQueue.isEmpty() || activeCount>0 || inServiceOrderCount>0)
	{

		for (int i = 0; i < 4; i++) {
			region[i]->updateRegion(this, CurrentTimeStep);
		}

		ExecuteEvents(CurrentTimeStep);

		inServiceOrderCount = 0;
		for (int i = 0; i < 4; i++) {
			inServiceOrderCount += region[i]->GetInServiceOrders();
		}


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
		CurrentTimeStep++;	//advance timestep
	}
	pGUI->PrintMessage("Generated and assigned all orders ! Click to exit...");
	pGUI->waitForClick();
	//Output
	
	ofstream out("outputfile.txt");
	{
		out << "FT" << "\t" << "ID" << "\t" << "AT" << "\t" << "WT" << "\t" << "ST" << endl;
		for (int i = 0; i < finishedOrderCount; i++)
		{
			finishedOrderStr* F = fOrderList.peek();
			fOrderList.dequeue();
			out << F->FT << "\t" << F->ID << "\t" << F->AT << "\t" << F->WT << "\t" << F->ST <<endl;
		}
		float RestTotalWait(0), RestTotalServ(0);
		int	RestCount(0), RestTotalN(0), RestTotalF(0), RestTotalV(0);
		int RestMotoN(0), RestMotoF(0), RestMotoV(0);
		for (int i = 0; i < 4; i++) {
			char reg = 65 + i;
			out << "Region " << reg << ":"<< endl;
			float avgwait = float(region[i]->getTotalWait()) / float(region[i]->getCount());
			float avgserv = float(region[i]->getTotalServ()) / float(region[i]->getCount());
			int Norm = region[i]->getTotalNCount();
			int froz = region[i]->getTotalFCount();
			int vip = region[i]->getTotalVCount();
			int orders = Norm + froz + vip;
			int nor = region[i]->getNMotoCount();
			int fst = region[i]->getVMotoCount();
			int frz = region[i]->getFMotoCount();
			int MotorC = nor + fst + frz;

			RestTotalWait += region[i]->getTotalWait(); 
			RestTotalServ += region[i]->getTotalServ(); 
			RestCount += region[i]->getCount(); 
			RestTotalN += Norm; 
			RestTotalF += froz; 
			RestTotalV += vip;
			RestMotoN += nor; 
			RestMotoF += frz; 
			RestMotoV += fst;

			/*Orders: 124 [Norm:100, Froz:15, VIP:9]
			MotorC: 9 [Norm:5, Froz:3, VIP:1]
			Avg Wait = 12.3, Avg Serv = 25.65*/
			out << "\t" << "Orders:" << orders << "[Norm:" << Norm << ", Froz:" << froz << ", VIP: " << vip << "]" << endl;
			out << "\t" << "MotorC:" << MotorC << "[Norm:" << nor << ", Froz:" << frz << ",VIP: " << fst << "]" << endl;
			out << "\t" << "Avg Wait=" << setprecision(3) << avgwait << ", Avg Serv=" << avgserv << endl;
		}

		out << "Total for Restaurant:" << endl;
		out << "\t" << "Orders:" << (RestTotalF + RestTotalN + RestTotalV) << "[Norm:" << RestTotalN << ", Froz:" << RestTotalF << ", VIP: " << RestTotalV << "]" << endl;
		out << "\t" << "MotorC:" << (RestMotoN + RestMotoF + RestMotoV) << "[Norm:" << RestMotoN << ", Froz:" << RestMotoF << ",VIP: " << RestMotoV << "]" << endl;
		out << "\t" << "Avg Wait=" << setprecision(3) << (RestTotalWait / RestCount) << ", Avg Serv=" << (RestTotalServ / RestCount) << endl;
	}
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

void Restaurant::finishedOrderStr::output()
{
	cout << FT << "\t" << ID << "\t" << AT << "\t" << WT << "\t" << ST;
}

bool Restaurant::finishedOrderStr::operator<=(const finishedOrderStr& fOrd) {
	if (FT > fOrd.FT)
		return true;
	else if (FT == fOrd.FT) {
		if (ST > fOrd.ST)
			return true;
	}
	return false;
}


Restaurant::~Restaurant()
{
	delete pGUI;
}




	 
