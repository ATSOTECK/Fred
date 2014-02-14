/*
  UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <Servo.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 45454;      // local port to listen on
Servo servos [8];
int   desired[8] = {90,90,90,90,90,90,90,90};
int   at     [8] = {90,90,90,90,90,90,90,90};
boolean sent = false;



// buffers for receiving and sending data
char packetBuffer[/*UDP_TX_PACKET_MAX_SIZE*/70]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac);
  Udp.begin(localPort);
  Serial.begin(9600);
  Serial.print("starting");
  
  servos[0].attach(2);
  servos[1].attach(3);
  servos[2].attach(4);
  servos[3].attach(5);
  servos[4].attach(6);
  servos[5].attach(7);
  servos[6].attach(8);
  servos[7].attach(9);
  
  
}

void setMotorsFromHTTP(String http)
{
  int val;
  int servoIndex = -1;
  char c;
  
  for (int n = 0; n < http.length(); n ++)
  {
    c = http.charAt(n);
    
    if (c == '?' || c == '=') continue;
    
    if (c == 'm')
    {
      val = 0;
      servoIndex ++;
      
      n++;
      
      servoIndex = http.charAt(n) - '0';
    }
    else if (c >= '0' && c <= '9')
    {
      val = val * 10 + (c - '0');
    }
      else if (c == '&' || c == '#')
    {
    //servos[servoIndex].write(val);
      
    desired[servoIndex] = val;
    Serial.println("Servo: " + String(servoIndex) + " -> " + String(val) );
    if (c == '#')
      break;
    }
  }
}



void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    sent = true;
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,/*UDP_TX_PACKET_MAX_SIZE*/70);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    Serial.println("");
    Serial.println("");
    
   
    setMotorsFromHTTP(String(packetBuffer));
    
  
  
    //servo.write(getVar("speed",packetBuffer).toInt());

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  delay(10);
  
  if (sent)
  {
     for(int x = 0; x<8;x++)
    {
      if (abs(at[x] - desired[x]) < 7 )
      {
         at[x] = desired[x]; 
      }
      
       if(at[x] > desired[x])
       {
          at[x] -= (at[x]-desired[x])/6;
       } 
       if(at[x] < desired[x])
       {
          at[x] += (desired[x] - at[x])/6; 
       }
       if (desired[x] == 90)
         at[x] = 90;
         
       servos[x].write(180 - at[x]);
    } 
  }
  
}


/*
  Processing sketch to run with this example
 =====================================================
 
 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send the "Hello Arduino" message
 
 
 import hypermedia.net.*;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true ); 		// <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 }
 
 void draw()
 {
 }
 
 void keyPressed() {
 String ip       = "192.168.1.177";	// the remote IP address
 int port        = 8888;		// the destination port
 
 udp.send("Hello World", ip, port );   // the message to send
 
 }
 
 void receive( byte[] data ) { 			// <-- default handler
 //void receive( byte[] data, String ip, int port ) {	// <-- extended handler
 
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 }
 */


