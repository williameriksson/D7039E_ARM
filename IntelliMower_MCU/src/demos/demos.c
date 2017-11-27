#include "demos.h"



/*
 * Runs the mower in a square clockwise
 * @
 */
Point currentPos;

Point pointList[] = {
		{.x = 0.0f, .y = 0.0f},
		{.x = 100.0f, .y = 0.0f},
		{.x = 100.0f, .y = 100.0f},
		{.x = 0.0f, .y = 100.0f},};

Point targets[3];

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
		curPoss.x = xxPos;
		curPoss.y = yyPos;
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
