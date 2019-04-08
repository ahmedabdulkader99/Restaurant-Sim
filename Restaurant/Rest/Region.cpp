#include "Region.h"
#include "../Defs.h"

Region::Region(int fc, int nc, int fzc, int ns, int fss, int fzs, REGION R)
	:totalOrders(0), fstCount(fc), norCount(nc), frzCount(fzc), 
	 fastSpeed(fss), normalSpeed(ns), frozenSpeed(fzs),
	 VIPOrders(), NormalOrders(), FrozenOrders(),
	 region(R)
{
	for (int i(1); i <= nc; i++)
	{
		Motorcycle M(TYPE_NRM, ns, R);
		normalMotos.insert(i, M);
	}
	for (int i(1); i <= fc; i++)
	{
		Motorcycle M(TYPE_VIP, fss, R);
		normalMotos.insert(i, M);
	}
	for (int i(1); i <= fzc; i++)
	{
		Motorcycle M(TYPE_FROZ, fzs, R);
		normalMotos.insert(i, M);
	}
}