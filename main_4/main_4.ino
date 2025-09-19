#include <LiquidCrystal_I2C.h>
#include <math.h>
#include <Wire.h>
// lcd setup
  //the first parameter is  the I2C address
  //the second parameter is how many rows are on your screen
  //the  third parameter is how many columns are on your screen
  LiquidCrystal_I2C lcd(0x27, 16, 2);
  int cursor;
  int cursor_limit_left;
  int cursor_limit_right;
// math setup
  // azimuth value
    int     Az_100;
    int     Az_10;
    int     Az_1;
    int     Az_01;
    int     Az_001;
    float   azimuth;
  // lintang Value
    String  lin_100;
    int     lin_10;
    int     lin_1;
    int     lin_01;
    int     lin_001;
    float   lintang;
  // altitude value
    int     lat_100;
    int     lat_10;
    int     lat_1;
    int     lat_01;
    int     lat_001;
    float   altitude;

  float deklanasi;
  float True_deklanasi;
  // date
    float   X_days;
    float   Y_days;
    char    X_monthName[10];
    int     X_dayNum;
    char    Y_monthName[10];
    int     Y_dayNum;
// stuff to kale a good flow
  int adder;
  int tiemr;
// special caharacter
  byte lock_close[] = { // lock close
    B00000,
    B01110,
    B01010,
    B01010,
    B11111,
    B11011,
    B11111,
    B01110,
  };
  
  byte lock_open[] = { // lock open
    B00000,
    B01110,
    B00010,
    B00010,
    B11111,
    B11011,
    B11111,
    B01110,
  };
  
  byte right_arrow[] = { // right arrow
    B00000,
    B01100,
    B00110,
    B11111,
    B11111,
    B00110,
    B01100,
    B00000,
  };
  byte left_arrow[] = { // left arrow
    B00000,
    B00110,
    B01100,
    B11111,
    B11111,
    B01100,
    B00110,
    B00000,
  };
  byte up_arrow[] = { // up arrow
    B00000,
    B00100,
    B01110,
    B10101,
    B10101,
    B00100,
    B00100,
    B00100,
  };
  byte down_arrow[] = { // down arrow
    B00000,
    B00100,
    B01110,
    B10101,
    B10101,
    B00100,
    B00100,
    B00100,
  };
  byte clear[] = { // blank
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
  };
// event setup
  String event;
// joystick setup
  int       joy_x;
  int       joy_y;
  int       joy_button;
  int       location_x;
  // button condition
    float pressed = false;
    bool bug = false;

// void setup
  void setup() {
    Serial.begin(19200);
    // initiate lcd setting
      lcd.init();          //initialize lcd screen
      lcd.backlight();     // turn on the backlight
    // set costom character
      lcd.createChar(0, lock_close);
      lcd.createChar(1, lock_open);
      lcd.createChar(2, right_arrow);
      lcd.createChar(3, left_arrow);
      lcd.createChar(4, up_arrow);
      lcd.createChar(5, down_arrow);
      lcd.createChar(6, clear);
    // joystick setup
      pinMode(A0, INPUT);  //joystick y
      pinMode(A1, INPUT);  //joystick x
      pinMode(3, INPUT_PULLUP);
    // set azimuth value
    // set lintang value
      lin_100 = "+";
    // set altitude value
    event = "lockscreen";
  }
// costom void
  void lockscreen(){
    lcd.setCursor(10,0);
    lcd.write(byte(0)); //byte lock
    lcd.setCursor(10,1);
    lcd.write(byte(0)); //byte lock
    lcd.setCursor(5,0);
    lcd.write(byte(0)); //byte lock
    lcd.setCursor(5,1);
    lcd.write(byte(0)); //byte lock
  }
  void azimuth_screen(){
    lcd.setCursor(0,0);
    lcd.print("Azimuth: ");
    lcd.print(Az_100);
    lcd.print(Az_10);
    lcd.print(Az_1);
    lcd.print(",");
    lcd.print(Az_01);
    lcd.print(Az_001);
    lcd.write(byte(1)); //byte lock
  }
  void lintang_screen(){
    lcd.setCursor(0,0);
    lcd.print("Lintang: ");
    lcd.print(lin_100);
    lcd.print(lin_10);
    lcd.print(lin_1);
    lcd.print(",");
    lcd.print(lin_01);
    lcd.print(lin_001);
    lcd.write(byte(1)); //byte lock
  }
  void altitude_screen(){
    lcd.setCursor(0,0);
    lcd.print("Altitude:");
    lcd.print(lat_100);
    lcd.print(lat_10);
    lcd.print(lat_1);
    lcd.print(",");
    lcd.print(lat_01);
    lcd.print(lat_001);
    lcd.write(byte(1)); //byte lock
  }
  void Calculating_screen(){
    lcd.setCursor(0,0);
    lcd.print("loading");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
    delay(200);
    lcd.print(".");
  }
  int limiter(int Upper_limit, int lower_limit, int current) {
    if (current > Upper_limit){
      current -= 1;
      return current;
    }
    else if (current < lower_limit){
      current += 1;
      return current;
    }
    else {
      return current;
    }
  }
  void joystick_movement(){
    Serial.println(joy_x);
    if ((joy_x < 300) && (cursor > cursor_limit_left)){
        cursor -= 1;
        Serial.print(cursor);
        delay(200);
      }
    if ((joy_x > 900) && (cursor < cursor_limit_right)){
        cursor += 1;
        Serial.print(cursor);
        delay(200);
      }
    if ((joy_y < 300)){
        adder += 1;
        Serial.print(cursor);
        delay(200);
      }
    if ((joy_y > 900)){
        adder -= 1;
        Serial.print(cursor);
        delay(200);
      }
  }
  void botom_arrow(){
    lcd.setCursor((cursor + 7),1);
    lcd.write(byte(6)); //byte clear
    lcd.write(byte(4)); //byte up arrow
    lcd.write(byte(6)); //byte clear
  }
  void combine(){
    azimuth  =  ((Az_100 * 100) + (Az_10 * 10) + (Az_1 * 1) + (Az_01 * 0.1) + (Az_001 * 0.01));
    Serial.print("azimuth = ");
    Serial.println(azimuth);
    lintang  =  ((lin_10 * 10) + (lin_1 * 1) + (lin_01 * 0.1) + (lin_001 * 0.01));
    if (lin_100 == "+"){
      lintang = lintang * 1; 
    }
    if (lin_100 == "-"){
      lintang = lintang * -1; 
    }
    Serial.print("lintang = ");
    Serial.println(lintang);
    altitude =  ((lat_100 * 100) + (lat_10 * 10) + (lat_1 * 1) + (lat_01 * 0.1) + (lat_001 * 0.01));
    Serial.print("altitude = ");
    Serial.println(altitude);
  }
  void find_deklinasi(){
    azimuth  = radians(azimuth);
    lintang  = radians(lintang);
    altitude = radians(altitude);
    deklanasi = ( 
      ( cos(azimuth) * cos(altitude) * cos(lintang) ) 
      + 
      ( sin(altitude) * sin(lintang) ) 
      );
    deklanasi = asin(deklanasi);
    True_deklanasi = degrees(deklanasi);
    deklanasi = degrees(deklanasi);
    Serial.print("deklanasi = ");
    Serial.println(deklanasi);
    Serial.print("True_deklanasi = ");
    Serial.println(True_deklanasi);
  }
  void find_X(){
    X_days = deklanasi / 23.5;
    Serial.print("dek / 23.5 = ");
    Serial.println(X_days, 10); 
    X_days = asin(X_days);
    Serial.print("asin(X_days) [in radians] = ");
    Serial.println(X_days, 10);
    X_days = degrees(X_days);
    Serial.print("degrees(X_days) = ");
    Serial.println(X_days, 10);
    X_days = X_days * 365;
    Serial.print("X_days * 365 = ");
    Serial.println(X_days, 10);
    X_days = X_days / 360;
    Serial.print("X_days / 360 = ");
    Serial.println(X_days, 10);
    X_days = X_days - 284;
    Serial.print("X_days - 284 = ");
    Serial.println(X_days, 10);
    X_days = X_days + 365;
    Serial.print("X_days + 365 = ");
    Serial.println(X_days, 10);
    Serial.print("Final X_days = ");
    Serial.println(X_days, 10);
  }
  void find_Y(){
    Y_days = deklanasi / 23.5;
    Serial.print("dek / 23.5 = ");
    Serial.println(Y_days);
    Y_days = asin(Y_days);
    Y_days = degrees(Y_days);
    Serial.print("asin(dek/23.5) in degrees = ");
    Serial.println(Y_days);
    Y_days = 540 - Y_days;
    Serial.print("540 - asin result = ");
    Serial.println(Y_days);
    Y_days = Y_days * 365;
    Serial.print("Y_days * 365 = ");
    Serial.println(Y_days);
    Y_days = Y_days / 360;
    Serial.print("Y_days / 360 = ");
    Serial.println(Y_days);
    Y_days = Y_days - 284;
    Serial.print("Y_days - 284 = ");
    Serial.println(Y_days);
    Serial.print("Final Y_days = ");
    Serial.println(Y_days);
  }
  void input_data(){
    if (event == "azimuth screen"){
      switch (cursor){
        case 1 :
          Az_100 += adder;
          adder = 0;
          Az_100 = limiter(9,0,Az_100);
          break;
        case 2 :
          Az_10 += adder;
          adder = 0;
          Az_10 = limiter(9,0,Az_10);
          break;
        case 3 :
          Az_1 += adder;
          adder = 0;
          Az_1 = limiter(9,0,Az_1);
          break;
        case 4 :
          Serial.print("nothing");
          break;
        case 5 :
          Az_01 += adder;
          adder = 0;
          Az_01 = limiter(9,0,Az_01);
          break;
        case 6 :
          Az_001 += adder;
          adder = 0;
          Az_001 = limiter(9,0,Az_001);
          break;
        case 7:
          if (joy_button == 0){
            event = "lintang screen";
            lcd.clear();
            cursor = 1;
            cursor_limit_right = 7;
            cursor_limit_left = 1;
          }
      }
    }
    if (event == "lintang screen"){
      switch (cursor){
        case 1:
          if ((joy_y < 300)){
            lin_100 = "+";
          }
          if ((joy_y > 900)){
            lin_100 = "-";
          }
          adder = 0;
          
          break;
        case 2:
          lin_10 += adder;
          adder = 0;
          lin_10 = limiter(9,0,lin_10);
          break;
        case 3:
          lin_1 += adder;
          adder = 0;
          lin_1 = limiter(9,0,lin_1);
          break;
        case 4:
          Serial.println("none");
          break;
        case 5:
          lin_01 += adder;
          adder = 0;
          lin_01 = limiter(9,0,lin_01);
          break;
        case 6:
          lin_001 += adder;
          adder = 0;
          lin_001 = limiter(9,0,lin_001);
          break;
        case 7:
          if (joy_button == 0){
            event = "altitude screen";
            lcd.clear();
            cursor = 1;
            cursor_limit_right = 7;
            cursor_limit_left = 1;
          }
        
      }
    }
    if (event == "altitude screen"){
      switch (cursor){
        case 1:
          lat_100 += adder;
          adder = 0;
          lat_100 = limiter(9,0,lat_100);
          break;
        case 2:
          lat_10 += adder;
          adder = 0;
          lat_10 = limiter(9,0,lat_10);
          break;
        case 3:
          lat_1 += adder;
          adder = 0;
          lat_1 = limiter(9,0,lat_1);
          break;
        case 4:
          Serial.println("none");
          break;
        case 5:
          lat_01 += adder;
          adder = 0;
          lat_01 = limiter(9,0,lat_01);
          break;
        case 6:
          lat_001 += adder;
          adder = 0;
          lat_001 = limiter(9,0,lat_001);
          break;
        case 7:
          if (joy_button == 0){
            lcd.clear();
            Calculating_screen();
            lcd.clear();
            delay(200);
            Calculating_screen();
            lcd.clear();
            event = "calculating";
          }
        
      }
    }
  }
  void getMonthAndDay(int dayOfYear, char* monthName, int &day) {
    const int daysInMonths[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    const char* monthNames[] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
      };
    int accumulatedDays = 0;
    for (int i = 0; i < 12; i++) {
      if (dayOfYear <= accumulatedDays + daysInMonths[i]) {
        day = dayOfYear - accumulatedDays;
        strncpy(monthName, monthNames[i], 10); // copy month name
        return;
      }
      accumulatedDays += daysInMonths[i];
    }
    strncpy(monthName, "Invalid", 10);
    day = -1;
  }
void loop() {
  // setup
    joy_x = analogRead(A1);
    joy_y = analogRead(A0);
    joy_button = digitalRead(3);
  
  //
  if (event == "lockscreen"){
    lockscreen();
    if (joy_button == 0){
      event = "azimuth screen";
      lcd.clear();
      lcd.backlight();
      cursor = 1;
      cursor_limit_right = 7;
      cursor_limit_left = 1;
    }
    // cleanup
    altitude = 0;
    lintang  = 0;
    azimuth  = 0;
    // serial prints
      Serial.println(joy_button); 
  }
  if (event == "azimuth screen"){
    azimuth_screen();
    input_data();
    joystick_movement();
    botom_arrow();
  }
  if (event == "lintang screen"){
    lintang_screen();
    input_data();
    joystick_movement();
    botom_arrow();
  }
  if (event == "altitude screen"){
    altitude_screen();
    input_data();
    joystick_movement();
    botom_arrow();
  }
  if (event == "calculating"){
    lcd.clear();
    Calculating_screen();
    lcd.clear();
    delay(200);
    Calculating_screen();
    lcd.clear();
    combine();
    find_deklinasi();
    find_X();
    find_Y();
    event = "result";

  }
  if (event == "result"){
    lcd.setCursor(0,0);
    lcd.print("Deklanasi = ");
    lcd.print(True_deklanasi);
    lcd.setCursor(0,1);
    getMonthAndDay(X_days, X_monthName, X_dayNum);
    getMonthAndDay(Y_days, Y_monthName, Y_dayNum);
    lcd.print(X_monthName);
    lcd.print(" ");
    lcd.print(X_dayNum);
    lcd.print(" / ");
    lcd.print(Y_monthName);
    lcd.print(" ");
    lcd.print(Y_dayNum);

    event = "done";
  }
  if (event == "done"){
    while(!bug){
      joy_button = 1;
      bug = (joy_button == 0 ? false : true);
    }
    for (int i = 0; i <= 300000; i++){
      delay(1);
      if (joy_button == 0){
        lcd.clear();
        event = "lockscreen";
      }
      else {
        i += 1;
      
      }
    }
    lcd.noBacklight();
    event = "lockscreen";
  }
  
  else {
    Serial.print("");
  }
}