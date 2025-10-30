//women security big antinna
 
 #include<SoftwareSerial.h>
 //make RX arduino line is pin 2, make TX arduino line is pin 3.
SoftwareSerial gps(7,8);
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,2,3,4,5);
//String str="";
#define sensor A0
float tempc;
float vout;
String buffer;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";
int s1p,x,y,x1,y1;
int temp=0;
int i=0,k=0,nt=0;
#define buz 13
#define led1 6
#define led2 3
int temperature;
void setup() 
{
  lcd.begin(16,2);
  Serial.begin(9600);
  gps.begin(9600);
  lcd.print("WOMEN SAFETY  ");
  lcd.setCursor(0,1);
  lcd.print("AND SECYRITY    ");
  delay(2000);
   gsm_init();
  lcd.clear();
  lcd.print("wait......");
  Serial.println("AT+CMGF=1");
  delay(3000);
  Serial.println("AT+CMGD=1");
  delay(3000);
  Serial.println("AT+CMGD=2");
  delay(3000);
  Serial.println("AT+CNMI=2,2,0,0,0");
  delay(3000);  
  Serial.println("AT+CNMA");
  delay(3000);
  lcd.clear();
  lcd.print("GPS Initializing");
  lcd.setCursor(0,1);
  lcd.print("  No GPS Range  ");
  get_gps();
  delay(2000);
  lcd.clear();
  lcd.print("GPS Range Found");
  lcd.setCursor(0,1);
  lcd.print("GPS is Ready");
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  temp=0;
  pinMode(buz,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  digitalWrite(led1,0);
  pinMode(sensor,INPUT);
  digitalWrite(sensor,1);
  
}

void loop()
{
s1p=digitalRead(sensor);
x=analogRead(1);
x1=map(x,0,1023,0,1000);

delay(1000);
    
if (s1p==1)
{
lcd.clear();
lcd.setCursor(0,0);  
lcd.print("WOMEN SECURITY  ");   
lcd.setCursor(0,1);  
lcd.print("WAITING FOR SEN.");
digitalWrite(led1,LOW );
digitalWrite(buz,LOW);
delay(1000);
}

if((s1p==0))
{
lcd.clear();
lcd.setCursor(0,0);  
lcd.print("SENSOR ACTIVATED");   
lcd.setCursor(0,1);  
lcd.print("I AM IN DAGER   ");
digitalWrite(led1,HIGH);
digitalWrite(buz,HIGH);
delay(4000);

    for(nt=0;nt<4;nt++)
    {
    get_gps();
    //show_coordinate();
    //coordinate2dec();
    tracking();
    delay(2000);
    tracking1();
     
     delay(20000);
     lcd.clear();
     lcd.setCursor(0,0);  
     lcd.print("Resending.......");   
     lcd.setCursor(0,1);  
     lcd.print(nt);
     delay(5000);
}   
}
 }


//------------

void serialEvent()
{
  while(Serial.available())
  {  buffer = readSIM900A();
    if (buffer.startsWith("\r\n+CMT: "))
    {
        buffer.remove(0, 51);
        int len = buffer.length();
        buffer.remove(len - 2, 2);
      //  Serial.println(buffer);
     
     lcd.clear();
     lcd.print(buffer);
     delay(2000);
     }
  
    temp=0; 
    if(buffer=="VPOS")
    {
     temp=1;
      break;
    }
   if(buffer=="LOCK")
    {
      temp=2;
      break;
    }
      
     
  }
}





//--------------------------------
String readSIM900A()
{
  String buffer;
  while (Serial.available())
    {
        char c = Serial.read();
        buffer.concat(c);
        delay(10);
    }
    return buffer;
}
//-----------------------------------

void gpsEvent()
{
  i=0;
  gpsString="";
  while(1)
  {
   while (gps.available()>0)            //Serial incoming data from GPS
   {
    char inChar = (char)gps.read();
     gpsString+= inChar;                    //store incoming data from GPS to temparary string str[]
     i++;
    // Serial.print(inChar);
     if (i < 7)                      
     {
      if(gpsString[i-1] != test[i-1])         //check for right string
      {
        i=0;
        gpsString="";
      }
     }
    if(inChar=='\r')
    {
     if(i>60)
     {
       gps_status=1;
       break;
     }
     else
     {
       i=0;
     }
    }
  }
   if(gps_status)
    break;
  }
  Serial.println(gpsString);
}
void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    Serial.println("AT");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      at_flag=0;
    }
    
    delay(1000);
  }
  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    Serial.println("ATE0");
    while(Serial.available()>0)
    {
      if(Serial.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0)
    {
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}
void get_gps()
{
  lcd.clear();
  lcd.print("Getting GPS Data");
  lcd.setCursor(0,1);
  lcd.print("Please Wait.....");
   gps_status=0;
   int x=0;
   while(gps_status==0)
   {
    gpsEvent();
    delay(2000);
    int str_lenth=i;
    coordinate2dec();
    delay(2000);
    show_coordinate();
    
    i=0;x=0;
    str_lenth=0;
   }
}
//*************************
void show_coordinate()
{
    lcd.clear();
    lcd.print("Lat:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Log:");
    lcd.print(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
    delay(2000);
}
//*******************************
void coordinate2dec()
{
  String lat_degree="";
    for(i=20;i<=21;i++)         
      lat_degree+=gpsString[i];
      Serial.println(lat_degree);
      
  String lat_minut="";
     for(i=22;i<=28;i++)         
      lat_minut+=gpsString[i];
      Serial.println(lat_minut);
      

  String log_degree="";
    for(i=32;i<=34;i++)
      log_degree+=gpsString[i];

  String log_minut="";
    for(i=35;i<=41;i++)
      log_minut+=gpsString[i];
    
    Speed="";
    for(i=45;i<48;i++)          //extract longitude from string
      Speed+=gpsString[i];
      
     float minut= lat_minut.toFloat();
     minut=minut/60;
     float degree=lat_degree.toFloat();
     latitude=degree+minut;
     
     minut= log_minut.toFloat();
     minut=minut/60;
     degree=log_degree.toFloat();
     logitude=degree+minut;
     Serial.println(latitude,6);
     Serial.println(logitude,6);
}
                                      
//**************************************

void init_sms()
{
  Serial.println("AT+CMGF=1");
  delay(400);
 // Serial.println("AT+CMGS=\"+919493464849\"");   // use 10 digit cell no. here
  Serial.println("AT+CMGS=\"+919182129577\"");
  delay(400);
}
void init_sms1()
{
  Serial.println("AT+CMGF=1");
  delay(400);
 // Serial.println("AT+CMGS=\"+917816018619\"");   // use 10 digit cell no. here
  Serial.println("AT+CMGS=\"+919618513526\"");
  delay(400);
}


void send_data(String message)
{
  Serial.println(message);
  delay(200);
}
void send_sms()
{
  Serial.write(26);
}
void lcd_status()
{
  lcd.clear();
  lcd.print("Message Sending.");
  delay(2000);
  lcd.clear();
  lcd.print("Message Sent");
  delay(2000);
  lcd.clear();
  lcd.print("System Ready");
  return;
}
void tracking()
{
    init_sms();
    //send_data("Child Tracking:");
    send_data("I AM IN DANGER AT  Location is:");
    Serial.print("http://maps.google.com/?q=");
    Serial.print(latitude,6);
    Serial.print(",");
    Serial.print(logitude,6);
    //Serial.println();
    //send_data("Please take some action soon..\nThankyou");
    send_sms();
    delay(2000);
    lcd_status();
}
void tracking1()
{
    init_sms1();
    //send_data("Child Tracking:");
    send_data("I AM IN DANGER AT  Location is:");
    Serial.print("http://maps.google.com/?q=");
    Serial.print(latitude,6);
    Serial.print(",");
    Serial.print(logitude,6);
    //Serial.println();
    //send_data("Please take some action soon..\nThankyou");
    send_sms();
    delay(2000);
    lcd_status();
}
//***********************************************************************