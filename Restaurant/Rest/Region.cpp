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
		Motorcycle* M = new Motorcycle(TYPE_NRM, ns, R);
		normalMotos.insert(i, M);
	}
	for (int i(1); i <= fzc; i++)
	{
		Motorcycle* M = new Motorcycle (TYPE_FROZ, fzs, R);
		frozenMotos.insert(i, M);
	}
	for (int i(1); i <= fc; i++)
	{
		Motorcycle* M = new Motorcycle (TYPE_VIP, fss, R);
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
		NormalOrders.insert(nOrderCount + 1, pOrd);
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


void Region::updateRegion(Restaurant* pRest, int timeStep) //HERE
{
	updateMotorcycles(timeStep);

	Order* pOrd;
	while (vOrderCount > 0) {
		pOrd = VIPOrders.peek();
		if (assign(pOrd, pRest, timeStep)) {
			VIPOrders.dequeue();
			vOrderCount--;
			waitingOrders--;
		}
		else
			break;
	}
	while (fOrderCount > 0) {
		FrozenOrders.peekFront(pOrd);
		if (assign(pOrd, pRest, timeStep)) {
			FrozenOrders.dequeue(pOrd);
			fOrderCount--;
			waitingOrders--;
		}
		else
			break;
	}
	while (nOrderCount > 0) {
		pOrd = NormalOrders.getEntry(1);
		if (assign(pOrd, pRest, timeStep)) {
			NormalOrders.remove(1);
			nOrderCount--;
			waitingOrders--;
		}
		else
			break;
	}

	checkForAutoPromo(timeStep);
}

void Region::updateMotorcycles(int T)
{
	for (int i(1); i < norCount; i++)
	{
		Motorcycle* M = normalMotos.getEntry(i);
		M->checkIfArrived(T);
	}
	for (int i(1); i < frzCount; i++)
	{
		Motorcycle* M = frozenMotos.getEntry(i);
		M->checkIfArrived(T);
	}
	for (int i(1); i < fstCount; i++)
	{
		Motorcycle* M = fastMotos.getEntry(i);
		M->checkIfArrived(T);
	}
}

void Region::checkForAutoPromo(int timeStep)
{
	int Count = nOrderCount;
	int PromoC = 0;
	bool RemoveCheck = false;
	for (int i(1); i <= Count; i++)
	{
		Order* pOrd;
		if (!RemoveCheck)
			pOrd = NormalOrders.getEntry(i);
		else
			PromoC++;
			pOrd = NormalOrders.getEntry(i-PromoC);
		if (timeStep - pOrd->getArrivalTime() > autoPromotion)
		{
			PromoteOrder(pOrd->GetID(), 0);
			RemoveCheck = true;
		}
	}
}

bool Region::assign(Order* pOrd, Restaurant* pRest, int timeStep)
{
	bool assigned = false;
	Motorcycle* pMoto;
	switch (pOrd->GetType())
	{
	case (TYPE_NRM):
		if (getAvailableMotoN(pMoto))
		{
			pOrd->assignOrd(timeStep, normalSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}
		else if (getAvailableMotoV(pMoto))
		{
			pOrd->assignOrd(timeStep, fastSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}			
		break;
	case(TYPE_FROZ):
		if (getAvailableMotoF(pMoto))
		{
			pOrd->assignOrd(timeStep, frozenSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}
		break;
	case(TYPE_VIP):
		if (getAvailableMotoV(pMoto))
		{
			pOrd->assignOrd(timeStep, fastSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}
		else if (getAvailableMotoN(pMoto))
		{
			pOrd->assignOrd(timeStep, normalSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}
		else if (getAvailableMotoF(pMoto))
		{
			pOrd->assignOrd(timeStep, frozenSpeed);
			pMoto->assign(pOrd);
			assigned = true;
		}
		break;
	}
	if (assigned)
	{
		inService.enqueue(pOrd);
		pRest->unPrintOrder(pOrd->GetID());
		pRest->DecrementCount();
	}	
	return assigned;
}

bool Region::getAvailableMotoN(Motorcycle* &pMoto)
{
	for (int i(1); i <= norCount; i++)
	{
		Motorcycle* M = normalMotos.getEntry(i);
		if (M->isIdle())
		{
			pMoto = M;
			return true;
		}		
	}
	return false;
}

bool Region::getAvailableMotoF(Motorcycle*& pMoto)
{
	for (int i(1); i <= frzCount; i++)
	{
		Motorcycle* M = frozenMotos.getEntry(i);
		if (M->isIdle())
		{
			pMoto = M;
			return true;
		}
	}
	return false;
}

bool Region::getAvailableMotoV(Motorcycle*& pMoto)
{
	for (int i(1); i <= fstCount; i++)
	{
		Motorcycle* M = fastMotos.getEntry(i);
		if (M->isIdle())
		{
			pMoto = M;
			return true;
		}
	}
	return false;
}
