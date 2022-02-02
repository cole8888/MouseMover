/*
	Cole L - 31st January 2022 - https://github.com/cole8888/MouseMover

	Mouse moving arduio program which can control where it wants to place the cursor rather than just sending
	repeated jitters to the computer. It randomly chooses a target point which it then moves the mouse over to
	by following a cubic bezier curve, where point 1 is the current position of the mouse at the start of the
	movement 

	https://stackoverflow.com/a/67766611/12412451 Was used for generating cubic bezier curves.
*/

#include <AbsMouse.h>

// 1080p
#define SCREEN_X 1920
#define SCREEN_Y 1080

// 1440p 3:2
// #define SCREEN_X 2160
// #define SCREEN_Y 1440

// 1440p 16:9
// #define SCREEN_X 2560
// #define SCREEN_Y 1440

// 4K
// #define SCREEN_X 3840
// #define SCREEN_Y 2160

// Maximum and minimum number of points used when moving the mouse to follow the bezier curve.
#define MOVE_MAX_RESOLUTION 300
#define MOVE_MIN_RESOLUTION 30

// Maximum and minimum delay in ms between finishing moving along an entire curve and starting on a new curve.
#define MOVE_MAX_DELAY 230000
#define MOVE_MIN_DELAY 200000

// Delay in ms between flipping between turning on the RX/TX LEDs to warn of imminent movement.
#define WARNING_LED_DELAY 500

// Number of times to play the warning message on the RX/TX LEDs.
#define WARNING_LED_PLAYS 3

// Offset to use in order to correct for the time the warning animation takes.
#define WARNING_OFFSET WARNING_LED_PLAYS*WARNING_LED_DELAY

// Maximum number of pixels the mouse is allowed to move away from the the current position.
// Must be smaller than the respective SCREEN_X or SCREEN_Y.
#define MOVE_MAX_X 1280
#define MOVE_MAX_Y 720

// Delay interval between sending the movements between each bezier curve point to the computer.
#define INTERVAL 8

#define RXLED 17

// Hold the current position of the mouse.
int curPosX = 0;
int curPosY = 0;

void setup(){
	/*
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		DO NOT REMOVE THIS!
		WITHOUT THIS YOU MAY HAVE A DIFFICULT TIME TRYING TO REPROGRAM YOUR ARDUINO SINCE IT
		WILL START MOVING YOUR MOUSE ALL OVER THE PLACE BEFORE YOU CAN CLICK THE PROGRAM BUTTON
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/
	delay(6000);	// NO TOUCHY!
	/*
		OKAY EVERYTHING BEYOND HERE IS OK TO CHANGE
	*/

	// Disable the TX and RX LEDs since they are annoying.
	// pinMode(LED_BUILTIN_TX,INPUT);
	// pinMode(LED_BUILTIN_RX,INPUT);

	// Provide a seed to the random number generator.
	randomSeed(analogRead(0));

	// Select a random spot to start at.
	curPosX = random(SCREEN_X);
	curPosY = random(SCREEN_Y);
	
	AbsMouse.init(SCREEN_X, SCREEN_Y);
}

// Pick a valid X coordinate for a control point.
int pickValidControlPointX(){
	int x = random(-MOVE_MAX_X, MOVE_MAX_X + 1);
	// If the point we generate is not valid then try again until we find one that works.
	while(x + curPosX > SCREEN_X || x + curPosX < 0){
		x = random(-MOVE_MAX_X, MOVE_MAX_X + 1);
	}
	return x+curPosX;
}

// Pick a valid Y coordinate for a control point.
int pickValidControlPointY(){
	int y = random(-MOVE_MAX_Y, MOVE_MAX_Y + 1);
	// If the point we generate is not valid then try again until we find one that works.
	while(y + curPosY > SCREEN_Y || y + curPosY < 0){
		y = random(-MOVE_MAX_Y, MOVE_MAX_Y + 1);
	}
	return y+curPosY;
}

/*
	Move the cursor to the requested screen position by generating a cubic bezier curve that
	connects these two points and does not touch the edges of the screen.
	Then approximate this curve using a number of points. Each of these points will be a place
	where the mouse will move to, so the fewer points there are, the faster the mouse will move
	to the final position since it will make larger strides per move.
*/
void goToPoint(int x4, int y4){
	// Number of points generated to approximate the bezier curve.
	int resolution = random(MOVE_MIN_RESOLUTION, MOVE_MAX_RESOLUTION+1);

	int x1 = curPosX;	// X1 is the starting X coordinate of the bezier curve, set it to the current position.
	int x2 = pickValidControlPointX();
	int x3 = pickValidControlPointX();

	int y1 = curPosY;	// Y1 is the starting Y coordinate of the bezier curve, set it to the current position.
	int y2 = pickValidControlPointY();
	int y3 = pickValidControlPointY();

	// Generate the points along the bezier curve where the mouse will move to.
	int x = 0;
	int y = 0;
	float step = (float)1/resolution;
	for(int i = 0; i <= resolution; i++){
		// Calculate where this point should be.
		float t = i*step;
		float t2 = t * t;
		float t3 = t * t * t;

		float A = (3 * t2 - 3 * t3);
		float B = (3 * t3 - 6 * t2 + 3 * t);
		float C = (3 * t2 - t3 - 3 * t + 1);

		x = t3 * x4 + A * x3 + B * x2 + C * x1;
		y = t3 * y4 + A * y3 + B * y2 + C * y1;

		AbsMouse.move(x+1, y+1);	// Tell the computer to move the mouse to the generated point.

		delay(INTERVAL);	// Wait before we send the next move.
	}

	// If for some reason the bezier curve does not reach the target point, move over to it.
	// Sometimes it is off by 1, so this fixes that.
	if(x != x4 || y != y4){
		AbsMouse.move(x4+1, y4+1);	// Move the mouse to the intended target point.
	}

	// Let the program know the mouse's current position.
	curPosX = x4;
	curPosY = y4;
}

void loop(){
	// Play the warning message on the TX/RX LEDs to indicate movement is about to happen.
	for(uint8_t i = 0; i < WARNING_LED_PLAYS; i++){
		digitalWrite(RXLED, LOW);
		TXLED0;
		delay(WARNING_LED_DELAY);
		digitalWrite(RXLED, HIGH);
		TXLED1;
		delay(WARNING_LED_DELAY);
	}

	// Generate a random target position.
	int x4 = pickValidControlPointX();
	int y4 = pickValidControlPointY();

	goToPoint(x4, y4);	// Go to the target position.

	delay(random(MOVE_MIN_DELAY - WARNING_OFFSET, (MOVE_MAX_DELAY+1) - WARNING_OFFSET));	// Wait to make the next movement.
}