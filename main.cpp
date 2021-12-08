#include "mbed.h"
#include "MMA8452.h"
#include "PinDetect.h"

PwmOut red(p25);
PwmOut green(p24);
PwmOut blue(p23);

PinDetect pb(p22);

Serial pc(USBTX,USBRX); //not used in this program

MMA8452 acc(p28, p27, 100000);  //instantiate an acc object! 

//Internet stuff
Serial esp(p9, p10); // ESP tx, rx

DigitalOut reset(p26);// ESP Reset
DigitalOut led1(LED1);//Visual aid
DigitalOut led2(LED2);//Visual aid
PwmOut led3(LED3); // Visual representation of Volume
DigitalOut led4(LED4);//Visual aid

Timer t; //Timer for ESP timeouts

PwmOut speaker(p21);


//Variables
int  bytesRead=0;
int ended;
int timeout; //ESP related
char buf[2024];     //ESP input buffer
char vol[4];        //Volume control
float vol_f;
char snd[1024];     //CMD buffer for ESP
char* num;      // strstr return pointer

bool MUSIC_DEVICE_ENABLED = false;

char ssid[32] = "WyattiPhone";     // WiFi ssid
char pwd [32] = "yjspsubscale";    // WiFi password

void SendCMD(),getreply(),ESPconfig(); //Function definitions

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
            default:
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

void ESPconfig()
{

    wait(5);
    pc.printf("\f---------- Starting ESP Config ----------\r\n\n");
    strcpy(snd,".\r\n.\r\n");
    SendCMD();
    wait(1);
    pc.printf("---------- Reset & get Firmware ----------\r\n");
    strcpy(snd,"node.restart()\r\n");
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(buf);

    wait(2);

    pc.printf("\n---------- Get Version ----------\r\n");
    strcpy(snd,"print(node.info())\r\n");
    SendCMD();
    timeout=4;
    getreply();
    pc.printf(buf);

    wait(3);

    // set CWMODE to 1=Station,2=AP,3=BOTH, default mode 1 (Station)
    pc.printf("\n---------- Setting Mode ----------\r\n");
    strcpy(snd, "wifi.setmode(wifi.STATION)\r\n");
    SendCMD();
    timeout=4;
    getreply();
    pc.printf(buf);

    wait(2);



    pc.printf("\n---------- Listing Access Points ----------\r\n");
    strcpy(snd, "function listap(t)\r\n");
    SendCMD();
    wait(0.2);
    strcpy(snd, "for k,v in pairs(t) do\r\n");
    SendCMD();
    wait(0.2);
    strcpy(snd, "print(k..\" : \"..v)\r\n");
    SendCMD();
    wait(0.2);
    strcpy(snd, "end\r\n");
    SendCMD();
    wait(0.2);
    strcpy(snd, "end\r\n");
    SendCMD();
    wait(0.2);
    strcpy(snd, "wifi.sta.getap(listap)\r\n");
    SendCMD();
    wait(0.2);
    timeout=15;
    getreply();
    pc.printf(buf);

    wait(0.5);

    pc.printf("\n---------- Connecting to AP ----------\r\n");
    pc.printf("ssid = %s   pwd = %s\r\n",ssid,pwd);
    strcpy(snd, "wifi.sta.config(\"");
    strcat(snd, ssid);
    strcat(snd, "\",\"");
    strcat(snd, pwd);
    strcat(snd, "\")\r\n");
    SendCMD();
    timeout=10;
    getreply();
    pc.printf(buf);

    wait(2);

    pc.printf("\n---------- Get IP's ----------\r\n");
    strcpy(snd, "print(wifi.sta.getip())\r\n");
    SendCMD();
    timeout=1;
    getreply();
    pc.printf(buf);

    wait(1);

    pc.printf("\n---------- Get Connection Status ----------\r\n");
    strcpy(snd, "print(wifi.sta.status())\r\n");
    SendCMD();
    timeout=5;
    getreply();
    pc.printf(buf);

    pc.printf("\n\n\n  If you get a valid (non zero) IP, ESP8266 has been set up.\r\n");
    pc.printf("  Run this if you want to reconfig the ESP8266 at any time.\r\n");
    pc.printf("  It saves the SSID and password settings internally\r\n");
    wait(1);

    timeout=5;

    pc.printf("Starting server creation...\n");
    //create server
    strcpy(snd, "srv=net.createServer(net.TCP)\r\n");
    SendCMD();
    getreply();
    wait(1);
    strcpy(snd,"srv:listen(80,function(conn)\r\n");
    SendCMD();
    getreply();
    wait(1);
    strcpy(snd,"conn:on(\"receive\",function(conn,payload) \r\n");
    SendCMD();
    getreply();
    wait(1);

    //print data to mbed
    strcpy(snd,"print(payload)\r\n");
    SendCMD();
    getreply();
    wait(1);


    //Create HTML
    strcpy(snd, "conn:send('<!DOCTYPE html><html><body><h1>Mbed Music Device Controls</h1>')\r\n");
    SendCMD();
    getreply();
    wait(1);

    strcpy(snd,"conn:send('<form method=\"POST\">')\r\n");
    SendCMD();
    getreply();
    wait(1);

        strcpy(snd,"conn:send('<p><input type=\"radio\" id=\"start\" name=\"action\" value=\"START\"></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><label for=\"start\">Start</label><br></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><input type=\"radio\" id=\"stop\" name=\"action\" value=\"STOP\"></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><label for=\"stop\">Stop</label><br></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><label for=\"vol\">Volume (0-100):</label></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><input type=\"range\" id=\"vol\" name=\"vol\" min=\"0\" max=\"100\"></p>')\r\n");
        SendCMD();
        getreply();
        wait(1);
        strcpy(snd,"conn:send('<p><input type=\"submit\" value=\"Refresh\"></p></form>')\r\n");
        SendCMD();
        getreply();
        wait(1);
    strcpy(snd,"conn:send('<p><h2>How to use:</h2><ul><li>Click Start to start</li><li>Click Stop to stop</li><li>Click Refresh to update volume data</li></ul></body></html>')\r\n");
    SendCMD();
    getreply();
    wait(1);
    // end web page data
    strcpy(snd, "conn:on(\"sent\",function(conn) conn:close() end)\r\n"); // close current connection
    SendCMD();
    getreply();
    wait(1);
    strcpy(snd, "end)\r\n");
    SendCMD();
    getreply();
    wait(1);
    strcpy(snd, "end)\r\n");
    SendCMD();
    getreply();
    wait(1);
    /*
    strcpy(snd,"end)\r\n");
    SendCMD();
    getreply();
    wait(1);
    */  
    pc.printf("\n\n++++++++++ Ready ++++++++++\r\n\n");
}

void SendCMD()
{
    esp.printf("%s", snd);
}
 
void getreply()
{
    memset(buf, '\0', sizeof(buf));
    t.start();
    ended=0;
    bytesRead=0;
    while(!ended) {
        if(esp.readable() > 0) {
            buf[bytesRead] = esp.getc();
            bytesRead++;
        }
        if(t.read() > timeout) {
            ended = 1;
            t.stop();
            t.reset();
        }
    }
    pc.printf("From ESP: %s\r\n", buf);
}

void readWebData(){
    memset(buf, '\0', sizeof(buf));
    //pc.printf("ESP readable?: %d bytesReader: %d\r\n", esp.readable(), ++bytesReader);
    timeout = 3;
    if (esp.readable() > 0){
        t.start();
        ended=0;
        bytesRead=0;
        while(!ended) {
            if(esp.readable() > 0) {
                buf[bytesRead] = esp.getc();
                bytesRead++;
            }
            if(t.read() > timeout) {
                ended = 1;
                t.stop();
                t.reset();
            }
        }   
        
        pc.printf("From Webpage: %s\r\n", buf);
        
        if( strstr(buf, "action=START") != NULL ) {
            led1=1;
            led2=1;
            led4=1;
            MUSIC_DEVICE_ENABLED = true;
            acc.activate();
        }
        if( strstr(buf, "action=STOP") != NULL ) {
            led1=0;
            led2=0;
            led3=0;
            led4=0;
            red = 0.0;
            green = 0.0;
            blue = 0.0;
            MUSIC_DEVICE_ENABLED = false;
            speaker = 0.0;
            acc.standby();
        }
        if( strstr(buf, "vol") != NULL ) {
            num = strstr(buf, "vol");
            vol[0] = *(num+4);
            vol[1] = *(num+5);
            if (*(num+6) == '\r'){
            vol[2] = '\0';    
            } else {
            vol[2] = *(num+6);
            vol[3] = '\0';   
            }
            
            pc.printf("received volume: %s\r\n", vol);
            
            led3 = atof(vol)/100.0;
            vol_f = atof(vol)/100.0;
            //Set the volume
            if(MUSIC_DEVICE_ENABLED){
                speaker = vol_f;
            }
        }
    }
    //pc.printf("Finished checking\r\n");
}
            
int main() {
    pb.mode(PullUp);

    //set parameters -- use these and don't change
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_2G);
    acc.setDataRate(MMA8452::RATE_800);

    acc.activate();
    //Setup webpage
    pc.baud(9600);  // set what you want here depending on your terminal program speed
    pc.printf("\f\n\r-------------ESP8266 Hardware Reset-------------\n\r");
    wait(0.5);
    reset=1;
    timeout=2;
    getreply();

    esp.baud(9600);
    ESPconfig();   
   
    while(1){
        readWebData();
        
        ind = 0;
        memset(Song, 0, sizeof(Song)); //reset Song array

        while(ind < 50) {
            if (MUSIC_DEVICE_ENABLED){
                //uLCD.circle(-1*y*factor+offsety, -1*x*factor+offsetx, radius, BLACK);
                acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers

                setNote(); //find note based on position and show it on led
                
                //if button pressed, play note
                if(!pb){
                    playNote();
                } else {
                    Note = 'r';
                    speaker = 0.0;
                }

                recordNote();
                pc.printf("Note, Index, X, isReady, status: %c, %d, %f, %d\r\n", Note, ind, x, acc.isXYZReady());

                //Because checking the webpage may take time, we 
                //may not want to wait any time at all
                wait(0.15); //0.01 was giving a very bad sound!

                //check for new webpage data  
            }
            readWebData();
        } //end 50 while 

        acc.standby();
        //print final song!
        speaker = 0.0;
        red = 1.0;
        green = 1.0;
        blue = 1.0;
        pc.printf("%s\r\n", Song);
        acc.activate();
        wait(3);
        
    }
} // end main
