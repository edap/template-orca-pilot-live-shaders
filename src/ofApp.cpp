#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetWindowTitle("template orca pilot live shader");
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    // listen on the given port
    ofLog() << "listening for osc messages on port " << PORT;
    receiver.setup(PORT);

    ofDisableArbTex();
    shader.load("shaders/vert.glsl", "shaders/frag.glsl", "");
    plane.set(ofGetWidth(), ofGetHeight(), 2, 2, OF_PRIMITIVE_TRIANGLES);
    plane.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    plane.rotateDeg(180, glm::vec3(0.0f,0.0f,1.0f));

    gui.setup();
    gui.add(decayVal1);
    gui.add(decayVal2);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(value1 > 0.0f){
        value1 -= decayVal1;
    }
    if(value2 > 0.0f){
        value2 -= decayVal2;
    }

    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msgStrings[i] = "";
        }
    }
    // check for waiting messages
    while(receiver.hasWaitingMessages()){

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // check for /a message
        if(m.getAddress() == "/a"){
            value1 = 1.0f;

            // or pass values from orca
            //value1 = getArgAsInt32(0);
            //value1 = getArgAsInt32(1);
        }
        // check for /b message
        else if(m.getAddress() == "/b"){
            value2 = 1.0f;

            // or pass values from orca
            //value2 = m.getArgAsInt32(0);
            //value2 = m.getArgAsInt32(1);
        }
        else{

            // unrecognized message: display on the bottom of the screen
            string msgString;
            msgString = m.getAddress();
            msgString += ":";
            for(size_t i = 0; i < m.getNumArgs(); i++){

                // get the argument type
                msgString += " ";
                msgString += m.getArgTypeName(i);
                msgString += ":";

                // display the argument - make sure we get the right type
                if(m.getArgType(i) == OFXOSC_TYPE_INT32){
                    msgString += ofToString(m.getArgAsInt32(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
                    msgString += ofToString(m.getArgAsFloat(i));
                }
                else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
                    msgString += m.getArgAsString(i);
                }
                else{
                    msgString += "unhandled argument type " + m.getArgTypeName(i);
                }
            }

            // add to the list of strings to display
            msgStrings[currentMsgString] = msgString;
            timers[currentMsgString] = ofGetElapsedTimef() + 5.0f;
            currentMsgString = (currentMsgString + 1) % NUM_MSG_STRINGS;

            // clear the next line
            msgStrings[currentMsgString] = "";
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
//    if (value1 > 0.0f || value1 >0.0f) {
//        ofLog() << value1;
//        ofLog() << value2;
//    }

    if (debug) {
        ofBackgroundGradient(100, 0);
        // draw recent unrecognized messages
        for(int i = 0; i < NUM_MSG_STRINGS; i++){
            ofDrawBitmapStringHighlight(msgStrings[i], 10, 40 + 15 * i);
        }

        string buf = "listening for osc messages on port " + ofToString(PORT);
        ofDrawBitmapStringHighlight(buf, 10, 20);

    } else {
        shader.begin();
        shader.setUniform1f("u_value1", value1);
        shader.setUniform1f("u_value2", value2);
        shader.setUniform2f("u_mouse", mouseX, mouseY);
        shader.setUniform1f("u_time", ofGetElapsedTimef());
        shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
        plane.draw();
        plane.draw();
        shader.end();
    }

    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key=='d'){
        debug = !debug;
    }

}

