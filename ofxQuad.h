#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"


class ofxQuad{
    
public:
    enum CornerLocation{
        TOP_LEFT = 0,
        TOP_RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT
    };
    
    ofxQuad();
    ~ofxQuad();
    void setup(float sw, float sh, float iw,float ih,int width,int height,int _classNumber);
    void update();
    void draw(ofVideoPlayer doit, int sx,int sy,int sw,int sh,int num);
    void draw(ofImage image,int sx,int sy,int sw,int sh,int num);
    void mouseDragged(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	void mouseMoved(ofMouseEventArgs &args);
    void keyPressed(ofKeyEventArgs & args);
    void activate();
	void deactivate();
	bool isActive();
    void save(string fileName);
    void load(string fileName);
    void debugSet(int _debug);
    
    ofVec4f	fromScreenToWarpCoord(float x,float y,float z);
	ofVec4f	fromWarpToScreenCoord(float x,float y,float z);

    void setCorner(CornerLocation cornerLocation, ofPoint screenLocation);
    ofPoint getCorner(CornerLocation cornerLocation);

    void setCornerSensibility(float sensibility);
    float getCornerSensibility();

    ofPoint TLPosition;
    ofPoint TRPosition;
    ofPoint BLPosition;
    ofPoint BRPosition;
    ofPoint src[4];
    ofPoint dst[4];
    GLfloat matrix[16];
    
    ofMesh yes;
    ofMesh no;
    int moveall;

    vector <ofVec2f> newQuad;
    int borderColor;
    bool initialized;
    void gaussian_elimination(float *input, int n);
    void findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]);


    int h;
    int w;
    int redColour;
    ofTrueTypeFont	verdana30;
    ofEvent<CornerLocation> changeEvent;
    int classNumber;
  private:
    
    ofMesh quadMesh;
    bool debug;
    int x, y;
    bool active;
    ofPoint corners[4];
	int whichCorner;
    float cornerSensibility;    
    protected:

};

