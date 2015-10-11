import processing.serial.*;

Serial myPort;  // Create object from Serial class
String val;     // Data received from the serial port

void setup()
{
  size(400, 400);
  String portName = Serial.list()[0]; //change the 0 to a 1 or 2 etc. to match your port
  myPort = new Serial(this, portName, 9600);  
  
}

void draw()
{
   float distances[4];
   if (myPort.available() > 0)
   {
    val = myPort.readStringUntil('\n');
    if (val == "distances")
    {
      float Xaverage;
      float Yaverage;
      for(int i =0; i<4; i++)
      {
        while(myPort.available() == 0);
        if(myPort.available()>0)
          distances[i] = float(myPort.readStringUntil('/n'));
      } 
      for(int i =0;i<4;i++)
      {
        int nextInt = i==3?0:i+1;
        float DistOne = distacnes[i];
        float DistTwo = distances[nextInt];
        float DistThree = 190;
        float S = (DistThree + DistOne + DistTwo)/2.0;
        float Y = (sqrt(S*(S-DistThree)*(S-DistOne)*(S-DistTwo))/DistThree);
        float X = sqrt(sq(DistOne)-sq(Y));
        Xaverage += X;
        Yaverage += Y;
      }
      Xaverage/=4;
      Yaverage/=4;
      println(Xaverage);
      println(Yaverage);
    }
   }
}