#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"
#include "..\Rest\Region.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, int oDis, ORD_TYPE oType, REGION reg, double oM):Event(eTime, oID)
{
	OrdType = oType;
	OrdRegion = reg;
	OrdDistance = oDis;
	OrdMoney = oM;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create and order and and fills its data 
	// Then adds it to normal, frozen, or VIP order lists that you will create in phase1
	Order* pOrd = new Order(OrderID, OrdType, OrdRegion, OrdDistance, OrdMoney, EventTime);
	pRest->AddOrder(pOrd);
	pRest->PrintOrder(pOrd);
}
