#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxAutoReloadedShader.h"
#include "ofxGui.h"

// listening port
#define PORT 49162

// max number of strings to display
#define NUM_MSG_STRINGS 20

// demonstrates receiving and processing OSC messages with an ofxOscReceiver,
// use in conjunction with the oscSenderExample
class ofApp : public ofBaseApp{
public:

    void setup();
    void update();
    void draw();

    void keyPressed(int key);

    ofTrueTypeFont font;
    ofxOscReceiver receiver;

    int currentMsgString;
    string msgStrings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];

    float mouseXf = 0;
    float mouseYf = 0;
    float value1 = 0.0f;
    float value2 = 0.0f;

    ofPlanePrimitive plane;
    ofxAutoReloadedShader shader;

    bool debug = false;

    ofxPanel gui;
    ofParameter<float> decayVal1 = {"decayVal1", 0.001, 0.001, 0.015 };
    ofParameter<float> decayVal2 = {"decayVal2", 0.001, 0.001, 0.015 };
};
