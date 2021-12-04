#include "mbed.h"
#include "MMA8452.h"
#include "PinDetect.h"

PwmOut red(p25);
PwmOut green(p24);
PwmOut blue(p23);

PinDetect pb(p22);
 
Serial pc(USBTX,USBRX); //not used in this program

PwmOut speaker(p21);

char Note;

int ind;

char Song[50];

double noteRange = 0.1538;
double highC = -0.923;
double B = highC + noteRange;
double bFlat = B + noteRange;
double A = bFlat + noteRange;
double aFlat = A + noteRange;
double G = aFlat + noteRange;
double gFlat = G + noteRange;
double F = gFlat + noteRange;
double E = F + noteRange;
double eFlat = E + noteRange;
double D = eFlat + noteRange;
double dFlat = D + noteRange;
double C = dFlat + noteRange;

double x = 0, y = 0, z = 0;

void setNote(){
    if(x < highC){
        red =1.0;
        green =1.0;
        blue =1.0;
        Note = 'h'; 
      } 
      else if(x < B){
        red =1.0;
        green =0.0;
        blue =0.0;
        Note = 'B';
      }
      else if(x < bFlat){
        red =0.0;
        green =1.0;
        blue =0.5;
        Note = 'b'; 
      }
      else if(x < A){ 
        red =1.0;
        green =1.0;
        blue =0.0;
        Note = 'A'; 
      }
      else if(x < aFlat){
        red =0.5;
        green =0.0;
        blue =1.0;
        Note = 'a'; 
      }
      else if(x < G){
        red =0.0;
        green =1.0;
        blue =0.0;
        Note = 'G'; 
      }
      else if(x < gFlat){
        red =1.0;
        green =0.5;
        blue =0.5;
        Note = 'g'; 
      }
      else if(x < F){
        red =0.0;
        green =1.0;
        blue =1.0;
        Note = 'F'; 
      }
      else if(x < E){ 
        red =0.5;
        green =1.0;
        blue =0.0;
        Note = 'E'; 
      }
      else if(x < eFlat){
        red =1.0;
        green =0.0;
        blue =1.0;
        Note = 'e'; 
      }
      else if(x < D){ 
        red =0.5;
        green =0.5;
        blue =1.0;
        Note = 'D'; 
      }
      else if(x < dFlat){ 
        red =1.0;
        green =0.0;
        blue =0.5;
        Note = 'd'; 
      }
      else if(x < C){
        red =0.0;
        green =0.0;
        blue =1.0;
        Note = 'C'; 
      }
      else {
        red =0.0;
        green =0.0;
        blue =0.0;
        Note = 'r'; 
      }
      
      //override note- rest if y < -0.5
      if(y < -0.5){
        red = 0.0;
        green = 0.0;
        blue = 0.0;
        Note = 'r'; 
      }
}

void playNote(){
    //set period based on note- only if the note is different
    if(ind==0||(ind>0 && Note != Song[ind-1])){
        switch(Note){
            case 'A':
                speaker.period(1.0/880); 
                break;
            case 'a':
                speaker.period(1.0/831); 
                break;
            case 'B':
                speaker.period(1.0/988); 
                break;
            case 'b':
                speaker.period(1.0/932); 
                break;
            case 'C':
                speaker.period(1.0/523); 
                break;
            case 'c':
                break;
            case 'D':
                speaker.period(1.0/587); 
                break;
            case 'd':
                speaker.period(1.0/554); 
                break;
            case 'E':
                speaker.period(1.0/659);
                break;
            case 'e':
                speaker.period(1.0/622); 
                break;
            case 'F':
                speaker.period(1.0/698); 
                break;
            case 'G':
                speaker.period(1.0/784); 
                break;
            case 'g':
                speaker.period(1.0/740);
                break;
            case 'h':
                speaker.period(1.0/1047); 
                break;
            case 'r':
                speaker = 0.0;
                break;
        }
    }

    //set volume based on y-axis position
    if(y < -0.5){
      speaker = 0.0;
    }
    else if(y > 0){
      speaker = y/2; //volume range ~0-50% duty cycle
    }
        
}

void recordNote(){
    if(ind==0||(ind>0 && Note != Song[ind-1])){
        Song[ind] = Note;
        ind++;
    } 
}
            
int main() {
   pb.mode(PullUp);
 
   MMA8452 acc(p9, p10, 40000);  //instantiate an acc object! 
   
   //set parameters -- use these and don't change
   acc.setBitDepth(MMA8452::BIT_DEPTH_12);
   acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
   acc.setDataRate(MMA8452::RATE_100);
   
   while(1){
       ind = 0;
       memset(Song, 0, sizeof(Song)); //reset Song array
       
       while(ind < 50) {
          
          //uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
           
          if(!acc.isXYZReady()) 
          {
             wait(0.01);
          }
          else
          { 
          
          acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
          
          setNote(); //find note based on position and show it on led
          
          //if button pressed, play note
          if(!pb){
            playNote();
          }
          else {
              Note = 'r';
              speaker = 0.0;
          }
          
          recordNote();
          
          wait(0.1); //0.01 was giving a very bad sound!
          
          } //end else 
       } //end infinite while loop

       //print final song!
       speaker = 0.0;
       red = 1.0;
       green = 1.0;
       blue = 1.0;
       pc.printf("%s\r\n", Song);
       wait(3);
    }
} // end main