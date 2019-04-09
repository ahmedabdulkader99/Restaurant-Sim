#include "Region.h"
#include "../Defs.h"

Region::Region(int fc, int nc, int fzc, int ns, int fss, int fzs, REGION R, int autoP)
	:totalOrders(0), fstCount(fc), norCount(nc), frzCount(fzc), 
	 fastSpeed(fss), normalSpeed(ns), frozenSpeed(fzs),
	 VIPOrders(), NormalOrders(), FrozenOrders(),
	 region(R), autoPromotion(autoP)
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
		break;
	case(TYPE_FROZ):
		FrozenOrders.enqueue(pOrd);
		orderInRegion[totalOrders].TYPE = 'F';
		break;
	case(TYPE_VIP):
		VIPOrders.enqueue(pOrd);
		orderInRegion[totalOrders].TYPE = 'V';
		break;
	}
	totalOrders++;
}

bool Region::RemoveOrder(int id)
{
	for (int i = 0; i < totalOrders; i++) {
		if (id == orderInRegion[i].ID) {
			switch (orderInRegion[i].TYPE) {
			case('N'):

			case('F'):

			case('V'):

			}
		}
	}
}
