#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    CGDisplayShowCursor(kCGDirectMainDisplay);
//  CGDisplayHideCursor(kCGDirectMainDisplay);
    swapGloble = 0;
    dir.listDir("videos");
	dir.sort();
    ofSetVerticalSync(true);
    hozpertion = 1;
    load = 0;
    ofEnableSmoothing();
    maxNumber = (int)dir.size();
    mapping =    new ofxQuad*[20];
    width  = 10;
    height = 10;
    for(int i = 0; i < maxNumber; i++){
        mapping[i] = new ofxQuad();
        vid.push_back(ofVideoPlayer());
        vid[i].loadMovie(dir.getPath(i));
       // vid[i].setPixelFormat(ofPixelFormat _rg)
        mapping[i]->setup(width+10,10, 200, 200, ofGetWidth(), ofGetHeight(),i);
        mapping[0]->activate();
        width = vid[i].getWidth() * (1 + i);
        height = 10;
        vid[i].play();
        mapping[i]->debugSet(0);
    }
    h = ofGetHeight();
    w = ofGetWidth();
    loadMe();
}

//--------------------------------------------------------------
void testApp::update(){

    for (int i = 0; i < maxNumber; i++){
        vid[i].update();
        mapping[i]->update();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0, 0, 0);
    
    for (int i = 0; i < maxNumber; i++){
        mapping[i]->draw(vid[i], 0, 0, vid[i].getWidth(), vid[i].getHeight(),i);
    }
    
    if(debug == 1){
        ofSetColor(255, 255, 255);
        ofDrawBitmapString("SPACEBAR ON OFF\ns to save\nl to load\nz to draw mode\n0-9 select quad", 20,300);
        ofLine(10, ofGetMouseY(), ofGetWidth() + 10, ofGetMouseY() );
        ofLine(ofGetMouseX(), 10, ofGetMouseX(), ofGetHeight()+ 10 );
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

int swap = ofMap(key, 48, 57, 0, 9);

    if((swap <= 9)&&(swap >= 0)){
        if(swap < maxNumber){
              for(int i = 0; i < maxNumber; i++){
                  mapping[i]->deactivate();
              }
            mapping[swap]->activate();
            swapGloble = swap;
        }
    }
    
    switch (key) {
        case 's':
            save();

            break;
        case 'l':
            loadMe();

            break;
        case 'f':
            
            CGDisplayHideCursor(kCGDirectMainDisplay);

            ofToggleFullscreen();
            break;
        case ' ':
            if(debug == 0){

                debug = 1;
            }
            else{
                debug = 0;
            }
            
            for(int i = 0; i < maxNumber; i++){
                mapping[i]->debugSet(debug);
            }
            break;
            
        case 13:
            maxNumber ++;
            mapping[maxNumber-1] = new ofxQuad();
            vid.push_back(ofVideoPlayer());
            vid[maxNumber-1].loadMovie(dir.getPath(0));

            width =  vid[maxNumber-1].getWidth() * (maxNumber-hozpertion);
            if((width + vid[maxNumber-1].getWidth()) > ofGetWidth() ){
                hozpertion = maxNumber + 1;
                height += vid[maxNumber-1].getHeight();
                width = 10;
            }
          
            mapping[maxNumber-1]->setup(width+10,10+height, 200, 200, ofGetWidth(), ofGetHeight(),maxNumber-1);
            mapping[maxNumber-1]->deactivate();
            vid[maxNumber-1].play();
            mapping[maxNumber-1]->debugSet(0);
            
            break;
            
        case 357:
            
            if( load >=(int)dir.size()-1){
                load = 0;
            }
            else{
                load += 1;
            }
            vid[swapGloble].loadMovie(dir.getPath(load));
            
            break;
    
    
        case 359:
            
            if( load <=0){

                load = (int)dir.size()-1;
            }
            else{
                load -= 1;
                
            }

            vid[swapGloble].loadMovie(dir.getPath(load));
            break;
            
        default:
            break;
    }
  //      cout << " me first " << endl;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::loadMe(){
    
    for(int i = 0; i < maxNumber; i++){
        vid[i].close();
        delete  mapping[i];
        
    }
    vid.empty();
    vid.clear();

    maxNumber = 0;
    width = 10;
    height = 10;
    
         
    
    // }
    
    ofxXmlSettings XML;
	if( !XML.loadFile("glob.xml") ){
		ofLog(OF_LOG_ERROR, "no xml");
	}
	
	XML.pushTag("GLOB");
//    int t = XML.addTag("GLOBY");
//    XML.pushTag("GLOBY");
    maxNumber = XML.getValue("GLOB:maxNumber", double(1.0));
    
    
    cout << maxNumber << endl;
    for(int x = 0;x < maxNumber;x++){
    
        mapping[x] = new ofxQuad();
        vid.push_back(ofVideoPlayer());
        vid[x].loadMovie( ofToString(XML.getValue("GLOB:vidPath"+ofToString(x), "videos/hands.mov")));
        mapping[x]->setup(width+10,10, 200, 200, w, h,x);
        mapping[x]->deactivate();
        mapping[0]->activate();
        width = vid[x].getWidth() * (1 + x);
        height = 10;
        vid[x].play();
        mapping[x]->debugSet(0);

    }
    
    XML.popTag();

    for (int i = 0; i < maxNumber; i++){
        mapping[i]->load(ofToString(i)+".xml");
    }

}
void testApp::save(){
    ofxXmlSettings XML;
	XML.clear();
    XML.addTag("GLOB");
    XML.pushTag("GLOB");
    XML.setValue("GLOB:maxNumber",maxNumber);
    
    for(int x = 0;x < maxNumber;x++){
        XML.setValue("GLOB:vidPath"+ofToString(x),vid[x].getMoviePath());
    }


	XML.saveFile("glob.xml");
    for (int i = 0; i < maxNumber; i++){
        mapping[i]->save(ofToString(i)+".xml");
    }
}