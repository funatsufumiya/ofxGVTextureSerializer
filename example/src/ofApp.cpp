#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofLogToConsole();

	fbo.allocate(512, 512, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){
    fbo.begin();
	ofClear(0);
	// draw rotating triangle
	ofPushMatrix();
	ofTranslate(fbo.getWidth() / 2, fbo.getHeight() / 2);
	ofRotateDeg(ofGetElapsedTimef() * 30);
	ofDrawTriangle(0, -100, 87, 100, -87, 100);
	ofPopMatrix();
	fbo.end();

	buffer = serializer.serializeTexture(fbo.getTexture());
	tex = serializer.deserialize(buffer);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255);
	fbo.draw(0, 0);
	tex.draw(0, 512);
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

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
