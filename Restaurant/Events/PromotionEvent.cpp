#include "PromotionEvent.h"

PromotionEvent::PromotionEvent(int eTime, int oID, int ExM)
	:Event(eTime,oID), ExMon(ExM)
{
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->promoteOrder(OrderID, ExMon);
}
