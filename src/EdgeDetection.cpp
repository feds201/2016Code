/*
 * EdgeDetection.cpp
 *
 *  Created on: Jan 14, 2016
 *      Author: feds
 */

#include <EdgeDetection.h>

EdgeDetection::EdgeDetection(bool inital) :
lastState(inital),
thisState(inital)
{
}

void EdgeDetection::update(bool state)
{
	lastState=thisState;
	thisState = state;
}

bool EdgeDetection::isEdge()
{
	return thisState != lastState;
}

bool EdgeDetection::isRising()
{
	return (thisState == true && lastState == false);
}

bool EdgeDetection::isFalling()
{
	return (thisState == false && lastState == true);
}

bool EdgeDetection::getState()
{
	return thisState;
}
