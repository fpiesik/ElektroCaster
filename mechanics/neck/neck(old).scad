include <parameter.scad>;


//neckBack();
//fretboard(1);
//fretboard(2);

topZ=fretboardZ;
ledCutY=13;
fret12off=0.5; //also change 


module fretboard(part){
fretNotch=0.7; //size off the gap for the frets
fretNotchBtm=0.9;//how much material beneath the fret
ledX=5.2;
ledY=ledCutY;
ledZ=2.5;
ledCutZ=1.5;
ledDia=4.5;
shaftX=5; //size of the power cable shaft

translate([0,-(scale-neckSlotY),0])rotate([-90,0,0])difference(){
union(){

difference(){
if(part==1)neckShape(1);
if(part==2)neckShape(2);

translate([-fretLB,-slotXY,-1])cube([fretLB*2,slotXY*2,scale+2]);
}

}
for(i=[0:nFrets]){

fretDist=(scale-(scale/pow(k,i)))-(scale-(scale/pow(k,i-1)));
echo (fretDist);
fretPos=scale-(scale-(scale/pow(k,i)));
fretPosFactor=fretPos/scale;
fretL=fretLN*fretPosFactor+fretLB*(1-fretPosFactor);
strDist=strDistN*fretPosFactor+strDistB*(1-fretPosFactor);
strSpan=(strDistN*fretPosFactor+strDistB*(1-fretPosFactor))*(nStrings-1);

ledCutX=strSpan+ledX+2.1;

scrwSp=1;

//fretNotches
if(i>0&&i<=nFrets){
t=-0.3;
translate([0,-slotXY-ledZ,fretPos])rotate([90,0,0])cylinder (d=2,h=fretLB*2,$fn=12);
translate([-(fretL/2+t),-slotXY-fretNotchBtm,fretPos-fretNotch/2])scale([1,-1,1])cube([fretL+t*2,10,fretNotch]);
hull(){
translate([-(fretL/2+t),-slotXY-topZ+0.6,fretPos-fretNotch/2])scale([1,-1,1])cube([fretL+t*2,10,fretNotch]);
translate([-(fretL/2+t),-slotXY-topZ,fretPos-fretNotch])scale([1,-1,1])cube([fretL+t*2,10,fretNotch*2]);
}
}
//leds
yOff=1;
if(i<=nLedFrets){

if(i==0){
for(i=[0:nStrings-1]){
translate([-strSpan/2-ledX/2+i*strDist,-slotXY+1,fretPos-ledCutY-fretNotch/2-yOff])scale([1,-1,1])cube([ledX,ledZ+1,ledY]);
translate([-strSpan/2+i*strDist,-slotXY+0.3,fretPos-ledCutY/2+fretNotch/2-yOff])rotate([90,0,0])cylinder(d=ledDia,h=topZ,$fn=12);
}
translate([-ledCutX/2,-slotXY+1,fretPos-ledCutY-fretNotch/2-yOff])scale([1,-1,1])cube([ledCutX,ledCutZ+1,ledCutY]);
}
if(i>0){
for(i=[0:nStrings-1]){
translate([-strSpan/2-ledX/2+i*strDist,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([ledX,ledZ+1,ledY]);
translate([-strSpan/2+i*strDist,-slotXY+0.3,fretPos+ledCutY/2+fretNotch/2+yOff])rotate([90,0,0])cylinder(d=ledDia,h=topZ,$fn=12);
}
translate([-ledCutX/2,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([ledCutX,ledCutZ+1,ledCutY]);
}
}
if(i==1){
compensation=0.1;
translate([-ledCutX/2+compensation,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([(ledCutX-strSpan-ledX)/2,ledCutZ+1,fretDist-yOff*3]);
translate([ledCutX/2-(ledCutX-strSpan-ledX)/2-compensation,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([(ledCutX-strSpan-ledX)/2,ledCutZ+1,fretDist-yOff*3]);
}
//dataCableShaft
if(i>1&&part!=2){
compensation=0.1;
translate([-ledCutX/2+compensation,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([(ledCutX-strSpan-ledX)/2,ledCutZ+1,fretDist+0.5]);
translate([ledCutX/2-(ledCutX-strSpan-ledX)/2-compensation,-slotXY+1,fretPos+fretNotch/2+yOff])scale([1,-1,1])cube([(ledCutX-strSpan-ledX)/2,ledCutZ+1,fretDist+0.5]);
}
}
translate([-shaftX/2,-slotXY+1,190])scale([1,-1,1])cube([shaftX,ledZ+1,scale-1.4]);
}
}

module neckBack(){
translate([0,-(scale-neckSlotY),0])rotate([-90,0,0])difference(){
neckShape(1);
translate([-fretLB,-slotXY*3,-1])cube([fretLB*2,slotXY*2,scale+2]);
}
}
module neckShape(purpose){
//purposes:
sideScale=1/4; //alters the roundness of the edges of the fretboard
bottomZ=0.7; //add thickness to the back

bottomScale=1/9; //roundness of the back of the neck
y=scale-1;
neckY=slotXY+bottomZ+topZ;
    
if(purpose==11)neckShape2d(fretLN,0);


module neckShape2d(fretLength,pos){
$fn=64;
fretLSp=0.3;
fretL=fretLength;

translate([0,0,pos]){
difference(){
hull(){
translate([(fretL/2+fretLSp),-neckY/2+bottomZ,0])scale([sideScale,1,1])cylinder(d=neckY,h=1);
translate([-(fretL/2+fretLSp),-neckY/2+bottomZ,0])scale([sideScale,1,1])cylinder(d=neckY,h=1);
translate([0,bottomZ,0])scale([1,bottomScale,1])cylinder(d=fretL+slotXY*sideScale/4,h=1);
}
difference(){
translate([0,-slotXY,0])scale([1,1,1])cylinder(d=fretL+slotXY*2,h=1);
translate([0,-slotXY,0])scale([1,2.3,1])cylinder(d=fretL+4.8,h=1,$fn=128);
}
}
}
}
if(purpose==1||purpose==12||purpose==2){
difference(){
hull(){



neckShape2d(fretLB,0);
neckShape2d(fretLN,y);

}
if(purpose!=12)translate([-neckSlotX/2,-slotXY,-1])cube([neckSlotX,slotXY,y+3]);



if(purpose==1){
translate([-fretLB,-slotXY*5,+3+(scale-(scale-(scale/pow(k,nFrets+1))))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]); 
}//FretLedLengh
if(purpose==2){
translate([-fretLB,-slotXY*5,+3+scale-(scale-(scale/pow(k,nFrets+1)))])scale([1,1,1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*5,-5+scale-(scale-(scale/pow(k,23)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
}
if(purpose==12){
translate([-fretLB,-slotXY*5,scale-(scale-(scale/pow(k,nFrets+1)))])scale([1,1,1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*5,-1+scale-(scale-(scale/pow(k,nFrets+1)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*11,scale-(scale-(scale/pow(k,15)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
}

}
}
}
