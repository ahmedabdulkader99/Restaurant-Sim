#ifndef CANCELLATION_EVENT_h
#define CANCELLATION_EVENT_h

#include "Event.h"


//class for the cancellation event
class CancellationEvent : public Event
{
public:
	CancellationEvent(int eTime, int oID);

	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif // !CANCELLATION_H

