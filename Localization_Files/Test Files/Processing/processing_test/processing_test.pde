import processing.serial.*;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port
float[] coordinate;
boolean readyToRead = false;
int num = 0;
void setup()
{
  size(400, 400);
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);  
  coordinate= new float[2];

}

void draw()
{
   if (myPort.available() > 0 && num < 2)
   {
     val = myPort.readStringUntil('\n');
     if(val!= null)
     {
     //println(val);
     //println(num);
     char c1 = val.charAt(0);
     //print(val.length());
     if (c1 == 'q')
        readyToRead= true;
     else if(readyToRead){
          println("READ");
          val = val.substring(0,5);
          coordinate[num] = float(val);
          num++;
   }
     }
   if (num >= 2)
   {
     println(coordinate[0]);
     println(coordinate[1]);
   }
   }
}