#include "Region.h"
#include "../Defs.h"

Region::Region(int fc, int nc, int fzc, int ns, int fss, int fzs, REGION R, int autoP)
	:totalOrders(0), fstCount(fc), norCount(nc), frzCount(fzc),
	fastSpeed(fss), normalSpeed(ns), frozenSpeed(fzs),
	VIPOrders(), NormalOrders(), FrozenOrders(),
	region(R), autoPromotion(autoP),
	nOrderCount(0), fOrderCount(0), vOrderCount(0),
	inService()
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

void Region::AddOrder(Order * pOrd)
{
	orderInRegion[totalOrders].ID = pOrd->GetID();
	switch (pOrd->GetType()) 
	{
	case (TYPE_NRM):
		NormalOrders.enqueue(pOrd);
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
}

bool Region::RemoveOrder(int id)
{
	for (int i = 0; i < totalOrders; i++) 
	{
		if (id == orderInRegion[i].ID) 
		{
			Queue<Order*> temp;
			if (orderInRegion[i].TYPE == 'N') {
			
				Order* npOrd;
				for (int i(0); i < nOrderCount; i++) 
				{
					NormalOrders.dequeue(npOrd);
					if (npOrd->GetID() == id)
						delete npOrd;
					else
						temp.enqueue(npOrd);
				}
				while (!temp.isEmpty())
				{
					temp.dequeue(npOrd);
					NormalOrders.enqueue(npOrd);
				}
				return true;
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
		vOrderCount--;
	}

	if (fOrderCount > 0) {
		FrozenOrders.dequeue(pOrd);
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		fOrderCount--;
	}
	if (nOrderCount > 0) {
		NormalOrders.dequeue(pOrd);
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		nOrderCount--;
	}
}
