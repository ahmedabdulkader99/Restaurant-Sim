#include "Region.h"
#include "../Defs.h"

Region::Region(int fc, int nc, int fzc, int ns, int fss, int fzs, REGION R, int autoP)
	:totalOrders(0), fstCount(fc), norCount(nc), frzCount(fzc),
	fastSpeed(fss), normalSpeed(ns), frozenSpeed(fzs),
	VIPOrders(), NormalOrders(), FrozenOrders(),
	region(R), autoPromotion(autoP),
	nOrderCount(0), fOrderCount(0), vOrderCount(0),
	inService(), waitingOrders(0)
{
	for (int i(1); i <= nc; i++)
	{
		Motorcycle M(TYPE_NRM, ns, R);
		normalMotos.insert(i, M);
	}
	for (int i(1); i <= fzc; i++)
	{
		Motorcycle M(TYPE_FROZ, fzs, R);
		frozenMotos.insert(i, M);
	}
	for (int i(1); i <= fc; i++)
	{
		Motorcycle M(TYPE_VIP, fss, R);
		fastMotos.insert(i, M);
	}
}

bool Region::HasOrder(int id)
{
	for (int i = 0; i < totalOrders; i++) {
		if (id == orderInRegion[i].ID)
			return true;
	}
	return false;
}

int Region::GetWaitingOrders()
{
	return waitingOrders;
}

int Region::GetNMotoCount()
{
	return norCount;
}

int Region::GetFMotoCount()
{
	return frzCount;
}

int Region::GetVMotoCount()
{
	return fstCount;
}

void Region::AddOrder(Order * pOrd)
{
	orderInRegion[totalOrders].ID = pOrd->GetID();
	switch (pOrd->GetType()) 
	{
	case (TYPE_NRM):
		NormalOrders.insert(1, pOrd);
		orderInRegion[totalOrders].TYPE = 'N';
		nOrderCount++;
		break;
	case(TYPE_FROZ):
		FrozenOrders.enqueue(pOrd);
		orderInRegion[totalOrders].TYPE = 'F';
		fOrderCount++;
		break;
	case(TYPE_VIP):
		VIPOrders.enqueue(pOrd);
		orderInRegion[totalOrders].TYPE = 'V';
		vOrderCount++;
		break;
	}
	totalOrders++;
	waitingOrders++;
}

bool Region::RemoveOrder(int id)
{
	for (int i = 0; i < totalOrders; i++) 
	{
		if (id == orderInRegion[i].ID) 
		{
			if (orderInRegion[i].TYPE == 'N') {
			
				Order* npOrd;
				for (int i(1); i <= nOrderCount; i++)
				{
					npOrd = NormalOrders.getEntry(i);
					if (npOrd->GetID() == id) {
						delete npOrd;
						NormalOrders.remove(i);
						nOrderCount--;
						waitingOrders--;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Region::PromoteOrder(int id, int exm)
{
	for (int i = 0; i < totalOrders; i++)
	{
		if (id == orderInRegion[i].ID)
		{			
			if (orderInRegion[i].TYPE == 'N') {

				Order* npOrd;
				for (int i(1); i <= nOrderCount; i++)
				{
					npOrd = NormalOrders.getEntry(i);
					if (npOrd->GetID() == id) {
						npOrd->promote(exm);
						NormalOrders.remove(i);
						VIPOrders.enqueue(npOrd);
						nOrderCount--;
						vOrderCount++;
						return true;
					}
				}
			}
		}
	}
	return false;
}


void Region::assign(Restaurant* pRest)
{
	Order* pOrd;
	if (vOrderCount > 0) {
		pOrd = VIPOrders.peek();
		VIPOrders.dequeue();
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		pRest->DecrementCount();
		vOrderCount--;
		waitingOrders--;
	}

	if (fOrderCount > 0) {
		FrozenOrders.dequeue(pOrd);
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		pRest->DecrementCount();
		fOrderCount--;
		waitingOrders--;
	}
	if (nOrderCount > 0) {
		pOrd = NormalOrders.getEntry(nOrderCount);
		NormalOrders.remove(nOrderCount);
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		pRest->DecrementCount();
		nOrderCount--;
		waitingOrders--;
	}
}
