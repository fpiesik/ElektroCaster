include <parameter.scad>;

//intersection(){
//rotate([180,0,0])fretboard(2);
//translate([-100,175,0])cube([200,60,200]);
//}

//neckShape(12);
//rotate([90,0,0])neckBack();
//rotate([0,180,0])fretboard();

topZ=fretboardZ;
ledCutY=13;
fret12off=0.5; //also change 

//neckShaped();

module fretboard(part){
fretNotch=0.7;
fretNotchBtm=0.9;
ledX=5.2;
ledY=ledCutY;
ledZ=2.5;
//ledCutY=12.5;
ledCutZ=1.5;
ledDia=4.5;
shaftX=5;
printSpZ=0.2;

nutXA=10;
nutXB=5;
nutZA=1.2;
nutZ=4;

translate([0,-(scale-neckSlotY),0])rotate([-90,0,0])difference(){
union(){

difference(){
if(part==1)neckShape(1);
if(part==2)neckShape(2);

translate([-fretLB,-slotXY,-1])cube([fretLB*2,slotXY*2,scale+2]);
}

//hull()translate([0,0,scale])rotate([-90,0,180]){
//translate([-nutXA/2,0,0])cube([nutXA,neckSlotY,nutZA]);
//translate([-nutXB/2,0,0])cube([nutXB,neckSlotY,nutZ]);
//}

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

////fretboardScrews
//if(i==1||i==3||i==5||i==7||i==9){
//translate([-(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+(fretDist-ledCutY)/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([-(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*2,h=topZ-scrwSp,$fn=12);
//
//translate([(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+(fretDist-ledCutY)/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*2,h=topZ-scrwSp,$fn=12);
//}
//if(i==12){
//off=fret12off;
//translate([-(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+(fretDist-ledCutY)/2+off])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([-(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+(fretDist-ledCutY)/2+off])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*1.6,h=topZ-scrwSp,$fn=12);
//
//translate([(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+(fretDist-ledCutY)/2+off])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+(fretDist-ledCutY)/2+off])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*1.6,h=topZ-scrwSp,$fn=12);
//}
//
//if(i==18||i==19){
//translate([-(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+fretDist/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([-(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+fretDist/2])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*2,h=topZ-scrwSp,$fn=12);
//
//translate([(neckSlotX/2-slotXY/2),-slotXY+1,fretPos+fretDist/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY*4,$fn=12);
//translate([(neckSlotX/2-slotXY/2),-slotXY-scrwSp,fretPos+fretDist/2])rotate([90,0,0])cylinder (d1=m3Dia,d2=m3Dia*2,h=topZ-scrwSp,$fn=12);
//}

//longNutScrews
if(i==1||i==3||i==5||i==7||i==9){
translate([0,+1,fretPos+(fretDist-ledCutY)/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
translate([0,-nutZ+m3NutZ,fretPos+(fretDist-ledCutY)/2])rotate([90,30,0])cylinder (d=m3NutDia,h=slotXY/2,$fn=6);
}
if(i==12){
off=fret12off;
translate([0,+1,fretPos+(fretDist-ledCutY)/2+off])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
translate([0,-nutZ+m3NutZ,fretPos+(fretDist-ledCutY)/2+off])rotate([90,30,0])cylinder (d=m3NutDia,h=slotXY/2,$fn=6);
}
if(i==18||i==19){
translate([0,+1,fretPos+fretDist/2])rotate([90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
translate([0,-nutZ+m3NutZ,fretPos+fretDist/2])rotate([90,30,0])cylinder (d=m3NutDia,h=slotXY/2,$fn=6);
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


sideScale=1/4;
//topZ=4;
bottomZ=0.2;
neckY=slotXY+bottomZ+topZ;
bottomScale=1/9;
topScale=1/20;
y=scale-1;
btmScrwPos=[675,575,475,375];
btmScrwDist=130;
scrwSp=1;

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
//translate([0,-slotXY-topZ,0])scale([1,topScale,1])cylinder(d=fretL+slotXY*sideScale/2,h=1);

}
//translate([(fretL/2+fretLSp),-slotXY,0])scale([1,1,1])cube([slotXY,slotXY*2,1]);
//translate([-(fretL/2+fretLSp+slotXY),-slotXY,0])scale([1,1,1])cube([slotXY,slotXY*2,1]);
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

//for(i=[1:nFrets]){
//fretL=fretLB;
//translate([fretL,-slotXY/2,scale-(scale-(scale/pow(k,i)))])rotate([0,-90,0])cylinder (d=m4Dia,h=fretL*2,$fn=12);
//}

//screwHoles
if(0==1)for(i=[0:nFrets]){
fretL=fretLB;
scrwSpZ=1;
scrwHeadZ=2;
if(i==1||i==3||i==5||i==7||i==9||i==12){
fretDist=(scale-(scale/pow(k,i)))-(scale-(scale/pow(k,i-1)));
fretPos=scale-(scale-(scale/pow(k,i)));
if(i!=12)translate([0,0,0]){

translate([-(neckSlotX/2-slotXY/2),-1,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
hull(){
translate([-(neckSlotX/2-slotXY/2),scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=5,$fn=12);
translate([-(neckSlotX/2-slotXY/2),scrwSp+scrwHeadZ,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia*2,h=5,$fn=12);
}

translate([(neckSlotX/2-slotXY/2),-1,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
hull(){
translate([(neckSlotX/2-slotXY/2),scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=5,$fn=12);
translate([(neckSlotX/2-slotXY/2),scrwSp+scrwHeadZ,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia*2,h=5,$fn=12);
}

translate([fretL,-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,-90,0])cylinder (d=m3Dia,h=fretL*2,$fn=12);
hull(){
translate([-neckSlotX/2-scrwSp,-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,-90,0])cylinder (d=m3Dia,h=2,$fn=12);
translate([-neckSlotX/2-scrwSp-scrwHeadZ,-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,-90,0])cylinder (d=m3Dia*2,h=slotXY,$fn=12);
}
hull(){
translate([-(-neckSlotX/2-scrwSp),-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,90,0])cylinder (d=m3Dia,d2=m3Dia*2,h=5.5,$fn=12);
translate([-(-neckSlotX/2-scrwSp-scrwHeadZ),-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,90,0])cylinder (d=m3Dia*2,h=5.5,$fn=12);
}
}
if(i==12)translate([0,0,fret12off]){
translate([-(neckSlotX/2-slotXY/2),-1,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
translate([-(neckSlotX/2-slotXY/2),scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d1=m3Dia*2,d2=m3Dia*2,h=2,$fn=12);
translate([(neckSlotX/2-slotXY/2),-1,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d=m3Dia,h=slotXY,$fn=12);
translate([(neckSlotX/2-slotXY/2),scrwSp,fretPos+(fretDist-ledCutY)/2])rotate([-90,0,0])cylinder (d1=m3Dia*2,d2=m3Dia*2,h=2,$fn=12);
translate([fretL,-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,-90,0])cylinder (d=m3Dia,h=fretL*2,$fn=12);
translate([-neckSlotX/2-scrwSp,-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,-90,0])cylinder (d1=m3Dia*2,d2=m3Dia*2,h=5.5,$fn=12);
translate([-(-neckSlotX/2-scrwSp),-slotXY/2,fretPos+(fretDist-ledCutY)/2])rotate([0,90,0])cylinder (d1=m3Dia*2,d2=m3Dia*2,h=5.5,$fn=12);
}}

}

//translate([-fretLB,-slotXY*5,-neckSlotY-slotXY])cube([fretLB*2,slotXY*10,scale]);
if(purpose==1){
translate([-fretLB,-slotXY*5,+3+(scale-(scale-(scale/pow(k,17))))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]); 
}//FretLedLengh
if(purpose==2){
translate([-fretLB,-slotXY*5,+3+scale-(scale-(scale/pow(k,17)))])scale([1,1,1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*5,-5+scale-(scale-(scale/pow(k,23)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
}
//translate([-fretLB,-slotXY*5,scale-(scale-(scale/pow(k,4)))])scale([1,1,1])cube([fretLB*2,slotXY*10,scale]);
//translate([-fretLB,-slotXY*5,scale-(scale-(scale/pow(k,17)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);

if(purpose==12){
translate([-fretLB,-slotXY*5,scale-(scale-(scale/pow(k,17)))])scale([1,1,1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*5,-1+scale-(scale-(scale/pow(k,17)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
translate([-fretLB,-slotXY*11,scale-(scale-(scale/pow(k,15)))])scale([1,1,-1])cube([fretLB*2,slotXY*10,scale]);
}

}
}
}
