#include "ofApp.h"

/* ---------------------------------------------------------------
 
 * VCD Demo. To be used with an arduino equipped with an ethernet shield.
 * 
 * Datastructure is very simple: 32 bytes sent to 192.168.0.177:8888
 * Each Byte defines the intensity of one LCD.
 *
 * 
 
 */

//--------------------------------------------------------------
void ofApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	udpConnection.Create();
	udpConnection.Connect("192.168.0.177",8888);
	udpConnection.SetNonBlocking(true);

    lastInteraction = 0;
    memset(lcdValues, 0, 32);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    // do some flickering if there was no interaction for 5 seconds
    if( lastInteraction + 5000 < ofGetElapsedTimeMillis() ){
        memset(lcdValues, 0, 32);
        
        // tickering through all available LCDs
        int lcd = (int)(ofGetFrameNum()/10)%32;
        lcdValues[lcd] = 0xff;
        
        // First LCD is blinking
        int t = ofGetElapsedTimeMillis()/1000;
        lcdValues[0] = t % 2 == 0 ? 0xff : 0;
        
        udpConnection.Send(lcdValues,32);
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(120, 120, 120);
	for(unsigned int i=1;i<stroke.size();i++){
		ofLine(stroke[i-1].x,stroke[i-1].y,stroke[i].x,stroke[i].y);
	}
    
    ofSetColor(20, 20, 20);
    ofDrawBitmapString("VSE Demo ", 15, 30);
    if( lastInteraction + 5000 < ofGetElapsedTimeMillis() ){
        int step = (int)(ofGetFrameNum()/10)%32;
        ofDrawBitmapString("NON INTERACTION STEP THROUGH ("+ofToString(step)+")", 15, 50);
    }else{
        ofDrawBitmapString("INTERACTIVE MODE - DRAG MOUSE ON THE X-AXIS", 15, 50);
    }
    
    int border = 50;
    int sqWidth = (ofGetWindowWidth() - border*2)/16;
    int px, py;
	for(unsigned int i=0;i<32;i++){
        ofSetColor( 255 -   (unsigned char) lcdValues[i] );
        px = border+sqWidth*(i%16) - 5;
        if( (i%16) > 7 ) px += 10;
        py = ofGetWindowHeight()/2 - (i>15?sqWidth:0);
        ofRect( px, py, sqWidth-1, sqWidth-1);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    lastInteraction = ofGetElapsedTimeMillis();
    int pos = (int)ofMap( x, 0, ofGetWindowWidth(), 0, 31);
    for(int i=0;i<32;i++){
        lcdValues[i] = (int)ofMap( abs(pos-i), 0, 32, 0, 255);
    }
    udpConnection.Send(lcdValues,32);
	stroke.push_back(ofPoint(x,y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	stroke.clear();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
