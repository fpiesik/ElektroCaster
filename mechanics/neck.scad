include <parameter.scad>
include <slotBody.scad>;

L=scale; //scale 
k=1.05946; //constant for the fret sizes

nStr=nStrings;
nFr=24; //nFrets; //number of frets used
strSpcN=strDistN;
strSpcB=strDistB;

ledXY=5;
ledZ=2;
ledXYT=1;
ledPadX=1.75; //x size of an led pad
ledPadY=1.5;   //y size of an led pad
ledPadYS=3.5;
ledPadXS=3;
ledWPadsX=ledPadXS+ledPadX*2;
ledFootX=ledPadX*2+ledPadXS;
ledFootY=ledPadY*3+ledPadYS*2;
fedFretOff=4.5;

pcbZ=fretPcbZ;

dataCOff=1;
dataEndOff=4;
strAngle=0.7;

ovrZ=fretOvrZ;
fretY=1; //size of the fret sink
fretSnk=2.1;
shnThrZ=0.3; //shine through layer z

mntSpcY=1.5;
mntBtmZ=neck_bottomT;
mntSW=neck_sideT;
mntScrwDst=70;
mntEndT=neck_mntEndT;

cutY=280;


lastFretSpc=neck_lastFretSpc;

z=0.2; //height of everything (it's 2d anyway)

trTh=0.75; //thickness of the traces
trPwrOff=fedFretOff+ledWPadsX/2+1.2;

holeDia=1.5;
holeRes=8;




module led(part){
    padX=ledPadX;
    padY=ledPadY;
    padYS=ledPadYS;
    padXS=ledPadXS;  
    if(part==0||part==1)cube([ledXY,ledXY,ledZ],center=true); 
    if(part==0||part==3){
        cube([ledXY+ledXYT,ledWPadsX+0.5,ledZ*2],center=true);
        translate([0,0,0])rotate([0,0,-90])cylinder(d=ledXY+0.75,h=ovrZ-shnThrZ,$fn=3);
    }
    if(part==0||part==2)rotate([0,0,90])for(fx=[0:1]){
    translate([(padXS+padX)*fx-ledFootX/2,ledPadYS/2-padY/2,0])cube([padX,padY,z]);
    translate([(padXS+padX)*fx-ledFootX/2,-ledPadYS/2-padY/2,0])cube([padX,padY,z]);
    //translate([(padXS+padX)*fx-ledFootX/2,(padYS+padY)*fy-ledFootY/2,0])cube([padX,padY,z]);   
    }
}


module ledFret(pos,ledPart){
    for(i=[0:nStr-1]){
        translate([strSpc(pos)*i-((strSpc(pos)*(nStr-1))/2),fedFretOff,0])led(ledPart);
    }
}

module fret(pos,z){
//cube([strSpc(pos)*(nStr-1),1.5,z],center=true);
translate([0,0,0])cylinder(d=holeDia,h=ovrZ*2+1,center=true,$fn=holeRes);
}

module pwrHoles(fret,pos,dia,z,part){
    if(fret%2==0)translate([(strSpc(pos)*1)/2-ledXY/2+ledPadY/2,trPwrOff+0.5,0])cylinder(d=dia,h=z*2,center=true,$fn=holeRes);
    if(fret%2==1)translate([-((strSpc(pos)*1)/2+ledXY/2-ledPadY/2),trPwrOff+0.6,0])cylinder(d=dia,h=z*2,center=true,$fn=holeRes);
     
    if(fret==0&&holeDia==dia)translate([-((strSpc(pos)*1)/2+ledXY/2-ledPadY/2),dia/2,0])cylinder(d=dia,h=z*2,center=true,$fn=holeRes);
    
    if(fret==0&&part==2)translate([-((strSpc(pos)*1)/2+ledXY/2-ledPadY/2),dia/2,0])cylinder(d=dia,h=z*2,center=true,$fn=holeRes);
    
    if(fret==0&&holeDia==dia)translate([0,fretPos(nFr)+fedFretOff+ledWPadsX/2-trTh/2+dataEndOff,0])cylinder(d=dia,h=z*2,center=true,$fn=holeRes);
     
}
module trPwr(fret,pos,part){
translate([-(strSpc(pos+trPwrOff)-strSpc(pos)),trPwrOff,0])cube([strSpc(pos+trPwrOff)*(nStr-1)+ledXY-ledPadY,ledPadY,z],center=true);

for(i=[0:nStr-1]){
    translate([0,0,0]){
        hull(){
            translate([strSpc(pos)*i-((strSpc(pos)*(nStr-1))/2)+ledXY/2-ledPadY/2,trPwrOff,0])cylinder(d=ledPadY,h=z,$fn=8);
            translate([strSpc(pos)*i-((strSpc(pos)*(nStr-1))/2)+ledXY/2-ledPadY/2,fedFretOff+ledXY/2,0])cylinder(d=ledPadY,h=z,$fn=8);
//translate([(strSpc(fretPos(fret+1))*(nStr-1))/2+ledXY/2-ledPadY/2,fretSpc(fret),0])cylinder(d=ledPadY,h=z,$fn=16);    
            }
        }
    if(fret<nFr)translate([0,0,0]){
        hull(){
            translate([strSpc(pos+trPwrOff)*i-((strSpc(pos+trPwrOff)*(nStr-1))/2)-ledXY/2+ledPadY/2,trPwrOff,0])cylinder(d=ledPadY,h=z,$fn=8);
            translate([strSpc(fretPos(fret+1))*i-((strSpc(fretPos(fret+1))*(nStr-1))/2)-ledXY/2+ledPadY/2,fretSpc(fret)+fedFretOff-ledXY/2,0])cylinder(d=ledPadY,h=z,$fn=8);   
            }
    }
}
//rotate([0,90,0])cylinder(d=1.5,h=strSpanB,center=true);
}

module firstPwr(dia){
    pos=0;
    fret=0;
    
    translate([-(strSpc(0+trPwrOff)-strSpc(0)),ledPadY/4,0])cube([strSpc(0+trPwrOff)*(nStr-1)+  ledXY-ledPadY,ledPadY/2,z],center=true);
    
    for(i=[0:nStr-1]){
         if(fret<nFr)translate([0,0,0]){
        hull(){
            translate([strSpc(fretPos(fret))*i-((strSpc(fretPos(fret))*(nStr-1))/2)-ledXY/2+        ledPadY/2,ledPadY/2,0])cylinder(d=ledPadY,h=z,$fn=8);
            translate([strSpc(fretPos(fret))*i-((strSpc(fretPos(fret))*(nStr-1))/2)-ledXY/2+        ledPadY/2,ledXY/2,0])cylinder(d=ledPadY,h=z,$fn=8);   
            }
            
        }
    }

    
}

module trData(){
    cube([trTh,ledWPadsX,z],center=true);
    translate([(ledPadYS+ledPadY)/4,ledPadXS/2+ledPadX-trTh/2,0])cube([(ledPadYS+ledPadY)/2,trTh,z],center=true);
    translate([-(ledPadYS+ledPadY)/4,-(ledPadXS/2+ledPadX-trTh/2),0])cube([(ledPadYS+ledPadY)/2,trTh,z],center=true);
}

module endData(pos,dia,part,z){
    smdResX=5;
    smdResY=2.2;
    off=dataEndOff;
    if(part==1)difference(){
        union(){
    translate([-(strSpc(pos+off)*2.5+fretStrPlus)/2+trTh,pos,0])cube([strSpc(pos+off)*2.5+fretStrPlus,trTh,z],center=true);
    translate([-(strSpc(pos+off)*2.5+fretStrPlus)/2+trTh,pos,0])cube([smdResX,smdResY,z],center=true);
    translate([0,pos,0])cylinder(d=dia,h=z,$fn=8);
        }
    translate([-(strSpc(pos+off)*2.5+fretStrPlus)/2+trTh,pos,0])cube([smdResX/3,smdResY+0.001,z+0.01],center=true);
        }
    if(part==2){
        translate([-(strSpc(pos+off)*2.5+fretStrPlus)/2+trTh,pos,0])cube([smdResX,smdResY,ledZ*2],center=true);
        translate([0,pos,0])cylinder(d=dia,h=ledZ*2,$fn=8,center=true);
    }
    }

module trDataC(fret,pos){
    if(fret%2==1){
    if(fret<nFr)translate([strSpc(pos)*2.5+ledXY/2+dataCOff-trTh/2,fedFretOff-ledWPadsX/2,0])rotate([0,0,-strAngle])cube([trTh,fretSpc(fret)+trTh,z]);
    
    if(fret<nFr)translate([strSpc(pos)*2.5+ledXY/2+dataCOff-dataCOff/2,-(ledPadXS/2+ledPadX)+trTh/2+fedFretOff,0])cube([dataCOff,trTh,z],center=true);
        
    translate([strSpc(fretPos(fret))*2.5+ledXY/2+dataCOff/2,fedFretOff-ledWPadsX/2+fretSpc(fret)+trTh/2,0])cube([dataCOff*2,trTh,z],center=true);
    }
    if(fret%2==0){
    if(fret<nFr)translate([-(strSpc(pos)*2.5+ledXY/2+dataCOff)-trTh/2,fedFretOff+ledWPadsX/2-trTh,0])rotate([0,0,strAngle])cube([trTh,fretSpc(fret)+trTh,z]);
    
    if(fret==nFr)translate([-(strSpc(pos)*2.5+ledXY/2+dataCOff)-trTh/2,fedFretOff+ledWPadsX/2-trTh,0])rotate([0,0,strAngle])cube([trTh,dataEndOff+trTh,z]);
    
    translate([-(strSpc(pos)*2.5+ledXY/2+dataCOff-dataCOff/2),(ledPadXS/2+ledPadX)-trTh/2+fedFretOff,0])cube([dataCOff,trTh,z],center=true);
        
    if(fret!=0)translate([-(strSpc(fretPos(fret-1))*2.5+ledXY/2+dataCOff-dataCOff/2),(ledPadXS/2+ledPadX)-trTh/2+fedFretOff-fretSpc(fret-1),0])cube([dataCOff,trTh,z],center=true);
    }

}

module trFret(fret,pos){
    if(fret<=nFr){
    for(i=[0:nStr-2]){
        translate([strSpc(pos)/2+strSpc(pos)*i-((strSpc(pos)*(nStr-1))/2),fedFretOff,0])trData();
    }
}
    translate([0,0,0])trDataC(fret,pos);
    //if(fret%2==1)scale([-1,1,1])translate([strSpc(pos)*2.5+ledXY/2+dataCOff,0,0])trDataC(fret,pos);
}

module frame(z,part){
    hull(){
    hull(){
        translate([0,5,z/2])cube([strSpcN*(nStr-1)+fretStrPlus*2,10,z],center=true);
        translate([0,fretPos(nFr)+lastFretSpc-1,z/2])cube([strSpc(fretPos(nFr)+lastFretSpc)*(nStr-1)+fretStrPlus*2,2,z],center=true);
    }
     if(part==2||part==3)hull(){
        translate([0,5,z-0.05])cube([strSpcN*(nStr-1)+fretStrPlus*2+mntSpcY*2,10,0.1],center=true);
        translate([0,fretPos(nFr)+lastFretSpc-1,z-0.05])cube([strSpc(fretPos(nFr)+lastFretSpc)*(nStr-1)+fretStrPlus*2+mntSpcY*2,2,0.1],center=true);
     }
     if(part==1||part==3)hull(){
        translate([0,5,0.05])cube([strSpcN*(nStr-1)+fretStrPlus*2+mntSpcY*2,10,0.1],center=true);
        translate([0,fretPos(nFr)+lastFretSpc-1,0.05])cube([strSpc(fretPos(nFr)+lastFretSpc)*(nStr-1)+fretStrPlus*2+mntSpcY*2,2,0.1],center=true);
    }
}
    
}
module ledBoard(part){
        for(i=[0:nFr]){
            translate([0,fretPos(i),0]){
                
                if(part==0||part==1){
                    if(i<=nFr)ledFret(fretPos(i),2);   
                    trPwr(i,fretPos(i));
                    trFret(i,fretPos(i));
                    translate([0,0,0])pwrHoles(i,fretPos(i),2.5,z);
                    }
                
                if(part==0||part==2){    
                    if(i<=22&&i!=0)color([1,1,0])translate([0,0,0])fret(fretPos(i));
                    //translate([0,0,0])pwrHoles(i,fretPos(i),2.5);
                    pwrHoles(i,fretPos(i),holeDia);
                    }          
            }
    }
    if(part==0||part==1)firstPwr(2.5);
    if(part==0||part==1)endData(fretPos(nFr)+fedFretOff+ledWPadsX/2-trTh/2+dataEndOff,2.5,2);
    if(part==3)color([0.5,0.5,1])frame(z,0);
}


module cutHalf(half,part){
    
    if(half==0){
        intersection(){
            ledBoard(part);
            translate([-100,0,0])cube([200,cutY,z]);
        }
    }
    if(half==1){
        translate([51,cutY*2-63,0])rotate([0,0,180]){
            intersection(){
                ledBoard(part);
                translate([-100,cutY,0])cube([200,scale,z]);
            }
        }
    }
}



module boardOver(){
    z=ovrZ;
    difference(){
        translate([0,0,0])color([0.5,0.5,1])frame(z,1);
        for(i=[0:nFr]){
            if(i>0&&i<=nFr-2)translate([0,fretPos(i),z/2+z-fretSnk])cube([200,fretY,z],center=true);
            translate([0,fretPos(i),0])ledFret(fretPos(i),3);
            translate([0,fretPos(i),0])pwrHoles(i,fretPos(i),3,ledZ,2);
            if(i>0&&i<=nFr-2)translate([0,fretPos(i),0])fret(fretPos(i),z);
        }
        endData(fretPos(nFr)+fedFretOff+ledWPadsX/2-trTh/2+dataEndOff,3,2,1);
        cDia=10;
        translate([0,cutY,-cDia/2+0.75])rotate([0,90,0])scale([1.05,1,1])cylinder(d=cDia,h=100,center=true);
        
    }
}

//projection(){
//    translate([25.5,-cutY,0]){
//        cutHalf(1,2);
//        cutHalf(0,2);
//    }
//}

//cutHalf(1,3);
//cutHalf(0,3);
//cutHalf(0,3);

//ledBoard(0);

//translate([0,0,slotXY/2+pcbZ])boardOver();

module brdCore(){
translate([0,0,-(slotXY/2)])frame(slotXY,3);
translate([0,0,slotXY/2])color([0.7,0.5,0.2])frame(pcbZ,3);
translate([0,0,slotXY/2+pcbZ])frame(ovrZ,1);
translate([0,0,-slotXY/2-mntBtmZ])color([0.5,0.5,1])frame(mntBtmZ,2);
}

module sideLck(){
    wT=mntSW*2;
    zOff=0;//((pcbZ+ovrZ)-mntBtmZ)/2
    res=160;
    difference(){
        union(){
        difference(){
        translate([0,0,zOff])scale([1,1,0.7])rotate([-90,0,0])cylinder(d1=wT+strSpcN*(nStr-1)+fretStrPlus*2,d2=wT+strSpc(fretPos(nFr)+lastFretSpc)*(nStr-1)+fretStrPlus*2,h=fretPos(nFr)+lastFretSpc+mntEndT,$fn=res);
            translate([-50,-1,0])cube([100,1000,50]);
        }
        difference(){
        translate([0,0,zOff])scale([1,1,0.9])rotate([-90,0,0])cylinder(d1=wT+strSpcN*(nStr-1)+fretStrPlus*2,d2=wT+strSpc(fretPos(nFr)+lastFretSpc)*(nStr-1)+fretStrPlus*2,h=fretPos(nFr)+lastFretSpc+mntEndT,$fn=res);
            translate([-50,-1,-50])cube([100,1000,50]);
        }
    }
        translate([-50,-1,slotXY/2+pcbZ+ovrZ])cube([100,1000,50]);
        translate([-50,-1,-50-slotXY/2-mntBtmZ])cube([100,1000,50]);
    
        }
}

module btmLck(){
    difference(){
    frame(mntBtmZ,2);
    mntPosYA=neck_mntPosYA+5;
    mntPosYB=neck_mntPosYB-5;
        height=100;

hull(){
translate([-(fretL(mntPosYA)/2-slotXY/2),mntPosYA,-height/2-0.1]){
    cylinder(d=4,h=height,$fn=12);
}
translate([-(fretL(mntPosYB)/2-slotXY/2),mntPosYB,-height/2-0.1]){
    cylinder(d=4,h=height,$fn=12);
}
}

hull(){
translate([fretL(mntPosYA)/2-slotXY/2,mntPosYA,-height/2-0.1]){
    cylinder(d=4,h=height,$fn=12);
}
translate([fretL(mntPosYB)/2-slotXY/2,mntPosYB,-height/2-0.1]){
    cylinder(d=4,h=height,$fn=12);
}
}

}
    difference(){
        l=409.3;
        scale([1,1,0.1])rotate([-90,0,0])cylinder(d1=strSpcN*(nStr-1)+fretStrPlus*2,d2=strSpc(l)*(nStr-1)+fretStrPlus*2,h=l,$fn=128);
    translate([-50,-1,0])cube([100,1000,50]);
    
    
    }
    

}

module neck(part){
translate([0,neckSlotY-(500-fretPos(22)),-slotXY/2])rotate([0,0,180]){
//translate([0,0,slotXY/2+pcbZ])boardOver();
//translate([0,0,slotXY/2+pcbZ])frame(ovrZ,1);
difference(){
union(){
    difference(){
        sideLck();
        brdCore();      
    }
    //translate([0,0,slotXY/2+pcbZ])boardOver();
    //translate([0,0,-slotXY/2-mntBtmZ])color([0.5,0.5,1])btmLck();
    
    
    
    
}
    for(i=[0:24]){
       if(i==0||i==2||i==4||i==6||i==8||i==11||i==14||i==18||i==21){
    rotate([0,0,-0.73])translate([(strSpcN*(nStr-1)+fretStrPlus*2)/2,0,0]){
        translate([mntSW,fretPos(i)+fretSpc(i)/1.75,0])mntSScrw(3.3,10,7,2.5,-90);   
    }
    rotate([0,0,0.73])translate([-((strSpcN*(nStr-1)+fretStrPlus*2)/2),0,0]){
        translate([-(mntSW),fretPos(i)+fretSpc(i)/1.75,0])mntSScrw(3.3,10,7,2.5,90); 
    }
}
}
   //translate([-50,fretPos(22)-7,-50])cube([100,1000,100]);
   if(part==0){
   translate([-50,fretPos(22),-50])scale([1,1,1])cube([100,1000,100]);
   }
   if(part==1){
   translate([-50,fretPos(22),-50])scale([1,-1,1])cube([100,1000,100]);
   translate([-50,fretPos(22),-slotXY/2])scale([1,1,-1])cube([100,1000,20]);
   }
}
}
}

module mntSScrw(dia,z,hDia,hZ,rY){
    rotate([0,rY,0]){
    $fn=12;
    cylinder(d1=hDia,d2=dia,h=hZ);
    cylinder(d=dia,h=z);
    }
}

module neckShp(){
translate([0,0,-slotXY/2])rotate([0,180,0]){
//translate([0,0,slotXY/2+pcbZ])boardOver();
//translate([0,0,slotXY/2+pcbZ])frame(ovrZ,1);
difference(){
translate([0,-0.1,0])union(){
    difference(){
        sideLck();
        brdCore();      
    }
    //translate([0,0,slotXY/2+pcbZ])boardOver();
    translate([0,0,slotXY/2+pcbZ])frame(ovrZ,1);
    translate([0,0,-slotXY/2-mntBtmZ])color([0.5,0.5,1])btmLck();
    
    
    
    
}
   translate([-50,0,-50])cube([100,1000,100]);
}
}
}

module mntSScrw(dia,z,hDia,hZ,rY){
    rotate([0,rY,0]){
    $fn=12;
    cylinder(d1=hDia,d2=dia,h=hZ);
    cylinder(d=dia,h=z);
    }
}

//neckShp();

module neckEnd(){
//neck(1);
slotBd_spacer();
}
//neckEnd();

module gnd(){
    cube([trTh,fretSpc(0),z]);
}
