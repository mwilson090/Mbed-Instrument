#include "mbed.h"
 
Serial pc(USBTX, USBRX);
Serial esp(p28, p27); // tx, rx
DigitalOut reset(p26);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
PwmOut led3(LED3);
DigitalOut led4(LED4);
Timer t;
 
int  count,ended,timeout;
char buf[2024];
char vol[4];
char snd[1024];
char* num;
 
char ssid[32] = "WyattiPhone";     // enter WiFi router ssid inside the quotes
char pwd [32] = "yjspsubscale"; // enter WiFi router password inside the quotes
 
void SendCMD(),getreply(),ESPconfig(),ESPsetbaudrate();
 void dev_recv()
{
    led1 = !led1;
    while(esp.readable()) {
        pc.putc(esp.getc());
    }
}
 
void pc_recv()
{
    led4 = !led4;
    while(pc.readable()) {
        esp.putc(pc.getc());
    }
}
 
 
int main()
{
    reset=0; //hardware reset for 8266
    pc.baud(9600);  // set what you want here depending on your terminal program speed
    pc.printf("\f\n\r-------------ESP8266 Hardware Reset-------------\n\r");
    wait(0.5);
    reset=1;
    timeout=2;
    getreply();
 
    esp.baud(9600);   // change this to the new ESP8266 baudrate if it is changed at any time.
 
    //ESPsetbaudrate();   //******************  include this routine to set a different ESP8266 baudrate  ******************
 
    ESPconfig();        //******************  include Config to set the ESP8266 configuration  ***********************
    timeout = 10;
    
    while(1){
        memset(buf, '\0', sizeof(buf));
        t.start();
        ended=0;
        count=0;
        while(!ended) {
            if(esp.readable()) {
                buf[count] = esp.getc();
                count++;
            }
            if(t.read() > timeout) {
                ended = 1;
                t.stop();
                t.reset();
            }
        }
        pc.printf("From ESP: %s\r\n", buf);
        
        if( strstr(buf, "action=START") != NULL ) {
            led1=1;
            wait(1);
            led1=0;
        }
        if( strstr(buf, "action=STOP") != NULL ) {
            led2=1;
            wait(1);
            led2=0;
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
            
            led3 = atoi(vol)/100;
            
        }
    }
    
 
}
 
// Sets new ESP8266 baurate, change the esp.baud(xxxxx) to match your new setting once this has been executed
void ESPsetbaudrate()
{
    strcpy(snd, "AT+CIOBAUD=115200\r\n");   // change the numeric value to the required baudrate
    SendCMD();
}
 
//  +++++++++++++++++++++++++++++++++ This is for ESP8266 config only, run this once to set up the ESP8266 +++++++++++++++
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
    count=0;
    while(!ended) {
        if(esp.readable()) {
            buf[count] = esp.getc();
            count++;
        }
        if(t.read() > timeout) {
            ended = 1;
            t.stop();
            t.reset();
        }
    }
    pc.printf("From ESP: %s\n", buf);
}
 
    
