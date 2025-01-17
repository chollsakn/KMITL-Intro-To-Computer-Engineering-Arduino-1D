#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define UP_BUTTON 3
#define DOWN_BUTTON 2


int buzzer = 11, score = 0;
unsigned long tim;

int16_t x1=0,y1=0,x2=0,y2=12;

//ball position//
uint8_t ball_dir_x = 1, ball_dir_y = 1;
uint8_t ball_x = 64, ball_y = 16;



void setup() {
    Serial.begin(9600);
    pinMode(UP_BUTTON, INPUT);
    pinMode(DOWN_BUTTON, INPUT);
    digitalWrite(UP_BUTTON,1);
    digitalWrite(DOWN_BUTTON,1);
    tim = millis();
    pinMode(buzzer, OUTPUT);
    
    
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      display.drawPixel(10, 10, SSD1306_WHITE);
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(20); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();


  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  delay(20);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...
  testdrawline();
  testdrawcircle();
  // Invert and restore display, pausing in-between
}

//paddle section//
void testdrawline(void) {
  

  display.clearDisplay(); // Clear display buffer
  // ระบุมจุดเริ่มต้นจุดสิ้นสุด + บอกสี

  if(y2 < display.height()) {
    y1 += 1;
    y2 += 1;
    
//    display.display(); // Update screen with each newly-drawn line
//    display.clearDisplay();
    
  }
}

void testdrawlineup(void) {
  display.clearDisplay(); // Clear display buffer
  // ระบุมจุดเริ่มต้นจุดสิ้นสุด + บอกสี

  if(y1 > 0) {
    y1 -= 1;
    y2 -= 1;
    
//    display.display(); // Update screen with each newly-drawn line
//    display.clearDisplay();
    
  }
}

// e-paddle section//

void loop() {
     
     bool updat = false;
     bool up_state ;
     bool down_state ;
     
     up_state = digitalRead(UP_BUTTON);
     down_state = digitalRead(DOWN_BUTTON);

    if(down_state == false){
      testdrawline();
      down_state = false;
      
      }
    if(up_state == false){
        testdrawlineup();
        up_state = false;

        
        
      }
    
    uint8_t new_x = ball_x + ball_dir_x;
    uint8_t new_y = ball_y + ball_dir_y;
    
    if(new_x == 127) {
            
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
            tone(buzzer,100,10);
            
            
        }
    if(new_y == 0 || new_y == 32) {
            
            ball_dir_y = -ball_dir_y;
            new_y += ball_dir_y + ball_dir_y;
            tone(buzzer,100,10);
            

        }
    if(new_x == 0
           && new_y >= y1-1
           && new_y <= y2+1)
        {
            
            ball_dir_x = -ball_dir_x;
            new_x += ball_dir_x + ball_dir_x;
            tone(buzzer,100,10);
            
            
        }
    if(new_x == 0 && !(new_y >= y1 && new_y <= y2)){
        
        display.clearDisplay();
        ball_x = 64;
        ball_y = 16;
        new_x = ball_x;
        new_y = ball_y;
        ball_dir_x *= -1;
        tone(buzzer,40,100);
        delay(500);
        tone(buzzer,40,100);
        delay(500);
        tone(buzzer,40,500);
        delay(500);
        
        
        
        
      }


    display.drawCircle(ball_x, ball_y, 2, SSD1306_BLACK);
    display.drawCircle(new_x, new_y, 2, SSD1306_WHITE);
    display.drawLine(0,y1,0,y2, SSD1306_WHITE);
    ball_x = new_x;
    ball_y = new_y;
    display.display();
    
    
    
}

void testdrawcircle(void) {
    display.drawCircle(ball_x, ball_y, 2, SSD1306_WHITE);
}


void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=26; i<52; i++) {
//    if(i == '\n') display.write(' ');
              display.write(i);
  }

  display.display();
  delay(2000);
}
