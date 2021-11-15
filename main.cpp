#include "mbed.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"

PwmOut red(p25);
PwmOut green(p24);
PwmOut blue(p23);

PinDetect pb(p22);
 
uLCD_4DGL uLCD(p13, p14, p30); // create a global uLCD object
 
Serial pc(USBTX,USBRX); //not used in this program

PwmOut speaker(p21);

void rest(){
    speaker = 0.0; //turn off speaker for rest
    red = 0.0;
    green = 0.0;
    blue = 0.0;
}

int main() {
   pb.mode(PullUp);
   
   pb.attach_asserted(&rest); //rest when button pushed

   #include "mbed.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"

PwmOut red(p25);
PwmOut green(p24);
PwmOut blue(p23);

PinDetect pb(p22);
 
uLCD_4DGL uLCD(p13, p14, p30); // create a global uLCD object
 
Serial pc(USBTX,USBRX); //not used in this program

PwmOut speaker(p21);

void rest(){
    speaker = 0.0; //turn off speaker for rest
    red = 0.0;
    green = 0.0;
    blue = 0.0;
}

int main() {
   pb.mode(PullUp);
   
   pb.attach_asserted(&rest); //rest when button pushed

   pb.setSampleFrequency(); 
   
   // you can play around with the parameters to see the response
   int radius = 10;
   int offsetx = 63;
   int offsety = 63;
   double factor = 50;
   
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
   
   //set the push buttons that control sounds
   //pb1.mode(PullUp);
   //pb2.mode(PullUp);
   //I will not use interupts like in jazzy tunes
 
   
   double x = 0, y = 0, z = 0;
 
   MMA8452 acc(p9, p10, 40000);  //instantiate an acc object! 
   
   //set parameters -- use these and don't change
   acc.setBitDepth(MMA8452::BIT_DEPTH_12);
   acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
   acc.setDataRate(MMA8452::RATE_100);
   
   
   while(1) {
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
       
      if(!acc.isXYZReady()) 
      {
         wait(0.01);
      }
      else
      { 
      
      acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
      
      pc.printf("%f, %f, %f\n", x, y, z);
      
      if(x < highC){
        speaker.period(1.0/1047); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =1.0;
      } 
      else if(x < B){
        speaker.period(1.0/988); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.0;
      }
      else if(x < bFlat){
        speaker.period(1.0/932); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.5;
      }
      else if(x < A){
        speaker.period(1.0/880); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < aFlat){
        speaker.period(1.0/831); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.0;
        blue =1.0;
      }
      else if(x < G){
        speaker.period(1.0/784); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < gFlat){
        speaker.period(1.0/740); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.5;
        blue =0.5;
      }
      else if(x < F){
        speaker.period(1.0/698); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =1.0;
      }
      else if(x < E){
        speaker.period(1.0/659); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =1.0;
        blue =0.0;
      }
      else if(x < eFlat){
        speaker.period(1.0/622); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =1.0;
      }
      else if(x < D){
        speaker.period(1.0/587); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.5;
        blue =1.0;
      }
      else if(x < dFlat){
        speaker.period(1.0/554); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.5;
      }
      else if(x < C){
        speaker.period(1.0/523); 
        speaker = 0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =0.0;
        blue =1.0;
      }
      else {
          speaker =0.0;
          red =0.0;
          green =0.0;
          blue =0.0;
      }
      
      wait(3);
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
      
      } //end else
      
   } //end infinite while loop
} // end main
   
   // you can play around with the parameters to see the response
   int radius = 10;
   int offsetx = 63;
   int offsety = 63;
   double factor = 50;
   
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
   
   //set the push buttons that control sounds
   //pb1.mode(PullUp);
   //pb2.mode(PullUp);
   //I will not use interupts like in jazzy tunes
 
   
   double x = 0, y = 0, z = 0;
 
   MMA8452 acc(p9, p10, 40000);  //instantiate an acc object! 
   
   //set parameters -- use these and don't change
   acc.setBitDepth(MMA8452::BIT_DEPTH_12);
   acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
   acc.setDataRate(MMA8452::RATE_100);
   
   
   while(1) {
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
       
      if(!acc.isXYZReady()) 
      {
         wait(0.01);
      }
      else
      { 
      
      acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
      
      pc.printf("%f, %f, %f\n", x, y, z);
      
      if(x < highC){
        speaker.period(1.0/1047); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =1.0;
      } 
      else if(x < B){
        speaker.period(1.0/988); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.0;
      }
      else if(x < bFlat){
        speaker.period(1.0/932); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.5;
      }
      else if(x < A){
        speaker.period(1.0/880); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < aFlat){
        speaker.period(1.0/831); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.0;
        blue =1.0;
      }
      else if(x < G){
        speaker.period(1.0/784); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < gFlat){
        speaker.period(1.0/740); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.5;
        blue =0.5;
      }
      else if(x < F){
        speaker.period(1.0/698); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =1.0;
      }
      else if(x < E){
        speaker.period(1.0/659); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =1.0;
        blue =0.0;
      }
      else if(x < eFlat){
        speaker.period(1.0/622); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =1.0;
      }
      else if(x < D){
        speaker.period(1.0/587); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.5;
        blue =1.0;
      }
      else if(x < dFlat){
        speaker.period(1.0/554); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.5;
      }
      else if(x < C){
        speaker.period(1.0/523); 
        speaker = 0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =0.0;
        blue =1.0;
      }
      else {
          speaker =0.0;
          red =0.0;
          green =0.0;
          blue =0.0;
      }
      
      wait(3);
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
      
      } //end else
      
   } //end infinite while loop
} // end main
   
   // you can play around with the parameters to see the response
   int radius = 10;
   int offsetx = 63;
   int offsety = 63;
   double factor = 50;
   
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
   
   //set the push buttons that control sounds
   //pb1.mode(PullUp);
   //pb2.mode(PullUp);
   //I will not use interupts like in jazzy tunes
 
   
   double x = 0, y = 0, z = 0;
 
   MMA8452 acc(p9, p10, 40000);  //instantiate an acc object! 
   
   //set parameters -- use these and don't change
   acc.setBitDepth(MMA8452::BIT_DEPTH_12);
   acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
   acc.setDataRate(MMA8452::RATE_100);
   
   
   while(1) {
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
       
      if(!acc.isXYZReady()) 
      {
         wait(0.01);
      }
      else
      { 
      
      acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
      
      pc.printf("%f, %f, %f\n", x, y, z);
      
      if(x < highC){
        speaker.period(1.0/1047); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =1.0;
      } 
      else if(x < B){
        speaker.period(1.0/988); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.0;
      }
      else if(x < bFlat){
        speaker.period(1.0/932); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.5;
      }
      else if(x < A){
        speaker.period(1.0/880); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < aFlat){
        speaker.period(1.0/831); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.0;
        blue =1.0;
      }
      else if(x < G){
        speaker.period(1.0/784); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =0.0;
      }
      else if(x < gFlat){
        speaker.period(1.0/740); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.5;
        blue =0.5;
      }
      else if(x < F){
        speaker.period(1.0/698); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =1.0;
        blue =1.0;
      }
      else if(x < E){
        speaker.period(1.0/659); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =1.0;
        blue =0.0;
      }
      else if(x < eFlat){
        speaker.period(1.0/622); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =1.0;
      }
      else if(x < D){
        speaker.period(1.0/587); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =0.5;
        green =0.5;
        blue =1.0;
      }
      else if(x < dFlat){
        speaker.period(1.0/554); 
        speaker =0.25; //25% duty cycle - mid range volume
        red =1.0;
        green =0.0;
        blue =0.5;
      }
      else if(x < C){
        speaker.period(1.0/523); 
        speaker = 0.25; //25% duty cycle - mid range volume
        red =0.0;
        green =0.0;
        blue =1.0;
      }
      else {
          speaker =0.0;
          red =0.0;
          green =0.0;
          blue =0.0;
      }
      
      wait(3);
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
      
      } //end else
      
   } //end infinite while loop
} // end main