//
//  HueLightbulb.cpp
//  ofxPhillipsHue_BasicExample
//
//  Created by Ben McChesney on 7/13/13.
//
//

#include "ofxHueLightbulb.h"

void ofxHueLightbulb::setup ( string _name )
{
    bOnState = false ;
    brightness = 255 ;
    hue = 30000 ;
    saturation = 255 ;
    name = _name ;
    hueOffset= 0 ; 
}

void ofxHueLightbulb::update ( )
{
    if ( ( hue + hueOffset ) > MAX_HUE )
    {
        hueOffset -= MAX_HUE ;
    }
}

void ofxHueLightbulb::draw( float x , float y )
{
    
    ofSetColor( getOfxColor() ) ;
    float radius = 20 ;
    ofCircle( x , y , radius ) ;

    ofSetColor( getOffsetOfxColor() ) ;
    ofCircle( x + radius * 2 , y , radius ) ;

    ofSetColor( ofColor::white ) ;
    stringstream ss ;
    ss << bOnState << " h|"<< hue << " ho|" << hueOffset << endl ; 
    ofDrawBitmapStringHighlight( ss.str() , x , y - radius - 15 , ofColor::black , getOfxColor() ) ;
}

string ofxHueLightbulb::getDebugString ( )
{
    return "" ;
}


ofColor ofxHueLightbulb::getOffsetOfxColor( )
{
    float ofxHue = ofMap ( hue + hueOffset , 0.0f ,65535.0f , 0.0f , 255.0f , true ) ;
    float ofxSaturation = saturation ;
    float ofxBrightness = brightness ;
    ofColor c = ofColor::fromHsb( ofxHue , ofxSaturation , ofxBrightness ) ;
    return c ;
}

ofColor ofxHueLightbulb::getOfxColor( )
{
    float ofxHue = ofMap ( hue , 0.0f ,65535.0f , 0.0f , 255.0f , true ) ;
    float ofxSaturation = saturation ;
    float ofxBrightness = brightness ; 
    ofColor c = ofColor::fromHsb( ofxHue , ofxSaturation , ofxBrightness ) ;
    return c ; 
}

string ofxHueLightbulb::createJsonData()
{
    stringstream json ;
    json << "{" << endl ;
    
    json << "hue: " << ofToString( (int)(hue + hueOffset) ) << "," << endl;
    json << "on: " << ofToString( bOnState ) << "," << endl;
    
    json << "}" ;
    
    return json.str() ;
}
/*
 {
 "hue": 50000,
 "on": true,
 "bri": 200
 }
 */
