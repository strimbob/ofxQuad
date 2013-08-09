#include "ofxQuad.h"

ofxQuad::ofxQuad() {
    
    debug = false;
    moveall = 0;
    glPointSize(30);
    quadMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
    ofAddListener(ofEvents().keyPressed, this, &ofxQuad::keyPressed);
    this->initialized = true;
    
}

ofxQuad::~ofxQuad() {
	ofRemoveListener(ofEvents().keyReleased, this, &ofxQuad::keyPressed);
    	this->initialized = false;
}

void ofxQuad::debugSet(int _debug) {
  //  debug = _debug;
}
//--------------------------
void ofxQuad::setup(float sw, float sh, float iw,float ih,int _width,int _height,int _classNumber) {
    classNumber = _classNumber;
    w = _width;
    h = _height;

    initialized = true;

    ofPoint TLPosition = ofPoint(sw, sh);
    ofPoint TRPosition = ofPoint(sw + iw, sh);
    ofPoint BLPosition = ofPoint(sw, sh + ih);
    ofPoint BRPosition = ofPoint(sw + iw, sh + ih);
    
    active=false;
	x=0;
	y=0;
	whichCorner = -1;
    cornerSensibility = 0.003; // rockwall
   // cornerSensibility = 0.09; landmass
    
    setCorner(TOP_LEFT, TLPosition);
    setCorner(TOP_RIGHT, TRPosition);
    setCorner(BOTTOM_LEFT, BLPosition);
    setCorner(BOTTOM_RIGHT, BRPosition);
  
}

bool ofxQuad::isActive(){
	return active;
}
//--------------------------------------------------------------
void ofxQuad::activate(){
	ofRegisterMouseEvents(this);
	active=true;

}
//--------------------------------------------------------------
void ofxQuad::deactivate(){
	ofUnregisterMouseEvents(this);
	active=false;
}

//--------------------------
void ofxQuad::update() {
 
    borderColor = 0xFF6600;

    //we set matrix to the default - 0 translation
    //and 1.0 scale for x y z and w
    for(int i = 0; i < 16; i++){
        if(i % 5 != 0) matrix[i] = 0.0;
        else matrix[i] = 1.0;
    }
    
    //we set the warp coordinates
    //source coordinates as the dimensions of our window
    src[0].x = x;
    src[0].y = y;
    src[1].x = x+w;
    src[1].y = y;
    src[2].x = x+w;
    src[2].y = y+h;
    src[3].x = x;
    src[3].y = y+h;
    
    //corners are in 0.0 - 1.0 range
    //so we scale up so that they are at the window's scale
    for(int i = 0; i < 4; i++){
        dst[i].x = corners[i].x * (float)w;
        dst[i].y = corners[i].y * (float)h;
    }
    
    
    
}
//--------------------------
//void ofxQuad::draw(ofTexture image,int sx,int sy,int sw,int sh){
  void ofxQuad::draw(ofVideoPlayer image,int sx,int sy,int sw,int sh,int num){
      
   //   ofSetColor(255, 255, 255);
    if(moveall == 1){
      if(quadMesh.getNumVertices() < 4){
          quadMesh.draw();
        }
            }
      ofTexture bb;
      bb = image.getTextureReference();
      ofPushMatrix();

    // find transformation matrix
    findHomography(src, dst, matrix);
    
    //finally lets multiply our matrix
   glMultMatrixf(matrix);
      

    bb.drawSubsection(0,0,w,h,sx,sy,sw,sh);
    if(debug){
      
        if(active){
              ofSetColor(redColour, 255, 0,255);
        }
        else{
            ofSetColor(255, 255, 255,255); 
        }
   // ofDrawBitmapString(ofToString(num), sw,sy);
    ofRect(0,0,w,h);
        

 //   verdana30.drawString(ofToString(num), sx,sh+sh);
    
         ofSetColor(0, 0, 0);
    }

    ofPopMatrix();

      if(debug){
          ofPushStyle();
          ofSetColor(255, 255, 255);
          ofSetLineWidth(4);
          ofLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
          ofLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
          ofPopStyle();
      }
}


void ofxQuad::draw(ofImage image,int sx,int sy,int sw,int sh,int num){
    
    //   ofSetColor(255, 255, 255);
    if(moveall == 1){
        if(quadMesh.getNumVertices() < 4){
            quadMesh.draw();
        }
    }
    ofTexture bb;
    bb = image.getTextureReference();
    ofPushMatrix();
    
    // find transformation matrix
    findHomography(src, dst, matrix);
    
    //finally lets multiply our matrix
    glMultMatrixf(matrix);
    
    
    bb.drawSubsection(0,0,w,h,sx,sy,sw,sh);
    if(debug){
        
        if(active){
            ofSetColor(redColour, 255, 0,255);
        }
        else{
            ofSetColor(255, 255, 255,255);
        }
        // ofDrawBitmapString(ofToString(num), sw,sy);
        ofRect(0,0,w,h);
        
        
        //   verdana30.drawString(ofToString(num), sx,sh+sh);
        
        ofSetColor(0, 0, 0);
    }
    
    ofPopMatrix();
    
    if(debug){
        ofPushStyle();
        ofSetColor(255, 255, 255);
        ofSetLineWidth(4);
        ofLine(0, ofGetMouseY(), ofGetWidth(), ofGetMouseY());
        ofLine(ofGetMouseX(), 0, ofGetMouseX(), ofGetHeight());
        ofPopStyle();
    }
}

void ofxQuad::gaussian_elimination(float *input, int n){
    // ported to c from pseudocode in
    // http://en.wikipedia.org/wiki/Gaussian_elimination
    
    float * A = input;
    int i = 0;
    int j = 0;
    int m = n-1;
    while (i < m && j < n){
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++){
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0){
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0;k<n;k++){
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            float A_ij=A[i*n+j];
            for(int k=0;k<n;k++){
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++){
                //subtract A[u,j] * row i from row u
                float A_uj = A[u*n+j];
                for(int k=0;k<n;k++){
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }
            
            i++;
        }
        j++;

    }
    
    //back substitution
    for(int i=m-2;i>=0;i--){
        for(int j=i+1;j<n-1;j++){
            A[i*n+m]-=A[i*n+j]*A[j*n+m];
            //A[i*n+j]=0;
        }
    }
}

void ofxQuad::findHomography(ofPoint src[4], ofPoint dst[4], float homography[16]){
    
    // create the equation system to be solved
    //
    // from: Multiple View Geometry in Computer Vision 2ed
    //       Hartley R. and Zisserman A.
    //
    // x' = xH
    // where H is the homography: a 3 by 3 matrix
    // that transformed to inhomogeneous coordinates for each point
    // gives the following equations for each point:
    //
    // x' * (h31*x + h32*y + h33) = h11*x + h12*y + h13
    // y' * (h31*x + h32*y + h33) = h21*x + h22*y + h23
    //
    // as the homography is scale independent we can let h33 be 1 (indeed any of the terms)
    // so for 4 points we have 8 equations for 8 terms to solve: h11 - h32
    // after ordering the terms it gives the following matrix
    // that can be solved with gaussian elimination:
    
    float P[8][9]={
        {-src[0].x, -src[0].y, -1,   0,   0,  0, src[0].x*dst[0].x, src[0].y*dst[0].x, -dst[0].x }, // h11
        {  0,   0,  0, -src[0].x, -src[0].y, -1, src[0].x*dst[0].y, src[0].y*dst[0].y, -dst[0].y }, // h12
        
        {-src[1].x, -src[1].y, -1,   0,   0,  0, src[1].x*dst[1].x, src[1].y*dst[1].x, -dst[1].x }, // h13
        {  0,   0,  0, -src[1].x, -src[1].y, -1, src[1].x*dst[1].y, src[1].y*dst[1].y, -dst[1].y }, // h21
        
        {-src[2].x, -src[2].y, -1,   0,   0,  0, src[2].x*dst[2].x, src[2].y*dst[2].x, -dst[2].x }, // h22
        {  0,   0,  0, -src[2].x, -src[2].y, -1, src[2].x*dst[2].y, src[2].y*dst[2].y, -dst[2].y }, // h23
        
        {-src[3].x, -src[3].y, -1,   0,   0,  0, src[3].x*dst[3].x, src[3].y*dst[3].x, -dst[3].x }, // h31
        {  0,   0,  0, -src[3].x, -src[3].y, -1, src[3].x*dst[3].y, src[3].y*dst[3].y, -dst[3].y }, // h32
    };
    
    gaussian_elimination(&P[0][0],9);
    
    // gaussian elimination gives the results of the equation system
    // in the last column of the original matrix.
    // opengl needs the transposed 4x4 matrix:
    float aux_H[]={ P[0][8],P[3][8],0,P[6][8], // h11  h21 0 h31
        P[1][8],P[4][8],0,P[7][8], // h12  h22 0 h32
        0      ,      0,0,0,       // 0    0   0 0
        P[2][8],P[5][8],0,1};      // h13  h23 0 h33
    
    for(int i=0;i<16;i++) homography[i] = aux_H[i];
}

void ofxQuad::mouseDragged(ofMouseEventArgs &args){
    
      if(moveall == 1){
 
          
      }
      else{
          
          float scaleX = (float)args.x / w;
          float scaleY = (float)args.y / h;
          if(whichCorner >= 0){
              corners[whichCorner].x = scaleX;
              corners[whichCorner].y = scaleY;
              
              CornerLocation location = (CornerLocation)whichCorner;
              ofNotifyEvent(changeEvent, location, this);
      }
      }
}
//--------------------------------------------------------------
void ofxQuad::mousePressed(ofMouseEventArgs &args){
        
//    if(moveall == 1){
//    if(quadMesh.getNumVertices() == 3){
//        quadMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
//        quadMesh.addVertex(ofVec3f(args.x, args.y , 10));
//        
//        for(int x = 0;x < quadMesh.getNumVertices();x++){
//            ofVec3f temp = quadMesh.getVertex(x);
//            float xb = temp.x/w;
//            float yb = temp.y/h;
//            corners[x].x = xb;
//            corners[x].y = yb;
//        }
//
//    }
//        
//    else if(quadMesh.getNumVertices() == 0){
//        quadMesh.setMode(OF_PRIMITIVE_POINTS);
//       quadMesh.addVertex(ofVec3f(args.x, args.y , 10));
//    }
//    else if(quadMesh.getNumVertices() >= 4){
//        
//        quadMesh.clear();
//    }
//    else{
//        quadMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
//    quadMesh.addVertex(ofVec3f(args.x, args.y , 10));
//    }
//    }
//    else{
    

	float smallestDist = 1.0;
	whichCorner = -1;
	
	for(int i = 0; i < 4; i++){
		float distx = corners[i].x - (float)args.x/w;
		float disty = corners[i].y - (float)args.y/h;
		float dist  = sqrt( distx * distx + disty * disty);
		
		if(dist < smallestDist && dist < cornerSensibility){
			whichCorner = i;
			smallestDist = dist;
            cout << classNumber << endl;
//		}
	}
	}
}
//--------------------------------------------------------------
void ofxQuad::mouseReleased(ofMouseEventArgs &args){
	whichCorner = -1;
}


//--------------------------------------------------------------
void ofxQuad::mouseMoved(ofMouseEventArgs &args){}
//--------------------------------------------------------------
void ofxQuad::keyPressed(ofKeyEventArgs & args){
    if(args.key == ' '){
        debug = !debug;
    }
    if(debug){
        ofShowCursor();
    }else{
        ofHideCursor();
    }
    if(args.key == 'z'){
        
        if(moveall == 1){
            moveall = 0;
            redColour = 0;
        }
        else {
            moveall = 1;
        //   quadMesh.clear();
            redColour = 255;
        }
    }
}
//--------------------------------------------------------------
ofVec4f ofxQuad::fromScreenToWarpCoord(float x, float y, float z)
{
	ofVec4f mousePoint;
	ofVec4f warpedPoint;
	
	// this is the point on the image which i want to know the coordinates inside the warped system ...
	mousePoint.x = x;
	mousePoint.y = y;
	mousePoint.z = 0.0;
	mousePoint.w = 1.0;
	
	// i create a ofMatrix4x4 with the ofxGLWarper myMatrixData in column order
	ofMatrix4x4 myOFmatrix = ofMatrix4x4(matrix[0], matrix[4],matrix[8],matrix[12],
										 matrix[1], matrix[5],matrix[9], matrix[13],
										 matrix[2], matrix[6],matrix[10],matrix[14],
										 matrix[3],matrix[7],matrix[11],matrix[15]);
	// do not invert the matrix
	ofMatrix4x4 invertedMyMatrix = myOFmatrix.getInverse();
	//ofMatrix4x4 invertedMyMatrix = myOFmatrix;
	
	// multiply both to get the point transformed by the matrix
	warpedPoint = invertedMyMatrix * mousePoint ;
	
	// we need to normalize the value as described here : http://tech.groups.yahoo.com/group/OpenCV/message/80121
	warpedPoint.x = warpedPoint.x / warpedPoint.w;
	warpedPoint.y = warpedPoint.y / warpedPoint.w;
	warpedPoint.z = warpedPoint.z / warpedPoint.w;
	
	return warpedPoint;
}
//--------------------------------------------------------------
ofVec4f ofxQuad::fromWarpToScreenCoord(float x, float y, float z)
{
	ofVec4f mousePoint;
	ofVec4f warpedPoint;
	
	// this is the point inside the warped system which i want to know the coordinates on the image  ...
	mousePoint.x = x;
	mousePoint.y = y;
	mousePoint.z = 0.0;
	mousePoint.w = 1.0;
	
	// i create a ofMatrix4x4 with the ofxGLWarper myMatrixData in column order
	ofMatrix4x4 myOFmatrix = ofMatrix4x4(matrix[0], matrix[4],matrix[8],matrix[12],
										 matrix[1], matrix[5],matrix[9], matrix[13],
										 matrix[2], matrix[6],matrix[10],matrix[14],
										 matrix[3],matrix[7],matrix[11],matrix[15]);
	// invert the matrix
	//ofMatrix4x4 invertedMyMatrix = myOFmatrix.getInverse();
	ofMatrix4x4 invertedMyMatrix = myOFmatrix;
	
	// multiply both to get the point transformed by the matrix
	warpedPoint = invertedMyMatrix * mousePoint ;
	
	warpedPoint.x = warpedPoint.x / warpedPoint.w;
	warpedPoint.y = warpedPoint.y / warpedPoint.w;
	warpedPoint.z = warpedPoint.z / warpedPoint.w;
	
	return warpedPoint;
}
//--------------------------------------------------------------
void ofxQuad::setCorner(CornerLocation cornerLocation, ofPoint screenLocation)
{
    corners[cornerLocation] = screenLocation / ofPoint(w, h, 1);


    CornerLocation location = cornerLocation;

    ofNotifyEvent(changeEvent, location, this);

}
//--------------------------------------------------------------
ofPoint ofxQuad::getCorner(CornerLocation cornerLocation)
{
    return corners[cornerLocation] * ofPoint(w,h, 1);
}
//--------------------------------------------------------------
void ofxQuad::setCornerSensibility(float sensibility)
{
    cornerSensibility = sensibility;
}
//--------------------------------------------------------------
float ofxQuad::getCornerSensibility()
{
    return cornerSensibility;
}

//--------------------------------------------------------------
void ofxQuad::save(string fileName){
    
    
	ofxXmlSettings XML;
	XML.clear();
	XML.addTag("corners");
	XML.pushTag("corners");
	
	
//	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.setValue("corner:TLPositionX",corners[0].x, t);
		XML.setValue("corner:TLPositionY",corners[0].y, t);
        
        XML.setValue("corner:TRPositionX",corners[1].x, t);
		XML.setValue("corner:TRPositionY",corners[1].y, t);
        
        XML.setValue("corner:BLPositionX",corners[2].x, t);
		XML.setValue("corner:BLPositionY",corners[2].y, t);
        
        XML.setValue("corner:BRPositionX",corners[3].x, t);
		XML.setValue("corner:BRPositionY",corners[3].y, t);
        
        
	//}
	XML.saveFile(fileName);
}
//--------------------------------------------------------------
void ofxQuad::load(string fileName){
    quadMesh.clear();
  //  cout << fileName << endl;
    debug = false;
    quadMesh.clear();
    ofxXmlSettings XML;
	if( !XML.loadFile(fileName) ){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : xml file not loaded. Check file path.");
	}
	
	XML.pushTag("corners");
    int t = XML.addTag("corner");
    XML.pushTag("corner");
    corners[0].x = XML.getValue("TLPositionX", double(1.0));
    corners[0].y = XML.getValue("TLPositionY", double(1.0));
    
    corners[1].x = XML.getValue("TRPositionX", double(1.0));
    corners[1].y = XML.getValue("TRPositionY", double(1.0));
    
    corners[2].x = XML.getValue("BLPositionX", double(1.0));
    corners[2].y = XML.getValue("BLPositionY", double(1.0));
    
    corners[3].x = XML.getValue("BRPositionX", double(1.0));
    corners[3].y = XML.getValue("BRPositionY", double(1.0));
    XML.popTag();
    
    
}


