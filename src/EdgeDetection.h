/*
 * EdgeDetection.h
 *
 *  Created on: Jan 14, 2016
 *      Author: feds
 */

#ifndef SRC_EDGEDETECTION_H_
#define SRC_EDGEDETECTION_H_

class EdgeDetection {
public:
	EdgeDetection(bool inital);
	void update(bool state);

	bool isEdge();
	bool isRising();
	bool isFalling();
	bool getState();

private:
	bool lastState;
	bool thisState;
};

#endif /* SRC_EDGEDETECTION_H_ */
