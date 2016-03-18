#include <IRremote.h>

int RECV_PIN = 11;
IRsend irsend;
IRrecv irrecv(RECV_PIN);
int count = 0;
int set = 1; 
decode_results results;
void setup()
{
Serial.begin(9600);
irrecv.enableIRIn();  
TCCR0A = 0;    
TCCR0B = 0;    
TIMSK0 = (1 << TOIE0);
}
ISR(TIMER0_OVF_vect)
{
  count = count + 16;
  if (count >= 3072)
  {
   Serial.println("re enabling pin 3");
   TCCR0B = 0x00;
   set = 1;
   count = 0;
  }
}
void loop() {
if (set==1)
 {
send();
set=0;
TCNT0= 0x0BDC;
TCCR0B = (1 << CS12) | (1 << CS10);
}
 if (irrecv.decode(&results)) {
  Serial.println(results.value, HEX);
  irsend.stop(); // disabling the pin 3   
  set=0;
  TCNT0= 0x0BDC;
  TCCR0B = (1 << CS12) | (1 << CS10);
  irrecv.resume();
 }
}

void send()
{ 
  for(int i=0;i<3;i++)
  {
    irsend.sendSony(0x123,12);
  }
}

