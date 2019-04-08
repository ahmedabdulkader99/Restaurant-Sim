#include "Motorcycle.h"

static int count = 0;

Motorcycle::Motorcycle(ORD_TYPE motoType, int s, REGION R)
	:ID(++count), type(motoType), speed(s), region(R), status(IDLE)
{}