#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define LCD_SDA  20
#define LCD_SCL  21
#define btn 2

char empty[12] = "..........."; // Base background

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);
void setup() {
  Wire.begin(LCD_SDA, LCD_SCL);
  lcd.begin (16,2);
  Serial.begin(115200);
  pinMode(btn, INPUT_PULLUP);
  // base lcd setup
  lcd.setCursor(0,0);
  lcd.print(empty);
  lcd.setCursor(0,1);
  lcd.print(empty);
  lcd.setCursor(11,0);
  lcd.print("<X>");
}

int bulletY(){
  int randomY = random(0,2);
  Serial.println(randomY);
  return randomY;
}

char bullet1[12] = "*.........."; // Initial bullet position
char fill[12] = "***********";

// Bullet tracking variables
int bullet1Pos = 1;
int bullet1Y = 0;
bool bullet1Alive = false;

int moveBullet1(int yPos1){ // Function that actually moves the bullet
  lcd.setCursor(0,yPos1);
  strncpy(bullet1,fill,bullet1Pos); //draws the bullet to the current position
  strncpy(bullet1,empty,bullet1Pos-1); //clears its trace
  bullet1Pos += 1; // adds one to the position
  Serial.println(bullet1);
  lcd.print(bullet1);
  // delay(500);
  return bullet1Pos;
}

int timer = 0;
int airshipPos = 0;
int score = 0;

void loop() {
  if(timer < 1000){
    Serial.println(timer);
    timer += 1;
  }
  else if(timer >= 1000){
    timer = 0;
  }

  if(digitalRead(btn) == LOW){ //moves the airship
    if(airshipPos == 0){
      lcd.setCursor(11,0);
      lcd.print("   ");
      lcd.setCursor(11,1);
      lcd.print("<X>");
      airshipPos = 1;
    }
    else if(airshipPos == 1){ //moves the airship
      lcd.setCursor(11,1);
      lcd.print("   ");
      lcd.setCursor(11,0);
      lcd.print("<X>");
      airshipPos = 0;
    }
  }

  while(bullet1Pos == 12 && bullet1Y == airshipPos){ //checks if there's a collision
    lcd.setCursor(0,0);
    delay(500);
    lcd.print("GAME OVER      ");
    lcd.setCursor(0,1);
    delay(500);
    lcd.print("Final score:" );
    lcd.setCursor(12,1);
    delay(500);
    lcd.print("   ");
    lcd.setCursor(13,1);
    delay(500);
    lcd.print(score);
  }

  lcd.setBacklight(HIGH);

  if(bullet1Pos >= 12){ //checks if the bullet has reached its limit
    bullet1Alive = false;
    bullet1Pos = 1;
    strncpy(bullet1,empty,12); //cleans the canvas
    lcd.setCursor(0,0);
    lcd.print(empty);
    lcd.setCursor(0,1);
    lcd.print(empty);
    score += 1;
  }

  if(bullet1Alive == true && timer == 999){
    moveBullet1(bullet1Y);
  }
  else if(bullet1Alive == false && timer == 999){
    bullet1Y = bulletY();
    bullet1Alive = true;
    
  }
}