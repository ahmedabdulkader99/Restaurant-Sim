#include "Motorcycle.h"

int Motorcycle::Mcount = 0;

Motorcycle::Motorcycle()
{
}

Motorcycle::Motorcycle(ORD_TYPE motoType, int s, REGION R)
	:ID(++Mcount), type(motoType), speed(s), region(R), status(IDLE)
{}