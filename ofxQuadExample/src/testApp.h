#pragma once

#include "ofMain.h"
#include "ofxQuad.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void save();
        void loadMe();    
        ofxQuad** mapping;
        ofDirectory dir;
        vector <ofVideoPlayer> vid;
    ofTexture toQuad;
        int maxNumber;
        int debug;
        int width ;
        int height;
        int hozpertion;
        int load;
        int swapGloble;
        int w;
        int h;

};
