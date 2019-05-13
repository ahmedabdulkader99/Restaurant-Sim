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
	:activeCount(0), finishedOrderCount(0), inServiceOrderCount(0),
	vFinishedCount(0), nFinishedCount(0), fFinishedCount(0)
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
	for (int i = 0; i < 4; i++) {
		if (region[i]->HasOrder(id)) {
			region[i]->RemoveOrder(id, this);
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

	switch (pOrd->GetType())
	{
	case (TYPE_NRM):
		nFinishedCount++;
		break;
	case(TYPE_FROZ):
		fFinishedCount++;
		break;
	case(TYPE_VIP):
		vFinishedCount++;
		break;
	}

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
		StepByStep();
		break;
	case MODE_SLNT:
		Silent();
		break;
	};

}

//Interactive Sim

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
		{
			string msg;
			string timestep;
			string active;
			string counts[4];
			int NumCounts[4];
			int TotalCount = 0;
			for (int i = 0; i < 4; i++) {
				TotalCount += region[i]->GetWaitingOrders();
				NumCounts[i] = region[i]->GetWaitingOrders();
				counts[i] = to_string(NumCounts[i]);
			}
			timestep = to_string(CurrentTimeStep);
			active = to_string(activeCount);
			string time = "Time: " + timestep;
			string ActiveCnt = " // Active Orders: " + active + " ==";
			string Counts;
			for (int i = 0; i < 4; i++) {
				char R = 65 + i;
				string af = ":";
				string bf = " | ";
				Counts += (bf + R + af + counts[i]);
			}

			string finishedCnt = " // Finished Orders: ";
			finishedCnt += ("N: " + to_string(nFinishedCount) + " | ");
			finishedCnt += ("F: " + to_string(fFinishedCount) + " | ");
			finishedCnt += ("V: " + to_string(vFinishedCount));

			Queue<Motorcycle*> M;
			string TOTALMOTO;
			int Rcount(0);
			for (int i = 0; i < 4; i++) {
				M = region[i]->getLastAssigned();
				Motorcycle* pMoto;
				string assignedMoto;
				int oId, mId, Mcount(0);
				char oType, mType;
				while (!M.isEmpty()) 
				{
					Rcount++;
					M.dequeue(pMoto);
					pGUI->AddMotoToRoad(pMoto);
					oId = pMoto->getOrderInfo(oType);
					mId = pMoto->getID();
					mType = pMoto->getType();
					Mcount++;
					assignedMoto += (mType + to_string(mId) + "(" + oType + to_string(oId) + ") ");
				}
				if (Mcount > 0) {
					char r = 65 + i;
					string sep = ": ";
					TOTALMOTO += (r + sep + assignedMoto);
				}

			}
			if (Rcount == 0) {
				TOTALMOTO = "No Motorcycles Assigned at the last timestamp!";
			}
			msg = time + ActiveCnt + Counts + finishedCnt;
			string msg2 = " // Assignment=> " + TOTALMOTO;
			int nOrd[4];
			int fOrd[4];
			int vOrd[4];
			int MnOrd[4];
			int MfOrd[4];
			int MvOrd[4];
			for (int i = 0; i < 4; i++) {
				nOrd[i] = region[i]->getActiveN();
				fOrd[i] = region[i]->getActiveF();
				vOrd[i] = region[i]->getActiveV();
				MnOrd[i] = region[i]->GetAvNMotoCount();
				MfOrd[i] = region[i]->GetAvFMotoCount();
				MvOrd[i] = region[i]->GetAvVMotoCount();
			}

			pGUI->UpdateDrawnCounts(nOrd, fOrd, vOrd, MnOrd, MfOrd, MvOrd);

			pGUI->PrintMessage(msg, msg2);
		}
		pGUI->UpdateInterface();
		pGUI->waitForClick();
		CurrentTimeStep++;	//advance timestep
	}
	pGUI->PrintMessage("Generated and assigned all orders ! Click to Generate Output...");
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
	pGUI->PrintMessage("Generated Output file! Click to exit...");
	pGUI->waitForClick();
}

void Restaurant::StepByStep()
{
	int CurrentTimeStep = 0;
	while (!EventsQueue.isEmpty() || activeCount > 0 || inServiceOrderCount > 0)
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
		{
			string msg;
			string timestep;
			string active;
			string counts[4];
			int NumCounts[4];
			int TotalCount = 0;
			for (int i = 0; i < 4; i++) {
				TotalCount += region[i]->GetWaitingOrders();
				NumCounts[i] = region[i]->GetWaitingOrders();
				counts[i] = to_string(NumCounts[i]);
			}
			timestep = to_string(CurrentTimeStep);
			active = to_string(activeCount);
			string time = "Time: " + timestep;
			string ActiveCnt = " // Active Orders: " + active + " ==";
			string Counts;
			for (int i = 0; i < 4; i++) {
				char R = 65 + i;
				string af = ":";
				string bf = " | ";
				Counts += (bf + R + af + counts[i]);
			}

			string finishedCnt = " // Finished Orders: ";
			finishedCnt += ("N: " + to_string(nFinishedCount) + " | ");
			finishedCnt += ("F: " + to_string(fFinishedCount) + " | ");
			finishedCnt += ("V: " + to_string(vFinishedCount) + " |");

			Queue<Motorcycle*> M;
			string TOTALMOTO;
			int Rcount(0);
			for (int i = 0; i < 4; i++) {
				M = region[i]->getLastAssigned();
				Motorcycle* pMoto;
				string assignedMoto;
				int oId, mId, Mcount(0);
				char oType, mType;
				while (!M.isEmpty())
				{
					Rcount++;
					M.dequeue(pMoto);
					pGUI->AddMotoToRoad(pMoto);
					oId = pMoto->getOrderInfo(oType);
					mId = pMoto->getID();
					mType = pMoto->getType();
					Mcount++;
					assignedMoto += (mType + to_string(mId) + "(" + oType + to_string(oId) + ") ");
				}
				if (Mcount > 0) {
					char r = 65 + i;
					string sep = ": ";
					TOTALMOTO += (r + sep + assignedMoto);
				}

			}
			if (Rcount == 0) {
				TOTALMOTO = "No Motorcycles Assigned at the last timestamp!";
			}
			msg = time + ActiveCnt + Counts + finishedCnt;
			string msg2 = " // Assignment=> " + TOTALMOTO;
			int nOrd[4];
			int fOrd[4];
			int vOrd[4];
			int MnOrd[4];
			int MfOrd[4];
			int MvOrd[4];
			for (int i = 0; i < 4; i++) {
				nOrd[i] = region[i]->getActiveN();
				fOrd[i] = region[i]->getActiveF();
				vOrd[i] = region[i]->getActiveV();
				MnOrd[i] = region[i]->GetAvNMotoCount();
				MfOrd[i] = region[i]->GetAvFMotoCount();
				MvOrd[i] = region[i]->GetAvVMotoCount();
			}

			pGUI->UpdateDrawnCounts(nOrd, fOrd, vOrd, MnOrd, MfOrd, MvOrd);

			pGUI->PrintMessage(msg,msg2);
		}
		pGUI->UpdateInterface();
		Sleep(100);
		CurrentTimeStep++;	//advance timestep
	}
	pGUI->PrintMessage("Generated and assigned all orders ! Click to Generate Output...");
	pGUI->waitForClick();

	//Output
	ofstream out("outputfile.txt");
	{
		out << "FT" << "\t" << "ID" << "\t" << "AT" << "\t" << "WT" << "\t" << "ST" << endl;
		for (int i = 0; i < finishedOrderCount; i++)
		{
			finishedOrderStr* F = fOrderList.peek();
			fOrderList.dequeue();
			out << F->FT << "\t" << F->ID << "\t" << F->AT << "\t" << F->WT << "\t" << F->ST << endl;
		}
		float RestTotalWait(0), RestTotalServ(0);
		int	RestCount(0), RestTotalN(0), RestTotalF(0), RestTotalV(0);
		int RestMotoN(0), RestMotoF(0), RestMotoV(0);
		for (int i = 0; i < 4; i++) {
			char reg = 65 + i;
			out << "Region " << reg << ":" << endl;
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
	pGUI->PrintMessage("Generated Output file! Click to exit...");
	pGUI->waitForClick();
}

void Restaurant::Silent()
{
	int CurrentTimeStep = 0;
	while (!EventsQueue.isEmpty() || activeCount > 0 || inServiceOrderCount > 0)
	{

		for (int i = 0; i < 4; i++) {
			region[i]->updateRegion(this, CurrentTimeStep);
		}

		ExecuteEvents(CurrentTimeStep);

		inServiceOrderCount = 0;
		for (int i = 0; i < 4; i++) {
			inServiceOrderCount += region[i]->GetInServiceOrders();
		}		
		CurrentTimeStep++;	//advance timestep
	}
	pGUI->PrintState();
	pGUI->PrintMessage("Finished Silent Simulation ! Click to Generate Output...");
	pGUI->waitForClick();

	//Output
	ofstream out("outputfile.txt");
	{
		out << "FT" << "\t" << "ID" << "\t" << "AT" << "\t" << "WT" << "\t" << "ST" << endl;
		for (int i = 0; i < finishedOrderCount; i++)
		{
			finishedOrderStr* F = fOrderList.peek();
			fOrderList.dequeue();
			out << F->FT << "\t" << F->ID << "\t" << F->AT << "\t" << F->WT << "\t" << F->ST << endl;
		}
		float RestTotalWait(0), RestTotalServ(0);
		int	RestCount(0), RestTotalN(0), RestTotalF(0), RestTotalV(0);
		int RestMotoN(0), RestMotoF(0), RestMotoV(0);
		for (int i = 0; i < 4; i++) {
			char reg = 65 + i;
			out << "Region " << reg << ":" << endl;
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
	pGUI->PrintMessage("Generated Output file! Click to exit...");
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


/////////////////////////////////// Struct Functions /////////////////////////////////////
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