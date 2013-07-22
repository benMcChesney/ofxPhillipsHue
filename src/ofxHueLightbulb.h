//
//  HueLightbulb.h
//  ofxPhillipsHue_BasicExample
//
//  Created by Ben McChesney on 7/13/13.
//  This class is used to control a single Phillips Hue lightbulb and work with the data nicely
//

#ifndef __ofxPhillipsHue_BasicExample__HueLightbulb__
#define __ofxPhillipsHue_BasicExample__HueLightbulb__

#include <iostream>

#include "ofMain.h"
#include "ofxJSONElement.h"

static int MAX_HUE = 65535 ;

class ofxHueLightbulb
{
public:
    ofxHueLightbulb( ) { }
    ~ofxHueLightbulb( ) { }
    
    void setup ( string name ) ;
    void update ( ) ;
    void draw( float x , float y ) ;

    string getDebugString ( ) ;

    bool bOnState ;
    float brightness ;
    float hue ;
    float saturation ;
    string name ;

    ofColor getOfxColor( ) ;
    ofColor getOffsetOfxColor( ) ;
    float hueOffset ;
    
    string createJsonData() ; 
    
    /*
     {"state": {"on":true,"bri":225,"hue":45489,"sat":253,"xy":[0.1859,0.0771],"ct":500,"alert":"none","effect":"none","colormode":"xy","reachable":true}, "type": "Extended color light", "name": "Hue Lamp 1", "modelid": "LCT001", "swversion": "65003148", "pointsymbol": { "1":"none", "2":"none", "3":"none", "4":"none", "5":"none", "6":"none", "7":"none", "8":"none" }}
     */
};
#endif /* defined(__ofxPhillipsHue_BasicExample__HueLightbulb__) */
