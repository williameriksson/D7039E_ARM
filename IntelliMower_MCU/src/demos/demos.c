#include "demos.h"

/*
 * demos.c contains multiple demos intended to test the functionality of the MCU (stm32) alone,
 * without the aid of the Raspberry pi feeding it commands and/or information.
 */

Point currentPos;

Point pointList[] = {
		{.x = 0.0, .y = 0.0},
		{.x = 0.0, .y = -100.0},
		{.x = -100.0, .y = -100.0},
		{.x = -100.0, .y = 0.0},
};

int DemoPointToPoint(int state, int speed) {
	Point cpos = {.x = currentPosition.x , .y = currentPosition.y };
	switch(state) {
	case 0:
		DriveForward(speed);
		InitControlLoop(&pointList[0], &pointList[1], speed);
		return 1;
	case 1:
		if(fabs(cpos.y) >= fabs(pointList[1].y)) {
			StopController();
			double turnAngle = GetTurnAngle(pointList[0], cpos, pointList[1]);
			RotateDegrees(turnAngle, posAngle);
			return 2;
		}
		return 1;
	case 2:
		if(_state == IDLE) {
			InitControlLoop(&pointList[1], &pointList[2], speed);
			return 3;
		}
		return 2;
	case 3:
		if(fabs(cpos.x) >= fabs(pointList[2].x)) {
			StopController();
			double turnAngle = GetTurnAngle(pointList[1], cpos, pointList[2]);
			RotateDegrees(turnAngle, posAngle);
			return 4;
		}
		return 3;
	case 4:
		if(_state == IDLE) {
			InitControlLoop(&pointList[2], &pointList[3], speed);
			return 5;
		}
		return 4;
	case 5:
		if(fabs(cpos.y) <= fabs(pointList[3].y)) {
			StopController();
			double turnAngle = GetTurnAngle(pointList[2], cpos, pointList[3]);
			RotateDegrees(turnAngle, posAngle);
			return 6;
		}
		return 5;
	case 6:
		if(_state == IDLE) {
			InitControlLoop(&pointList[2], &pointList[3], speed);
			return 7;
		}
		return 6;
	case 7:
		if(fabs(cpos.x) <= fabs(pointList[3].x)) {
			DriveForward(0);
		}
	}
	return 1337;
}

int DemoGyroNav(int state, int speed) {
	switch (state) {
	case 0 :
		currentPos.x = 0.0f;
		currentPos.y = 0.0f;
//		DriveForward(speed);
		RotateLeft(2000);
//		AdjustSpeed(10, -10);
		return 1;
	case 1:
		if(abs(rightEncoder.encoderDistance) >= 180.0) {
			return 2;
		}
		break;
	default:
		break;
	}
	return 1337;
}

int demoCount;
double demoAngle;

int DemoSquare(int state, int speed) {
	if(demoCount >= 4) {
		DriveForward(0);
		return 1337;
	}
	if(state == 0) {
//		gyro.yaw = 0.0f;
		RotateLeft(speed);
//		demoAngle = 0.0;
		return 1;
	}
	else if(abs((GetPosAngle()) - demoAngle) >= 90.0 && state == 1) {
		DriveForward(speed);
		demoAngle += 90.0;
		rightEncoder.encoderDistance = 0.0f;
		return 2;
	}
	else if(abs(rightEncoder.encoderDistance) >= 100.0f && state == 2) {
		demoCount++;
		return 0;
	}
	return state;
}

int DemoForward(int state, int speed, float distance) {
	if(abs(rightEncoder.encoderDistance) >= 100.0f) {
		DriveForward(speed);
	}
	return 0;
}

int DemoRotate(int state, int speed, int rotations) {
	switch(state) {
	case 0:
		RotateLeft(speed);
		return 1;
	case 1:
		break;
	}
	return 1337;
}

Point start = {.x = 0.0, .y = 0.0};
Point target = {.x = 0.0, .y = 200.0};
Point curPoss;
int DemoEncoderNav(int state, int speed) {
	switch(state) {
	case 0:
		curPoss.x = 0.0;
		curPoss.y = 0.0;
		InitControlLoop(&start, &target, speed);
		return 1;
	case 1:
		curPoss.x = currentPosition.x;
		curPoss.y = currentPosition.y;
		UpdatePIDValue(&curPoss);
		if(abs(curPoss.y) >= target.y) {
			StopController();
			DriveForward(0);
			return 2;
		}
		return 1;
	default:
		return 1337;
	}
}

int DemoCurve(int state, int speed, float distance) {
	switch(state) {
	case 0:
		DriveForward(speed);
//		AdjustSpeed(-10, 10);
		return 1;
	case 1:
		if(leftEncoder.encoderDistance >= 100.0) {
			DriveForward(0);
			return 2;
		}
		return 1;
	case 2:
		return 2;
	}
	return 1337;
}

int DemoAvoidance(int state, int speed) {
	if(state == 0) {
		DriveForward(speed);
		return 1;
	}
	else if(state == 1 && rightEncoder.encoderDistance >= 100.0f) {
		gyro.yaw = 0.0f;
		RotateLeft(speed);
		return 2;
	}
	else if(state == 2 && abs(gyro.yaw) >= 87.0f) {
		rightEncoder.encoderDistance = 0.0f;
		DriveForward(speed);
		return 3;
	}
	else if(state == 3 && rightEncoder.encoderDistance >= 100.0f) {
		gyro.yaw = 0.0f;
		RotateRight(speed);
		return 4;
	}
	else if(state == 4 && abs(gyro.yaw) >= 87.0f) {
		rightEncoder.encoderDistance = 0.0f;
		DriveForward(speed);
		return 5;
	}
	else if(state == 5 && rightEncoder.encoderDistance >= 200.0f) {
		gyro.yaw = 0.0f;
		RotateRight(speed);
		return 6;
	}
	else if(state == 6 && abs(gyro.yaw) >= 87.0f) {
		rightEncoder.encoderDistance = 0.0f;
		DriveForward(speed);
		return 7;
	}
	else if(state == 7 && rightEncoder.encoderDistance >= 50.0f) {
		DriveForward(0);
	}

	return state;
}
