#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1024, 768, OF_FULLSCREEN);
//    window.setGlutDisplayString("rgba double samples>=16 depth");
	ofRunApp(new testApp()); // start the app
}
