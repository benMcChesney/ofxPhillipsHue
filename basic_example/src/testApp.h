#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ofxJSONElement.h"
#include "ofxHueLightbulb.h"  
#include "ofxHttpUtils.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        void urlResponse(ofHttpResponse &httpResponse) ;
    
        //void urlResponse(ofxHttpResponse & httpResponse);
        void newResponse(ofxHttpResponse & response);

        ofxUICanvas * gui ;
        void setupUI( ) ;
        void guiEvent ( ofxUIEventArgs &e ) ;
        void callHueApi( string apiUrl , string callTag = "" ) ;
    
        vector< string > callHistory ;
    
        //Hue API variables
        string ipAddress ;
        string username ;
        string apiMethod ;
    
    
        ofHttpResponse lastHttpResponse ;
    
        ofxJSONElement json;
    
        vector< ofPtr<ofxHueLightbulb> > lightbulbs ;
        void removeNewLine( string * in ) ;
        
        ofxHttpUtils httpUtils;
        int counter;
        string responseStr;
        string requestStr;
        string action_url;
    
        float hueIncrement ; 
    
    
};
