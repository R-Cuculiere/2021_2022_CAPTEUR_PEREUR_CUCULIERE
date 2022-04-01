/** Collision (Pong). 
 * Move the mouse up and down to move the paddle.  
 * Modified to use Serial port by Tod E. Kurt, 2007
 * Updated 13 January 2003 by K Pfeiffer
 */

import processing.serial.*;

//String portname = "/dev/tty.usbserial-A4001qa8";  // or "COM8"
String portname = "COM4";  // or "COM8"
Serial port;  // Create object from Serial class

// Global variables for the ball
float ball_x;
float ball_y;
float ball_dir = 1;
float ball_size = 10;  // Radius
float dy = 0;  // Direction

// Global variables for the paddle
int paddle_width = 5;
int paddle_height = 20;
int paddle_pos;   // new position
int paddle_ppos;  // last position
int dist_wall = 15;

void setup()
{
  size(255, 255);
/*
  rectMode(CENTER_RADIUS);
  ellipseMode(CENTER_RADIUS);
*/
  rectMode(CENTER);
  ellipseMode(CENTER);

  noStroke();
  smooth();
  ball_y = height/2;
  ball_x = 1;
  
  // Open the port that the board is connected to and use the same speed (19200 bps)
  port = new Serial(this, portname, 19200);

}

void draw() 
{
  background(51);
    ball_x += ball_dir * 1.0;
  ball_y += dy;
  if(ball_x > width+ball_size) {
    ball_x = -width/2 - ball_size;
    ball_y = random(0, height);
    dy = 0;
  }
  
 if (port.available() > 0) {  // If data is available,
    paddle_ppos = paddle_pos;      // save old position
    paddle_pos = port.read();      // read it and store it as the new pos
    paddle_pos = 255-paddle_pos; //pour avoir les valeurs de 0 à 255
    println(paddle_pos);  
}
  // Constrain paddle to screen
  float paddle_y = constrain(paddle_pos, paddle_height, height-paddle_height);

  // Test to see if the ball is touching the paddle
  float py = width-dist_wall-paddle_width-ball_size;
  if(ball_x == py 
     && ball_y > paddle_y - paddle_height - ball_size 
     && ball_y < paddle_y + paddle_height + ball_size) {
    ball_dir *= -1;
    if(paddle_pos != paddle_ppos) {
      dy = (paddle_pos - paddle_ppos)/2.0;
      if(dy >  5) { dy =  5; }
      if(dy < -5) { dy = -5; }
    }
  } 
  
  // If ball hits paddle or back wall, reverse direction
  if(ball_x < ball_size && ball_dir == -1) {
    ball_dir *= -1;
  }
  
  // If the ball is touching top or bottom edge, reverse direction
  if(ball_y > height-ball_size) {
    dy = dy * -1;
  }
  if(ball_y < ball_size) {
    dy = dy * -1;
  }
  // Draw ball
  fill(255);
  ellipse(ball_x, ball_y, ball_size, ball_size);
  
  // Draw the paddle
  fill(153);
  rect(width-dist_wall, paddle_y, paddle_width, paddle_height);  
}