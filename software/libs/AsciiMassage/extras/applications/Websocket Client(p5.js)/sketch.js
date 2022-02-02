// THE HTML FILE IMPORTS THE FOLLOWING LIBRARIES USED BY THIS SKETCH:
// - "libraries/p5.js" [ p5.js (http://p5js.org/) ]
// - "libraries/AsciiMassagePacker.js"
// - "libraries/AsciiMassageParser.js"


var ws;
var msFromArduino =0;
var a0FromArduino =0;
var debugLed = 0;

var inbound = new AsciiMassageParser();
var outbound = new AsciiMassagePacker();


 function setup() { 

     createCanvas(windowWidth,windowHeight);

      
     // ETABLIR UN WEBSOCKET 
    ws = new WebSocket('ws://192.168.4.1:81/', ['arduino']);


     ws.onopen = function (event) {
        console.log("connected");
        
     };
     
     // QUOI FAIRE QUAND UN MESSAGE EST RECU
     ws.onmessage = function(event) {

        //console.log("got: "+event.data);
        for ( var i =0 ; i < event.data.length; i++ ) {
            if ( inbound.parse( event.data.charAt(i)) ) {
                if ( inbound.fullMatch("ms")) {
                    msFromArduino = inbound.nextLong();
                } else if ( inbound.fullMatch("a0")) {
                    a0FromArduino = inbound.nextInt();
                }
            }    
        }
      };

 }

 function draw(){
    
    if ( debugLed == 1 ) {
        background(255); 
        fill(0);
    } else {
        background(0); 
        fill(255);
    }
    
    text("ms:"+msFromArduino, width*0.5, height*0.5-10);
    text("a0:"+a0FromArduino, width*0.5, height*0.5+10);
}
 

function mousePressed() {
     if ( debugLed == 0 ) debugLed = 1;
     else debugLed = 0;
     console.log("sending d "+debugLed);
        
    outbound.packOneInt("d", debugLed);

    ws.send(outbound.buffer());
    
}


function windowResized( ) {

    resizeCanvas(windowWidth, windowHeight);
}