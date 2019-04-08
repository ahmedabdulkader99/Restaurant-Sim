#ifndef _PROMOTION_EVENT_h
#define _PROMOTION_EVENT_h

#include "Event.h"


//class for the promotion event
class PromotionEvent : public Event
{
	int ExMon; //Extra Money Paid for promotion
public:
	PromotionEvent(int eTime, int oID, int ExM);
	//Add more constructors if needed

	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif //!_PROMOTION_EVENT_h