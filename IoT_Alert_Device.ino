/*

When the button is pressed or a fall is detected from the accelerometer, a message will be sent to the phone number with 
the alert message and the GPS lat and lon.

When the secound button is pressed it will datalog the accelerometer x, y, and z for 24 logs to a CSV file and then creates a new one.

*/

#define CUSTOM_SETTINGS
#define INCLUDE_SMS_SHIELD
#define INCLUDE_GPS_SHIELD
#define INCLUDE_EMAIL_SHIELD
#define INCLUDE_ACCELEROMETER_SENSOR_SHIELD
#define INCLUDE_DATA_LOGGER_SHIELD

/* Include 1Sheeld library. */
#include <OneSheeld.h>

/* Button */
int buttonSend = 4;
int buttonDataLog = 2;

/* LED */
int ledDatalog = 13;

/* Reserve a counter for datalogging */
int counter = 0;

/* Boolean to start logging. */ 
bool startFlag = false;

/* GPS Data */
/* lat */
float lat;
char charlat [10];
/* lon */
float lon;
char charlon [10];
/*lat and lon reading */
char readings [80];

/* Text message not sent/ Define a boolean flag. */
boolean isMessageSent = false;
boolean accelMessageSent = false;

/*Accelerometer Data */
float xValue = AccelerometerSensor.getX();
char charX [10];
float yValue = AccelerometerSensor.getY();
char charY [10];
float zValue = AccelerometerSensor.getZ();
char charZ [10];
char readings2 [80];

void setup() 
{
 /* Start communication. */
 OneSheeld.begin(); 
 pinMode(buttonSend,INPUT);
 pinMode(buttonDataLog,INPUT);
 pinMode(ledDatalog,OUTPUT);
 /* Save previous log data */
 Logger.stop();
}

void accelSMS(){
{if (AccelerometerSensor.getZ() <= 1)
  {
    if(!accelMessageSent)
   {
      lat = GPS.getLatitude(); lon = GPS.getLongitude(); 
      dtostrf(lat, 7, 3, charlat); 
      dtostrf(lon, 7, 3, charlon);
      strcat(readings,"ALERT Accelerometer has detected a fall - GPS : \n");               
      strcat(readings,"Latitude is : ");   
      strcat (readings,charlat);  
      strcat(readings,"\nLongitude is : "); 
      strcat (readings,charlon);
      SMS.send("07934093813",readings);                             
      accelMessageSent = true;
   }
      else
    {
      /* Reset the flag. */
     isMessageSent = false;
    }
  }
}
}

void loop() 
{
  /* When the button is pressed, it sends an alert. */
  if (digitalRead(buttonSend) == HIGH)
  {
    if(!isMessageSent)
    {
         lat = GPS.getLatitude(); lon = GPS.getLongitude(); 
         dtostrf(lat, 7, 3, charlat); 
         dtostrf(lon, 7, 3, charlon);
         strcat(readings,"ALERT Button has been pressed - GPS : \n");               
         strcat(readings,"Latitude is : ");   
         strcat (readings,charlat);  
         strcat(readings,"\nLongitude is : "); 
         strcat (readings,charlon);
         SMS.send("07934093813",readings);                             
         isMessageSent = true;
      } 
    else
    {
      /* Reset the flag. */
     isMessageSent = false;
    }
  }

  if (AccelerometerSensor.getZ() <= 1)
  {
    accelSMS;
  }

   if (digitalRead(buttonDataLog) == HIGH)
   {
    /* LED ON */
    digitalWrite(ledDatalog,HIGH);
    /* First insure to save previous logged values. */
    Logger.stop();
    /* Set a delay. */
    OneSheeld.delay(500);
    /* Start logging in a new CSV file. */
    Logger.start("Accelerometer Data X, Y, Z");
    /* Set startFlag. */
    startFlag = true;
   }

   /* Check Datalog start */
   if(startFlag)
  {
    /* Add Z axis dat to the CSV file. */
    Logger.add("X",AccelerometerSensor.getX());
    /* Add Z axis dat to the CSV file. */
    Logger.add("Y",AccelerometerSensor.getY());
    /* Add Z axis dat to the CSV file. */
    Logger.add("Z",AccelerometerSensor.getZ());
    /* Log the row in the file. */
    Logger.log();  
    /* Delay for 1 second. */
    OneSheeld.delay(1000);
    /* Increment counter. */
    counter++;
    /* Stop logging after 24 readings and save the CSV file. */
    if(counter==24)
    {
      /* LED OFF */
      digitalWrite(ledDatalog,LOW);
      /* Save the logging CSV file. */
      Logger.stop();
      /* Reset counter. */
      counter=0;
    }
  }
}
