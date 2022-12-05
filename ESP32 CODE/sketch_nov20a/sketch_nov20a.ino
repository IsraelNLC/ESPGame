#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define LCD_SDA  20
#define LCD_SCL  21
#define btn 2

const char* ssid = "SHARE-RESIDENTE";
const char* password = "Share@residente";
char empty[12] = "..........."; // Base background

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

WiFiServer server(80);

void WiFiConnect(){
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {  //Check for the connection
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
}

int postDataToServer(int gstatus){
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Iniciando POST...");
    // Block until we are able to connect to the WiFi access point
    HTTPClient http;   
      
      // Especifica a URL e o tipo de arquivo da requisição
      http.begin("https://nyld21-3061.preview.csb.app/gamestatus");  
      http.addHeader("Content-Type", "application/json");
      
      StaticJsonDocument<200> doc;
      // Add values in the document
      //
      doc["status"] = gstatus;
      // Add an array.
      //
      // JsonArray data = doc.createNestedArray("data");
      // data.add(2);
      
      String requestBody;
      serializeJson(doc, requestBody);
      
      int httpResponseCode = http.POST(requestBody);
  
      if(httpResponseCode>0){
        
        String response = http.getString();                       
        
        Serial.println(httpResponseCode);   
        Serial.println(response);
        return 0;
      }
    http.end();
  }
  else{
     Serial.println("Error in WiFi connection");
  }
}

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

  Serial.begin(115200);
  delay(4000);  //Delay needed before calling the WiFi.begin

  WiFiConnect();
  postDataToServer(1);
}

int bulletY(){
  // int randomY = random(0,2);
  int randomY = getDataFromServer();
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
  delay(500);
  return bullet1Pos;
}

int timer = 0;
int airshipPos = 0;
int score = 0;

int getDataFromServer(){
  if (WiFi.status() == WL_CONNECTED) {  //Check WiFi connection status

    Serial.println("Iniciando o GET request...");
    HTTPClient http;

    http.begin("https://nyld21-3061.preview.csb.app/sendposition");  //Specify destination for HTTP request
    http.addHeader("Content-Type", "application/json");       //Specify content-type header

    StaticJsonDocument<200> doc;

    int httpResponseCode = http.GET();  //Send the actual GET request

    if (httpResponseCode > 0) {


      String response = http.getString();  //Get the response to the request

      Serial.println(httpResponseCode);  //Print return code
      Serial.println(response);          //Print request answer

      deserializeJson(doc, response);
      int direction = doc["position"];

      // Serial.println(direction);
      return direction;
      
    }
    else{

      Serial.print("Error on GET request: ");
      Serial.println(httpResponseCode);
    }

    http.end();  //Free resources

  }
  else{
    Serial.println("Error in WiFi connection");
  }
}

void loop() {

  if(timer < 10){
    Serial.println(timer);
    timer += 1;
  }
  else if(timer >= 10){
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
    postDataToServer(0);
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

  if(bullet1Alive == true && timer == 9){
    delay(300);
    moveBullet1(bullet1Y);
  }
  else if(bullet1Alive == false && timer == 9){
    bullet1Y = bulletY();
    bullet1Alive = true;
    
  }
  
}