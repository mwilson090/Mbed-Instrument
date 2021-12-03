#include "mbed.h"
#include "MMA8452.h"
#include "PinDetect.h"

PwmOut red(p25);
PwmOut green(p24);
PwmOut blue(p23);

PinDetect pb(p22);
 
Serial pc(USBTX,USBRX); //not used in this program

PwmOut speaker(p21);

double Song[100];

double highC = -0.923;
double B = highC + 0.0769;
double bFlat = B + 0.0769;
double A = bFlat + 0.0769;
double aFlat = A + 0.0769;
double G = aFlat + 0.0769;
double gFlat = G + 0.0769;
double F = gFlat + 0.0769;
double E = F + 0.0769;
double eFlat = E + 0.0769;
double D = eFlat + 0.0769;
double dFlat = D + 0.0769;
double C = dFlat + 0.0760;

double x = 0, y = 0, z = 0;

void rest(){
    while(pb){
        speaker = 0.0; //turn off speaker for rest
        red = 0.0;
        green = 0.0;
        blue = 0.0;
    }
}

void setNote(int i){
    if(x < highC){
        speaker.period(1.0/1047); 
        red =1.0;
        green =1.0;
        blue =1.0; 
      } 
      else if(x < B){
        speaker.period(1.0/988); 
        red =1.0;
        green =0.0;
        blue =0.0;
      }
      else if(x < bFlat){
        speaker.period(1.0/932); 
        red =0.0;
        green =1.0;
        blue =0.5;
      }
      else if(x < A){
        speaker.period(1.0/880); 
        red =1.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < aFlat){
        speaker.period(1.0/831); 
        red =0.5;
        green =0.0;
        blue =1.0;
      }
      else if(x < G){
        speaker.period(1.0/784); 
        red =0.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < gFlat){
        speaker.period(1.0/740); 
        red =1.0;
        green =0.5;
        blue =0.5;
      }
      else if(x < F){
        speaker.period(1.0/698); 
        red =0.0;
        green =1.0;
        blue =1.0;
      }
      else if(x < E){
        speaker.period(1.0/659); 
        red =0.5;
        green =1.0;
        blue =0.0;
      }
      else if(x < eFlat){
        speaker.period(1.0/622); 
        red =1.0;
        green =0.0;
        blue =1.0;
      }
      else if(x < D){
        speaker.period(1.0/587); 
        red =0.5;
        green =0.5;
        blue =1.0;
      }
      else if(x < dFlat){
        speaker.period(1.0/554); 
        red =1.0;
        green =0.0;
        blue =0.5;
      }
      else if(x < C){
        speaker.period(1.0/523); 
        red =0.0;
        green =0.0;
        blue =1.0;
      }
      else {
        red =0.0;
        green =0.0;
        blue =0.0;
        speaker = 0.0;
      }
      
      //set volume based on y-axis position
      if(y < 0){
        speaker = 0.0;
        red =0.0;
        green =0.0;
        blue =0.0;
      }
      else {
        speaker = y/2; //volume range ~0-50% duty cycle
      }
      Song[i] = x;
}

int main() {
   pb.mode(PullUp);
   
   pb.attach_asserted(&rest); //rest when button pushed

   pb.setSampleFrequency(); 
 
   MMA8452 acc(p9, p10, 40000);  //instantiate an acc object! 
   
   //set parameters -- use these and don't change
   acc.setBitDepth(MMA8452::BIT_DEPTH_12);
   acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
   acc.setDataRate(MMA8452::RATE_100);
   
   int i;
   while(i < 100) {
      
      //uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
       
      if(!acc.isXYZReady()) 
      {
         wait(0.01);
      }
      else
      { 
      
      acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
      
      setNote(i);
      
      i++;
      
      pc.printf("%f, %f, %f\n\n", x, y, z);
      
      wait(1);
      
      //uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
      
      } //end else 
   } //end infinite while loop
   for(int i=0; i<100; i++){
        pc.printf("Note %f: %f\n\n", i, Song[i]);
    }
} // end main