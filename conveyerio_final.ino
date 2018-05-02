/* Program to automate a 2 conveyors as well as to 
   avoid collition and overflow of items on the conveyor */
int sensor[5]={2,3,4,5,6};//Integer array for storing sensor pins
int output[3]={10,11,12};//Integer array for storing output settings
int val[5]={0,0,0,0,0};//Integer array for storing sensor values
int start=0;//Variable to reset the program
int time=0;//Variable to store time
void setup()
{
  Serial.begin(9600);//Sets baud rate
  for(int i=0;i<5;i++)//Initializes sensor array as input
    pinMode(sensor[i],INPUT_PULLUP);
  
  //Defines output pins
  pinMode(output[0],OUTPUT);
  pinMode(output[1],OUTPUT);
  pinMode(output[2],OUTPUT);
  
  //Initializes LED indicator and sets it on
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  
  //Sets motors to off state
  digitalWrite(output[0],LOW);
  digitalWrite(output[1],LOW);
}

void blink() //Function to blink the LED on pin 13
{
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}

void readSensor()
{
  for(int i=0;i<5;i++)  //Reads sensor inputs & stores their value to val array
    val[i] = digitalRead(sensor[i]);
}

void loop()
{
  readSensor();
  /*for(int i=0;i<5;i++)
  {
    Serial.print(val[i]);
  }*/
  //Serial.print("\n");
  //delay(2000);
  while(!digitalRead(2)&&start==0) //Program initiator - Pauses program until object placed at sensor 1
  {
    blink();
    val[0]=digitalRead(sensor[0]);
  }
  start=1; 
  if(val[2]&&val[3]&&val[4]) //Checks if conveyor 2 is full
  {
    while(val[3]||val[4])  //Freezes program until objects cleared from sensor 4 & sensor 5
    {
      readSensor();
      delay(500);
      digitalWrite(output[1],LOW);   //Turns off conveyor 2
      if(val[1])                     //Checks if conveyor 1 is full
        digitalWrite(output[0],LOW); //If full turns off conveyor 1
      else
        digitalWrite(output[0],HIGH);//Else keeps conveyor 1 on
    }
  }
  else                             //If conveyors not full
  {
    digitalWrite(output[0],HIGH);  //Run conveyor 1&2
    delay(2000);
    digitalWrite(output[1],HIGH);
  }
  if((!(val[0]||val[1]||val[2]||val[3]||val[4]))||(val[3]&&val[4])) //If conveyors empty
  {
    if(time==0)  //Records time
      time=millis();
    else
    {
      int diff;
      diff = millis() - time;
      if(diff>30000) //If empty for more than 30 seconds resets the program
      {
        digitalWrite(output[0],LOW);
        digitalWrite(output[1],LOW);
        start=0;
        time=0;
        blink();
      }
    }
  }
  else
    time=0;
  delay(50);
  digitalWrite(13,HIGH);//Turns on indicator LED
}

      
