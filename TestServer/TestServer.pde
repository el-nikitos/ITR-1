/**
 * Chat Server 
 * by Tom Igoe. 
 * 
 * Press the mouse to stop the server.
 */
 

import processing.net.*;

int port = 80;
boolean myServerRunning = true;
int bgColor = 0;
int direction = 1;
int textLine = 20;

Server myServer;

void setup()
{
  size(400, 800);
  //textFont(createFont("SanSerif", 16));
  myServer = new Server(this, port); // Starts a myServer on port 10002
  background(0);
}

/*
void mousePressed()
{
  // If the mouse clicked the myServer stops
  myServer.stop();
  myServerRunning = false;
}
*/

void draw()
{
  if (myServerRunning == true)
  {
    //text("server", 15, 45);
    //println("server started");
    
    Client thisClient = myServer.available();
    if (thisClient != null) {
      if (thisClient.available() > 0) {
        text("mesage from: " + thisClient.ip() + " : " + thisClient.readString(), 15, textLine);
        textLine = textLine + 15;
      }
      if (textLine > height)  {
       background(0);
       textLine = 20;
       text("mesage from: " + thisClient.ip() + " : " + thisClient.readString(), 15, textLine);
       textLine = textLine + 15;
      }
      
    }
  } 
  else 
  {
    background(0);
    text("server", 15, 45);
    text("stopped", 15, 65);
  }
}
