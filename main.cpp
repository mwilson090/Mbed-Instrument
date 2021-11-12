#include "mbed.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
 
 
uLCD_4DGL uLCD(p13, p14, p30); // create a global uLCD object
 
Serial pc(USBTX,USBRX); //not used in this program

PwmOut speaker(p21);

//PinDetect pb1(p15);
//PinDetect pb2(p16);

int main() {
   
   // you can play around with the parameters to see the response
   int radius = 10;
   int offsetx = 63;
   int offsety = 63;
   double factor = 50;
   double music_factor = 200;
   bool MusicOn = false;
   
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
      } 
      else if(x < B){
        speaker.period(1.0/988); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < bFlat){
        speaker.period(1.0/932); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < A){
        speaker.period(1.0/880); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < aFlat){
        speaker.period(1.0/831); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < G){
        speaker.period(1.0/784); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < gFlat){
        speaker.period(1.0/740); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < F){
        speaker.period(1.0/698); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < E){
        speaker.period(1.0/659); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < eFlat){
        speaker.period(1.0/622); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < D){
        speaker.period(1.0/587); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < dFlat){
        speaker.period(1.0/554); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else if(x < C){
        speaker.period(1.0/523); 
        speaker =0.25; //25% duty cycle - mid range volume
      }
      else {
          speaker=0.0;
      }
      
      wait(3);
      
      uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, WHITE);
      /*
      if (MusicOn)
           mySpeaker.PlayNote(440.0+x*music_factor,0.25+0.2*y,0.05);
      
      if (pb1 == false)
           MusicOn = true;
           
      if (pb2 == false)
            MusicOn = false;
        */        
                 
      // You can uncomment this line to see the values coming off the MMA8452           
      //uLCD.printf("\n(%.2f,%.2f,%.2f)  \n", x,y,z);
      
      } //end else
      
   } //end infinite while loop
} // end main