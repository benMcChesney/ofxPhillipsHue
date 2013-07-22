#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofRegisterURLNotification( this ) ;
    ofSetVerticalSync( true ) ;
    ofSetLogLevel( OF_LOG_WARNING ) ;
    ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
	httpUtils.start();
    
    
    ipAddress = "localhost" ; 
    username = "newdeveloper" ; 
    apiMethod + "lights" ;
    
    setupUI( ) ; 
    

}

//--------------------------------------------------------------
void testApp::update(){
    for ( int i = 0 ; i < lightbulbs.size() ; i++ )
    {
        lightbulbs[ i ]->hueOffset += hueIncrement ;
        lightbulbs[ i ]->update() ;
        
        if ( i == 0 )
        {
           // cout << lightbulbs[ i ]->createJsonData() << endl  ;
            ///api/<username>/lights/<id>/state
            action_url = ipAddress +   "/lights/1/state/hue/" + ofToString( lightbulbs[0]->hueOffset + lightbulbs[0]->hue );
            ofxHttpForm form;
            
            form.action = action_url;
            form.method = OFX_HTTP_PUT ;
            form.addFormField("body", lightbulbs[ i ]->createJsonData() ) ;
            httpUtils.addForm( form ) ; 
            requestStr = "message sent: " + ofToString(counter);
            counter++;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
        
    vector< string > callHistory ;

    string currentUrl = ipAddress + "/api/" + username + "/" + apiMethod ;
    stringstream ss ;
    ss << "Current API Call : " << currentUrl << endl ;
    
    for ( int i = 0 ; i < callHistory.size() ; i++ )
    {
        ss << i << "| " << callHistory[i] << endl ;
    }
    
    ss << endl << endl ;
    ss << "last HTTP response : " << endl ; 
    ss << lastHttpResponse.data.getText() << endl ;
    
    
    if ( lightbulbs.size() > 0 )
    {
        float x = 375 ;
        float yStart = 250 ;
        float ySpacing = 105 ;
        for ( int i = 0 ; i < lightbulbs.size() ; i++ )
        {
            
            lightbulbs[ i ]->draw( x , ySpacing * i + yStart ) ; 
            
        }
    }
    ofDrawBitmapStringHighlight( ss.str() , 400 , 15 ) ;
    
    ofDrawBitmapStringHighlight(requestStr,400,500);
	ofDrawBitmapStringHighlight(responseStr,400,560);
}

void testApp::callHueApi( string apiUrl , string callTag )
{
    callHistory.push_back( apiUrl ) ;
    ofLoadURLAsync( apiUrl , callTag ) ;
    
    if ( callHistory.size() > 10 )
    {
       // callHistory.erase( callHistory.begin() ) ;
    }
}

void testApp::setupUI( )
{
    
    
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    gui->addToggle( "SAVE SETTINGS" , false ) ; 
    gui->addToggle("DEBUG HTTP CALL" , false ) ;
    gui->addToggle("GET BULB INFO" , false ) ; 
    
    gui->addTextInput("DEVELOPER USERNAME", username , length-xInit);
    gui->addTextInput("HUE BRIDGE IP ADDRESS", ipAddress , length-xInit);
    gui->addTextInput("API METHOD", apiMethod  , length-xInit);
    gui->addSlider( "HUE INCREMENT" , 0.0f , 500.0f , &hueIncrement ) ;
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
    
    gui->loadSettings( "GUI/settings.xml") ;

}

void testApp::guiEvent ( ofxUIEventArgs &e )
{
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    /*
     gui->addTextInput("DEVELOPER USERNAME", username , length-xInit);

     */
    if(name == "DEVELOPER USERNAME")
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        username = textinput->getTextString() ;
        
    }
    
    if(name == "HUE BRIDGE IP ADDRESS" )
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        ipAddress = textinput->getTextString() ;
        
    }
    
    if(name == "API METHOD" )
    {
        ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
        apiMethod = textinput->getTextString() ;
    }
    
    if(name == "DEBUG HTTP CALL")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if ( toggle->getValue() == true )
        {
            toggle->setValue ( false ) ;
            
            string testUrl = ipAddress + "/api/" + username + "/" + apiMethod ;
            //"http://192.168.2.4/api/newdeveloper/lights" ;
            callHueApi ( testUrl  ) ;
        }
    }
    if(name == "SAVE SETTINGS")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if ( toggle->getValue() == true )
        {
            toggle->setValue ( false ) ;
            gui->saveSettings("GUI/settings.xml" );
        }
    }
    
    if(name == "GET BULB INFO" )
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		if ( toggle->getValue() == true )
        {
            toggle->setValue ( false ) ;
            string testUrl = ipAddress + "/api/" + username + "/lights" ;
            callHueApi( testUrl , "LIGHTS_INFO" ) ;
        }
    }

    
    
    /*
     HueLightbulb
     */
    /*
     if(name == "RED")
     {
     ofxUISlider *slider = (ofxUISlider *) e.widget;
     red = slider->getScaledValue();
     cout << "value: " << slider->getScaledValue() << endl;
     }*/
}


//--------------------------------------------------------------
void testApp::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
}

/*
	ofxHttpForm form;
	form.action = action_url;
	form.method = OFX_HTTP_POST;
	form.addFormField("number", ofToString(counter));
	form.addFile("file","ofw-logo.gif");
	httpUtils.addForm(form);
	requestStr = "message sent: " + ofToString(counter);
	counter++;

*/


void testApp::urlResponse(ofHttpResponse &httpResponse){
    
    stringstream ss ;
    ss << "urlResponse status code : " << httpResponse.status << endl ;
    
    if ( httpResponse.status == 200 )
    {
        ss << "name : " << httpResponse.request.name << endl ;
        //ss << "url : " << httpResponse.request.url << endl ;
        if ( httpResponse.request.name == "LIGHTS_INFO" )
        {
            bool bResult = json.parse( httpResponse.data.getText() ) ;
            if ( bResult == true )
            {
               if ( lightbulbs.size() > 0 )
               {
                   lightbulbs.clear() ;
               }
                
                
                for ( int j = 0 ; j < json.size() ; j++ )
                {
                    
                    //cout << "opening lightbulb data : " << j << endl ;
                    stringstream parseInput ;
                    parseInput << json[ofToString(j)]["name"] << endl ;
                    string _name = parseInput.str() ;
                    removeNewLine( &_name ) ; 
                    
                    //cout << " name : '" << parseInput.str() << "'" << endl ;
                    //The first value is NULL so we should check for that 
                    if ( parseInput.str().find( "null" ) == -1 )
                    {
                        lightbulbs.push_back( ( ofPtr< ofxHueLightbulb > ) new ofxHueLightbulb() ) ;
                        lightbulbs[ (lightbulbs.size() -1 ) ]->setup( parseInput.str() ) ;
                        
                        //cout << " index : " << j << endl ;
                        string testUrl = ipAddress + "/api/" + username + "/lights/" + ofToString( j ) ;
                        callHueApi( testUrl , "LIGHTBULB_" + ofToString( j ) ) ; 
                    }
                    
                }
                
                
            }
        }
        
        int findIndex = httpResponse.request.name.find("LIGHTBULB_" ) ;
        
        ss << "FIND INDEX : " << findIndex << endl ;
        if (  findIndex > -1 )
        {
            
            int index = ofToInt( ofToString( httpResponse.request.name[10] ) ) - 1 ;
            ss << "index : " << index << endl ; 
            cout << "Lightbulb : " << index << " data has loaded ! " << endl ;
            
            ofxJSONElement lightData ;
            bool bResult = lightData.parse( httpResponse.data.getText() ) ;
            if ( bResult == true )
            {
                //Hue
                stringstream hueSs ;
                hueSs << lightData["state"]["hue"] ;
                string _hue = hueSs.str() ; //hueSs.str().substr( 0 , (hueSs.str().size()-1)) ;
                removeNewLine( &_hue ) ;
                
                //Saturation
                stringstream saturationSs ;
                saturationSs << lightData["state"]["sat"] ;
                string _saturation = saturationSs.str() ;
                removeNewLine( &_saturation ) ;
                
                //Brightness
                stringstream brightnessSs ;
                brightnessSs << lightData["state"]["bri"] ;
                string _brightness = brightnessSs.str() ;
                removeNewLine( &_brightness ) ;
                
                //On / Off
                stringstream onSs ;
                onSs << lightData["state"]["on"] ;
                string _on = onSs.str().substr( 0 , (onSs.str().size()-1)) ;
                //cout << "_on" << "before remove line: " << _on << endl ;
                removeNewLine( &_on ) ;
             
                
                
                ss << " on : '" << _on << "'" << endl ;
                
                //cout << "_hue : '"<<_hue<<"'"<<endl ;
                if ( index < lightbulbs.size() )
                {
                    lightbulbs[ index ]->hue = ofToFloat( _hue) ;
                    lightbulbs[ index ]->saturation = ofToFloat( _saturation ) ;
                    lightbulbs[ index ]->brightness = ofToFloat( _brightness ) ;
                    lightbulbs[ index ]->bOnState = ofToBool( _on ) ;
                    /*
                    if ( _on.compare( "true" ) == 0 )
                    {
                        cout << " ON ! : " << endl ;
                        lightbulbs[ index ]->bOnState = true ; 
                    }
                    else if ( _on.compare( "false" ) == 0 )
                    lightbulbs[ index ]->bOnState = false ;*/
                }
            }
            /*
              {"state": {"on":true,"bri":225,"hue":45489,"sat":253,"xy":[0.1859,0.0771],"ct":500,"alert":"none","effect":"none","colormode":"xy","reachable":true}, "type": "Extended color light", "name": "Hue Lamp 5", "modelid": "LCT001", "swversion": "65003148", "pointsymbol": { "1":"none", "2":"none", "3":"none", "4":"none", "5":"none", "6":"none", "7":"none", "8":"none" }}
             */
        }
       

    }
    
    //ofLog( OF_LOG_WARNING ,  httpResponse.data.getText() ) ;
    lastHttpResponse = httpResponse ;
    ofLog( OF_LOG_WARNING , ss.str() ) ;
}

void testApp::removeNewLine( string * in )
{
    in->substr(  0 , (in->size() -1 )) ;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

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