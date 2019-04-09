#include "CancellationEvent.h"
#include "..\Rest\Restaurant.h"
#include "..\Rest\Region.h"

CancellationEvent::CancellationEvent(int eTime, int oID)
	:Event(eTime,oID)
{
}

void CancellationEvent::Execute(Restaurant* pRest)
{
	pRest->Remove(OrderID);
}
