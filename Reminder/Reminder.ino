#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <iarduino_RTC.h>
#include <EEPROM.h>

iarduino_RTC time(RTC_DS3231);

Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);

const byte START_MENU_LENGTH = 3;
const byte ADD_MENU_LENGTH = 4;
const byte MENU_ROW_HEIGHT = 11;
const byte LCD_ROWS = 4;

const byte BTN_A = 2;
const byte BTN_B = 3;
const byte BTN_C = 4;
const byte BTN_D = 5;
const byte BTN_E = 6;
const byte BTN_F = 7;

const byte PIN_ANALOG_X = 0;
const byte PIN_ANALOG_Y = 1;

const int X_THRESHOLD_LOW = 480;
const int X_THRESHOLD_HIGH = 540;
const int Y_THRESHOLD_LOW = 480;
const int Y_THRESHOLD_HIGH = 540;

int levelMenu = 0;

int startMenuPos = 0;
int menuScreen = 0;
int markerPos = 0;
int startMenuStartAt = 0;

int addMenuStartAt = 0;
int addMenuPos = 0;

int eventsMenuStartAt = 0;
int eventsMenuPos = 0;
int eventsMenuLength = 2;

const byte MENU_ROW_WIGHT = 13;
int posit = 0;
int seconddot = 0;
int line = 0;
int data[6] = {0, 0, 0, 1, 1, 0};
int highLim[6] = {23, 59, 59, 31, 12, 99};
int lowLim[6] = {0, 0, 0, 1, 1, 0};

int setTime[6] = {0, 0, 0, 1, 1, 0};

int positName = 0;
int codeOfSymbol = 32;
char Symbol = ' ';
String nameOfEvent = " ";

int positOfDes = 0;
int codeOfSymbolOfDes = 32;
char SymbolOfDes = ' ';
String desOfEvent = " ";

const byte SIZE_OF_EVENT = 80;
const int NUMBER_OF_EVENTS = 1023;

struct Event{
  byte year;
  byte month;
  byte day;
  byte hour;
  byte minute;
  byte second;
  byte lengthOfName;
  byte lengthOfDes;
  String nameOfEvent;
  String desOfEvent;  
};

const int LENGTH_EVENTS = 6;
byte numberOfEvents = 0;

Event arrayOfEvents[LENGTH_EVENTS];

String startMenu[3] = {
  "Add event",
  "Events",
  "Set time"
};

String addMenu[4] = {
  "Time",
  "Name",
  "Description",
  "Save"
};

String eventsMenu[LENGTH_EVENTS];

byte numberOfPage = 0;

byte yesOrNo = 0;

byte seconds;
byte minutes;
byte hours;
byte day;
byte month;
byte year;

byte nowEvent;

void showError(String str1, String str2 = "") {
   display.clearDisplay();
   display.setTextColor(BLACK, WHITE);
   display.setCursor(1, 0); 
   display.print(str1);
   display.setCursor(1, 9);
   display.print(str2);
   display.display();
}

void showSetTime(){
   display.clearDisplay();
   display.setTextColor(BLACK, WHITE);
   display.setCursor(28, 10); 
   display.print("Time:");
   int markerX = 0;
   int markerY = 0;
   int seconddot = 0;
   int line = 0;

   if (setTime[5] % 4 == 0 && setTime[3] > 29 && setTime[4] == 2) {
     highLim[3] = 29;
     setTime[3] = 29;
   } else if (setTime[5] % 4 == 0 && setTime[4] == 2) {
     highLim[3] = 29;
   } else if (setTime[5] % 4 != 0 && setTime[3] > 28 && setTime[4] == 2) {
     highLim[3] = 28;
     setTime[3] = 28;
   } else if (setTime[5] % 4 != 0 && setTime[4] == 2) {
     highLim[3] = 28;
   } else if ((setTime[4] <= 7 && setTime[4] % 2 == 0 && setTime[4] != 2 && setTime[3] == 31) || (setTime[4] >= 9 && setTime[4] % 2 == 1 && setTime[3] == 31)) {
     highLim[3] = 30;
     setTime[3] = 30;
   } else if ((setTime[4] <= 7 && setTime[4] % 2 == 0 && setTime[4] != 2) || (setTime[4] >= 9 && setTime[4] % 2 == 1)) {
     highLim[3] = 30;
   } else {
     highLim[3] = 31;
   }

   for (int i = 0; i < 6; ++i) {
    if (i < 3) {
      markerX = i * MENU_ROW_WIGHT + 7*seconddot + 15;
      markerY = 20;
    }
    if (i >= 3) {
      markerX = (i - 3) * MENU_ROW_WIGHT + 7*line + 15;
      markerY = 30;
    }

    if (i == posit) {
      display.setTextColor(WHITE, BLACK);
      display.fillRect(markerX - 1, markerY - 1, 13, 9, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
      display.fillRect(markerX - 1, markerY - 1, 13, 14, WHITE);
    }

    display.setCursor(markerX, markerY);
    if (setTime[i] < 10) {
      display.print(0);
      display.setCursor(markerX + 6, markerY);
      display.print(setTime[i]);
    } else {
      display.print(setTime[i]);
    }

    if (i == 0 || i == 1) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(markerX + 13, markerY);
      display.print(":");
      seconddot++;
    }

    if (i == 3 || i == 4) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(markerX + 13, markerY);
      display.print("/");
      line++;
    }
   }

  display.display();
}


void showTimeMenu(){
   display.clearDisplay();
   display.setTextColor(BLACK, WHITE);
   display.setCursor(28, 10); 
   display.print("Time:");
   int markerX = 0;
   int markerY = 0;
   int seconddot = 0;
   int line = 0;

   if (data[5] % 4 == 0 && data[3] > 29 && data[4] == 2) {
     highLim[3] = 29;
     data[3] = 29;
   } else if (data[5] % 4 == 0 && data[4] == 2) {
     highLim[3] = 29;
   } else if (data[5] % 4 != 0 && data[3] > 28 && data[4] == 2) {
     highLim[3] = 28;
     data[3] = 28;
   } else if (data[5] % 4 != 0 && data[4] == 2) {
     highLim[3] = 28;
   } else if ((data[4] <= 7 && data[4] % 2 == 0 && data[4] != 2 && data[3] == 31) || (data[4] >= 9 && data[4] % 2 == 1 && data[3] == 31)) {
     highLim[3] = 30;
     data[3] = 30;
   } else if ((data[4] <= 7 && data[4] % 2 == 0 && data[4] != 2) || (data[4] >= 9 && data[4] % 2 == 1)) {
     highLim[3] = 30;
   } else {
     highLim[3] = 31;
   }

   for (int i = 0; i < 6; ++i) {
    if (i < 3) {
      markerX = i * MENU_ROW_WIGHT + 7*seconddot + 15;
      markerY = 20;
    }
    if (i >= 3) {
      markerX = (i - 3) * MENU_ROW_WIGHT + 7*line + 15;
      markerY = 30;
    }

    if (i == posit) {
      display.setTextColor(WHITE, BLACK);
      display.fillRect(markerX - 1, markerY - 1, 13, 9, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
      display.fillRect(markerX - 1, markerY - 1, 13, 14, WHITE);
    }

    display.setCursor(markerX, markerY);
    if (data[i] < 10) {
      display.print(0);
      display.setCursor(markerX + 6, markerY);
      display.print(data[i]);
    } else {
      display.print(data[i]);
    }

    if (i == 0 || i == 1) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(markerX + 13, markerY);
      display.print(":");
      seconddot++;
    }

    if (i == 3 || i == 4) {
      display.setTextColor(BLACK, WHITE);
      display.setCursor(markerX + 13, markerY);
      display.print("/");
      line++;
    }
   }

  display.display();
}

template <typename T>
void showMenu(int menuStartAt, int menuPos, T MENU_LENGTH, String menu[], int watch = 0){
  if (watch == 0) {
    display.clearDisplay();
   for (int i = menuStartAt; i < (menuStartAt + LCD_ROWS); i++) {
    int markerY = (i - menuStartAt) * MENU_ROW_HEIGHT;
    
    if (i == menuPos) {
      display.setTextColor(WHITE, BLACK);
      display.fillRect(0, markerY, display.width(), MENU_ROW_HEIGHT, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
      display.fillRect(0, markerY, display.width(), MENU_ROW_HEIGHT, WHITE);
    }

    if (i >= MENU_LENGTH) {
      continue;
    }

    display.setCursor(2, markerY + 2);
    display.print(menu[i]);
  }
  } else {
    display.clearDisplay();
   for (int i = menuStartAt; i < (menuStartAt + LCD_ROWS); i++) {
    int markerY = (i - menuStartAt) * MENU_ROW_HEIGHT;
    
    if (i == menuPos) {
      display.setTextColor(WHITE, BLACK);
      display.fillRect(0, markerY, display.width(), MENU_ROW_HEIGHT, BLACK);
    } else {
      display.setTextColor(BLACK, WHITE);
      display.fillRect(0, markerY, display.width(), MENU_ROW_HEIGHT, WHITE);
    }

    if (i >= MENU_LENGTH) {
      continue;
    }

    display.setCursor(2, markerY + 2);
    display.print(menu[i]);
  }
  if (hours >= 10) {
    display.setCursor(18, 33);
    display.print(hours);
  } else {
    display.setCursor(18, 33);
    display.print("0");
    display.setCursor(24, 33);
    display.print(hours);
  }  
  display.setCursor(30, 33);
  display.print(":");
  if (minutes >= 10) {
    display.setCursor(36, 33);
    display.print(minutes);
  } else {
    display.setCursor(36, 33);
    display.print("0");
    display.setCursor(42, 33);
    display.print(minutes);
  }
  display.setCursor(48, 33);
  display.print(":");
  if (seconds >= 10) {
    display.setCursor(54, 33);
    display.print(seconds);
  } else {
    display.setCursor(54, 33);
    display.print("0");
    display.setCursor(60, 33);
    display.print(seconds);
  }

  if (day >= 10) {
    display.setCursor(18, 41);
    display.print(day);
  } else {
    display.setCursor(18, 41);
    display.print("0");
    display.setCursor(24, 41);
    display.print(day);
  }  
  display.setCursor(30, 41);
  display.print("/");
  if (month >= 10) {
    display.setCursor(36, 41);
    display.print(month);
  } else {
    display.setCursor(36, 41);
    display.print("0");
    display.setCursor(42, 41);
    display.print(month);
  }
  display.setCursor(48, 41);
  display.print("/");
  if (year >= 10) {
    display.setCursor(54, 41);
    display.print(year);
  } else {
    display.setCursor(54, 41);
    display.print("0");
    display.setCursor(60, 41);
    display.print(year);
  }
  }
   

  display.display();
}

void setup() {

  Serial.begin(9600);
  time.begin();

  EEPROM.get(NUMBER_OF_EVENTS, numberOfEvents);

  for (int i = 0; i < numberOfEvents; i++) {
    EEPROM.get(SIZE_OF_EVENT * i, arrayOfEvents[i].year);
    EEPROM.get(SIZE_OF_EVENT * i + 1, arrayOfEvents[i].month);
    EEPROM.get(SIZE_OF_EVENT * i + 2, arrayOfEvents[i].day);
    EEPROM.get(SIZE_OF_EVENT * i + 3, arrayOfEvents[i].hour);
    EEPROM.get(SIZE_OF_EVENT * i + 4, arrayOfEvents[i].minute);
    EEPROM.get(SIZE_OF_EVENT * i + 5, arrayOfEvents[i].second);
    EEPROM.get(SIZE_OF_EVENT * i + 6, arrayOfEvents[i].lengthOfName);
    EEPROM.get(SIZE_OF_EVENT * i + 7, arrayOfEvents[i].lengthOfDes);
    char ch;
    for (int j = 0; j < arrayOfEvents[i].lengthOfName; j++) {
        EEPROM.get(SIZE_OF_EVENT * (i + 1) - (67 - j), ch);
        arrayOfEvents[i].nameOfEvent += ch;
    }
    for (int j = 0; j < arrayOfEvents[i].lengthOfDes; j++) {
        EEPROM.get(SIZE_OF_EVENT * (i + 1) - (53 - j), ch);
        arrayOfEvents[i].desOfEvent += ch;
    }
    eventsMenu[i] = arrayOfEvents[i].nameOfEvent;
  }
  
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_F, INPUT_PULLUP);

  display.begin();
  display.setContrast(50);
  display.setTextSize(1);
  display.clearDisplay();
  display.display();

  showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
}

void loop() {
  
  seconds = atoi(time.gettime("s"));
  minutes = atoi(time.gettime("i"));
  hours = atoi(time.gettime("H"));
  day = atoi(time.gettime("d"));
  month = atoi(time.gettime("m"));
  year = atoi(time.gettime("y"));
  for (int i = 0; i < numberOfEvents; ++i) {
    if (seconds == arrayOfEvents[i].second &&
        minutes == arrayOfEvents[i].minute &&
        hours == arrayOfEvents[i].hour &&
        day == arrayOfEvents[i].day &&
        month == arrayOfEvents[i].month &&
        year == arrayOfEvents[i].year) {
          while (!isButtonDown(BTN_C)) {
            showError(arrayOfEvents[i].nameOfEvent, arrayOfEvents[i].desOfEvent);
            delay(100);
          }
          arrayOfEvents[i].year = 100;
          arrayOfEvents[i].nameOfEvent = "";
          arrayOfEvents[i].desOfEvent = "";
          sortOfEvents();
          numberOfEvents--;
          EEPROM.put(NUMBER_OF_EVENTS, numberOfEvents);

          for (int i = 0; i < numberOfEvents; i++) {
              EEPROM.put(SIZE_OF_EVENT * i, arrayOfEvents[i].year);
              EEPROM.put(SIZE_OF_EVENT * i + 1, arrayOfEvents[i].month);
              EEPROM.put(SIZE_OF_EVENT * i + 2, arrayOfEvents[i].day);
              EEPROM.put(SIZE_OF_EVENT * i + 3, arrayOfEvents[i].hour);
              EEPROM.put(SIZE_OF_EVENT * i + 4, arrayOfEvents[i].minute);
              EEPROM.put(SIZE_OF_EVENT * i + 5, arrayOfEvents[i].second);
              EEPROM.put(SIZE_OF_EVENT * i + 6, arrayOfEvents[i].lengthOfName);
              EEPROM.put(SIZE_OF_EVENT * i + 7, arrayOfEvents[i].lengthOfDes);
              for (int j = 0; j < arrayOfEvents[i].lengthOfName; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (67 - j), arrayOfEvents[i].nameOfEvent[j]);
              }
              for (int j = 0; j < arrayOfEvents[i].lengthOfDes; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (53 - j), arrayOfEvents[i].desOfEvent[j]);
              }
          }
          
          levelMenu = 0;
          showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
    }
  }
  
  if (levelMenu == 0){
    showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {   
      if (startMenuPos < START_MENU_LENGTH - 1) {
        startMenuPos++;
  
        if (startMenuPos > 3) {
          startMenuStartAt++;
        }
        
        showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
      }   
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      if (startMenuPos > 0) {
        startMenuPos--;
  
        if (startMenuStartAt > 0) {
          startMenuStartAt--;
        }
        
        showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
      }
    }

    if (isButtonDown(BTN_C)) {
      levelMenu = levelMenu + 1 + startMenuPos * 4;
      if (levelMenu == 1){
        showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
      }
      if (levelMenu == 5){
        if (numberOfEvents == 0) {
          showError("No events!");
          delay(2000);
          levelMenu = 0;
          showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
        } else {
          showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
        }
      }
      if (levelMenu == 9) {
        showSetTime();
      }
    }
    delay(100);
  }

  if (levelMenu == 1) {
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {   
      if (addMenuPos < ADD_MENU_LENGTH - 1) {
        addMenuPos++;
  
        if (addMenuPos > 3) {
          addMenuStartAt++;
        }
        
        showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
      }
      
      delay(100);   
    }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      if (addMenuPos > 0) {
        addMenuPos--;
  
        if (addMenuStartAt > 0) {
          addMenuStartAt--;
        }
        
        showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
      }
      
      delay(100);
    }

    if (isButtonDown(BTN_B)) {
      addMenuPos = 0;
      levelMenu = 0;
      showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
    }

    if (isButtonDown(BTN_C)) {
      if (addMenuPos == 0) {
        levelMenu = 2;
        showTimeMenu();
      } else if (addMenuPos == 1) {
        levelMenu = 3;
        showNameMenu();
      } else if (addMenuPos == 2) {
        levelMenu = 4;
        showDesMenu();
      } else {
        if (numberOfEvents < LENGTH_EVENTS && nameOfEvent != " ") {
            arrayOfEvents[numberOfEvents].year = data[5];
            arrayOfEvents[numberOfEvents].month = data[4];
            arrayOfEvents[numberOfEvents].day = data[3];
            arrayOfEvents[numberOfEvents].hour = data[0];
            arrayOfEvents[numberOfEvents].minute = data[1];
            arrayOfEvents[numberOfEvents].second = data[2];
            arrayOfEvents[numberOfEvents].lengthOfName = nameOfEvent.length();
            arrayOfEvents[numberOfEvents].lengthOfDes = desOfEvent.length();
            arrayOfEvents[numberOfEvents].nameOfEvent = nameOfEvent;
            arrayOfEvents[numberOfEvents].desOfEvent = desOfEvent;
            eventsMenu[numberOfEvents] = nameOfEvent;
            numberOfEvents++;
            EEPROM.put(NUMBER_OF_EVENTS, numberOfEvents);
            
            sortOfEvents(); 

            for (int i = 0; i < numberOfEvents; i++) {
              EEPROM.put(SIZE_OF_EVENT * i, arrayOfEvents[i].year);
              EEPROM.put(SIZE_OF_EVENT * i + 1, arrayOfEvents[i].month);
              EEPROM.put(SIZE_OF_EVENT * i + 2, arrayOfEvents[i].day);
              EEPROM.put(SIZE_OF_EVENT * i + 3, arrayOfEvents[i].hour);
              EEPROM.put(SIZE_OF_EVENT * i + 4, arrayOfEvents[i].minute);
              EEPROM.put(SIZE_OF_EVENT * i + 5, arrayOfEvents[i].second);
              EEPROM.put(SIZE_OF_EVENT * i + 6, arrayOfEvents[i].lengthOfName);
              EEPROM.put(SIZE_OF_EVENT * i + 7, arrayOfEvents[i].lengthOfDes);
              for (int j = 0; j < arrayOfEvents[i].lengthOfName; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (67 - j), arrayOfEvents[i].nameOfEvent[j]);
              }
              for (int j = 0; j < arrayOfEvents[i].lengthOfDes; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (53 - j), arrayOfEvents[i].desOfEvent[j]);
              }
            }
            
            posit = 0;
            seconddot = 0;
            line = 0;
            for (int i = 0; i < 6; ++i) {
              data[i] = 0;
              if (i == 3 || i == 4) {
                data[i] = 1;
              }
            }
            positName = 0;
            codeOfSymbol = 32;
            Symbol = ' ';
            nameOfEvent = " ";
            positOfDes = 0;
            codeOfSymbolOfDes = 32;
            SymbolOfDes = ' ';
            desOfEvent = " ";
            addMenuPos = 0;
            levelMenu = 0;
            showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);          
        } else if (nameOfEvent == " ") {
          showError("No name!");
          delay(2000);
          showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);           
        } else {
          showError("Maximum events");
          delay(2000);
          showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
        }
      }
    }
  }

  if (levelMenu == 2) {
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {
      if (lowLim[posit] < data[posit]){
        data[posit]--;
        showTimeMenu();
        delay(200); 
      } else {
        data[posit] = highLim[posit];
        showTimeMenu();
        delay(200);   
      }
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {
      if (highLim[posit] > data[posit]){
        data[posit]++;
        showTimeMenu();
        delay(200);
      } else {
        data[posit] = lowLim[posit];
        showTimeMenu();
        delay(200);
      }
   }

   if (analogRead(PIN_ANALOG_X) < X_THRESHOLD_LOW) {
    if (posit > 0){
      posit--;
      showTimeMenu();
      delay(300);
    } else if (posit == 0) {
      posit = 5;
      showTimeMenu();
      delay(300);
    }
  }

    if (analogRead(PIN_ANALOG_X) > X_THRESHOLD_HIGH) {
      if (posit < 5){
         posit++;
         showTimeMenu();
         delay(300); 
      } else if (posit == 5) {
         posit = 0;
         showTimeMenu();
         delay(300);      
      }
   }

   if (isButtonDown(BTN_B)) {
      levelMenu = 1;
      showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
    }
    
  }

  if (levelMenu == 3) {
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {
      if (codeOfSymbol == 32) {
        codeOfSymbol = 46;
      } else if (codeOfSymbol == 46) {
        codeOfSymbol = 97;
      } else if (codeOfSymbol == 122) {
        codeOfSymbol = 65;
      } else if (codeOfSymbol == 90) {
        codeOfSymbol = 48;
      } else if (codeOfSymbol == 57) {
        codeOfSymbol = 32;
      } else {
        codeOfSymbol++;
      }
      
      Symbol = codeOfSymbol;
      nameOfEvent[positName] = Symbol;

      showNameMenu();
      delay(300);
  }

  if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {
      if (codeOfSymbol == 32) {
        codeOfSymbol = 57;
      } else if (codeOfSymbol == 48) {
        codeOfSymbol = 90;
      } else if (codeOfSymbol == 65) {
        codeOfSymbol = 122;
      } else if (codeOfSymbol == 97) {
        codeOfSymbol = 46;
      } else if (codeOfSymbol == 46) {
        codeOfSymbol = 32;
      } else {
        codeOfSymbol--;
      }
      
      Symbol = codeOfSymbol;
      nameOfEvent[positName] = Symbol;

      showNameMenu();
      delay(300);
  }

  if (analogRead(PIN_ANALOG_X) < X_THRESHOLD_LOW) {
    if (positName > 0) {
      if (positName + 1 == nameOfEvent.length() && nameOfEvent[positName] == ' ') {
        nameOfEvent = nameOfEvent.substring(0, nameOfEvent.length()- 1);
      }
      positName--;
      codeOfSymbol = nameOfEvent[positName];
      showNameMenu();
      delay(300);
    }
  }

  if (analogRead(PIN_ANALOG_X) > X_THRESHOLD_HIGH) {
      if (positName < 12) {
        positName++;
        if (positName == nameOfEvent.length()) {
          nameOfEvent += ' ';
        }
        codeOfSymbol = nameOfEvent[positName];
        showNameMenu();
        delay(300);
      }
  }

    if (isButtonDown(BTN_B)) {
      levelMenu = 1;
      showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
    }
  }

  if (levelMenu == 4) {
     if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {
      if (codeOfSymbolOfDes == 32) {
        codeOfSymbolOfDes = 46;
      } else if (codeOfSymbolOfDes == 46) {
        codeOfSymbolOfDes = 97;
      } else if (codeOfSymbolOfDes == 122) {
        codeOfSymbolOfDes = 65;
      } else if (codeOfSymbolOfDes == 90) {
        codeOfSymbolOfDes = 48;
      } else if (codeOfSymbolOfDes == 57) {
        codeOfSymbolOfDes = 32;
      } else {
        codeOfSymbolOfDes++;
      }
      
      SymbolOfDes = codeOfSymbolOfDes;
      desOfEvent[positOfDes] = SymbolOfDes;

      showDesMenu();
      delay(300);
  }

  if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {
      if (codeOfSymbolOfDes == 32) {
        codeOfSymbolOfDes = 57;
      } else if (codeOfSymbolOfDes == 48) {
        codeOfSymbolOfDes = 90;
      } else if (codeOfSymbolOfDes == 65) {
        codeOfSymbolOfDes = 122;
      } else if (codeOfSymbolOfDes == 97) {
        codeOfSymbolOfDes = 46;
      } else if (codeOfSymbolOfDes == 46) {
        codeOfSymbolOfDes = 32;
      } else {
        codeOfSymbolOfDes--;
      }
      
      SymbolOfDes = codeOfSymbolOfDes;
      desOfEvent[positOfDes] = SymbolOfDes;

      showDesMenu();
      delay(300);
  }

  if (analogRead(PIN_ANALOG_X) < X_THRESHOLD_LOW) {
    if (positOfDes > 0) {
      if (positOfDes + 1 == desOfEvent.length() && desOfEvent[positOfDes] == ' ') {
        desOfEvent = desOfEvent.substring(0, desOfEvent.length()- 1);
      }
      positOfDes--;
      codeOfSymbolOfDes = desOfEvent[positOfDes];
      showDesMenu();
      delay(300);
    }
  }

  if (analogRead(PIN_ANALOG_X) > X_THRESHOLD_HIGH) {
      if (positOfDes < 51) {
        positOfDes++;
        if (positOfDes == desOfEvent.length()) {
          desOfEvent += ' ';
        }
        codeOfSymbolOfDes = desOfEvent[positOfDes];
        showDesMenu();
        delay(300);
      }
  }

  if (isButtonDown(BTN_B)) {
      levelMenu = 1;
      showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
  }
  }

  if(levelMenu == 5) {
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {   
      if (eventsMenuPos < numberOfEvents - 1) {
        eventsMenuPos++;
  
        if (eventsMenuPos > 3) {
          eventsMenuStartAt++;
        }
        
        showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
      }
      
      delay(100);   
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      if (eventsMenuPos > 0) {
        eventsMenuPos--;
  
        if (eventsMenuStartAt > 0 && eventsMenuStartAt == eventsMenuPos + 1) {
          eventsMenuStartAt--;
        }
        
        showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
      }
      
      delay(100);
    }

    if (isButtonDown(BTN_B)) {
      levelMenu = 0;
      eventsMenuPos = 0;
      showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
    }

    if (isButtonDown(BTN_C)) {
      levelMenu = 6;
      showEvent(eventsMenuPos);
      
    }

    if (isButtonDown(BTN_D)) {
      
    }

    if (isButtonDown(BTN_A)) {
       levelMenu = 7;
       showDeleteMenu();
    }
  }

  if (levelMenu == 6) {
    showEvent(eventsMenuPos);
    if (isButtonDown(BTN_B)) {
      levelMenu = 5;
      numberOfPage = 0;
      showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
    }
    
  }

  if (levelMenu == 7) {
    showDeleteMenu();
    if (isButtonDown(BTN_C)){
      if (yesOrNo == 0) {
        levelMenu = 5;
        showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
      } else {
        arrayOfEvents[eventsMenuPos].year = 100;
        arrayOfEvents[eventsMenuPos].nameOfEvent = "";
        arrayOfEvents[eventsMenuPos].desOfEvent = "";
        sortOfEvents();
        numberOfEvents--;
        EEPROM.put(NUMBER_OF_EVENTS, numberOfEvents);

        for (int i = 0; i < numberOfEvents; i++) {
              EEPROM.put(SIZE_OF_EVENT * i, arrayOfEvents[i].year);
              EEPROM.put(SIZE_OF_EVENT * i + 1, arrayOfEvents[i].month);
              EEPROM.put(SIZE_OF_EVENT * i + 2, arrayOfEvents[i].day);
              EEPROM.put(SIZE_OF_EVENT * i + 3, arrayOfEvents[i].hour);
              EEPROM.put(SIZE_OF_EVENT * i + 4, arrayOfEvents[i].minute);
              EEPROM.put(SIZE_OF_EVENT * i + 5, arrayOfEvents[i].second);
              EEPROM.put(SIZE_OF_EVENT * i + 6, arrayOfEvents[i].lengthOfName);
              EEPROM.put(SIZE_OF_EVENT * i + 7, arrayOfEvents[i].lengthOfDes);
              for (int j = 0; j < arrayOfEvents[i].lengthOfName; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (67 - j), arrayOfEvents[i].nameOfEvent[j]);
              }
              for (int j = 0; j < arrayOfEvents[i].lengthOfDes; j++) {
                EEPROM.put(SIZE_OF_EVENT * (i + 1) - (53 - j), arrayOfEvents[i].desOfEvent[j]);
              }
        }
        
        if (numberOfEvents == eventsMenuPos && eventsMenuPos != 0) {
          eventsMenuPos--;
        }
        yesOrNo = 0;
        if (numberOfEvents == 0) {
          levelMenu = 0;
          showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
        } else {
          levelMenu = 5;
          showMenu(eventsMenuStartAt, eventsMenuPos, numberOfEvents, eventsMenu);
        }
      }
    }
  }

  if (levelMenu == 9) {
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {
      if (lowLim[posit] < setTime[posit]){
        setTime[posit]--;
        showSetTime();
        delay(200); 
      } else {
        setTime[posit] = highLim[posit];
        showSetTime();
        delay(200);   
      }
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {
      if (highLim[posit] > setTime[posit]){
        setTime[posit]++;
        showSetTime();
        delay(200);
      } else {
        setTime[posit] = lowLim[posit];
        showSetTime();
        delay(200);
      }
   }

   if (analogRead(PIN_ANALOG_X) < X_THRESHOLD_LOW) {
    if (posit > 0){
      posit--;
      showSetTime();
      delay(300);
    } else if (posit == 0) {
      posit = 5;
      showSetTime();
      delay(300);
    }
  }

    if (analogRead(PIN_ANALOG_X) > X_THRESHOLD_HIGH) {
      if (posit < 5){
         posit++;
         showSetTime();
         delay(300); 
      } else if (posit == 5) {
         posit = 0;
         showSetTime();
         delay(300);      
      }
   }

   if (isButtonDown(BTN_C)) {
    levelMenu = 0;
    time.settime(setTime[2], setTime[1], setTime[0], setTime[3], setTime[4], setTime[5]
    );
    showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
   }

   if (isButtonDown(BTN_B)) {
      levelMenu = 0;
      showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu, 1);
    }
  }
  
}

bool isButtonDown(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30);

    if (digitalRead(pin) == LOW) { 
      delay(100);     
      return true;
    }
    return false;
  }
  return false;
}

void replacement(int i, int j) {
  byte t;
  String str;
  
  t = arrayOfEvents[i].year;
  arrayOfEvents[i].year = arrayOfEvents[j].year;
  arrayOfEvents[j].year = t;

  t = arrayOfEvents[i].month;
  arrayOfEvents[i].month = arrayOfEvents[j].month;
  arrayOfEvents[j].month = t;

  t = arrayOfEvents[i].day;
  arrayOfEvents[i].day = arrayOfEvents[j].day;
  arrayOfEvents[j].day = t;

  t = arrayOfEvents[i].hour;
  arrayOfEvents[i].hour = arrayOfEvents[j].hour;
  arrayOfEvents[j].hour = t;

  t = arrayOfEvents[i].minute;
  arrayOfEvents[i].minute = arrayOfEvents[j].minute;
  arrayOfEvents[j].minute = t;

  t = arrayOfEvents[i].second;
  arrayOfEvents[i].second = arrayOfEvents[j].second;
  arrayOfEvents[j].second = t;

  str = arrayOfEvents[i].nameOfEvent;
  arrayOfEvents[i].nameOfEvent = arrayOfEvents[j].nameOfEvent;
  arrayOfEvents[j].nameOfEvent = str;

  str = arrayOfEvents[i].desOfEvent;
  arrayOfEvents[i].desOfEvent = arrayOfEvents[j].desOfEvent;
  arrayOfEvents[j].desOfEvent = str; 
  
  eventsMenu[i] = arrayOfEvents[i].nameOfEvent;
  eventsMenu[j] = arrayOfEvents[j].nameOfEvent;
}

void sortOfEvents() {
  for (int i = 0; i < numberOfEvents; i++) {
    for (int j = 0; j < numberOfEvents - 1; j++) {
        if (arrayOfEvents[j].year > arrayOfEvents[j + 1].year) replacement(j, j + 1);
        else if (arrayOfEvents[j].month > arrayOfEvents[j + 1].month) replacement(j, j + 1);
        else if (arrayOfEvents[j].day > arrayOfEvents[j + 1].day) replacement(j, j + 1);
        else if (arrayOfEvents[j].hour > arrayOfEvents[j + 1].hour) replacement(j, j + 1);
        else if (arrayOfEvents[j].minute > arrayOfEvents[j + 1].minute) replacement(j, j + 1);
        else if (arrayOfEvents[j].second > arrayOfEvents[j + 1].second) replacement(j, j + 1);
    } 
  }
}

void showDeleteMenu() {
  display.clearDisplay();
  display.setTextColor(BLACK, WHITE);
  display.setCursor(0, 0);
  display.print("Delete this   event?");

  if (analogRead(PIN_ANALOG_X) < X_THRESHOLD_LOW) {
    yesOrNo = 1;
  }

  if (analogRead(PIN_ANALOG_X) > X_THRESHOLD_HIGH) {
    yesOrNo = 0;
  }

  if (yesOrNo == 0) {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(20, 30);
    display.print("YES");
    

    display.setTextColor(WHITE, BLACK);
    display.fillRect(39, 29, 13, 9, BLACK);
    display.setCursor(40, 30);
    display.print("NO");
    
  } else {
    display.setTextColor(BLACK, WHITE);
    display.setCursor(40, 30);
    display.print("NO");

    display.setTextColor(WHITE, BLACK);
    display.fillRect(19, 29, 18, 9, BLACK);
    display.setCursor(20, 30);
    display.print("YES");
  }

  display.display();
}


void showEvent(int event) {
  if (numberOfPage == 0) {
   display.clearDisplay();
  display.setTextColor(BLACK, WHITE);  
  display.setCursor(28, 0); 
  display.print("Name:"); 
  display.setCursor(2,9);
  display.print(arrayOfEvents[event].nameOfEvent);
  display.setCursor(28, 18);
  display.print("Time:");
  
  if (arrayOfEvents[event].hour >= 10) {
    display.setCursor(18, 27);
    display.print(arrayOfEvents[event].hour);
  } else {
    display.setCursor(18, 27);
    display.print("0");
    display.setCursor(24, 27);
    display.print(arrayOfEvents[event].hour);
  }  
  display.setCursor(30, 27);
  display.print(":");
  if (arrayOfEvents[event].minute >= 10) {
    display.setCursor(36, 27);
    display.print(arrayOfEvents[event].minute);
  } else {
    display.setCursor(36, 27);
    display.print("0");
    display.setCursor(42, 27);
    display.print(arrayOfEvents[event].minute);
  }
  display.setCursor(48, 27);
  display.print(":");
  if (arrayOfEvents[event].second >= 10) {
    display.setCursor(54, 27);
    display.print(arrayOfEvents[event].second);
  } else {
    display.setCursor(54, 27);
    display.print("0");
    display.setCursor(60, 27);
    display.print(arrayOfEvents[event].second);
  }

  if (arrayOfEvents[event].day >= 10) {
    display.setCursor(18, 35);
    display.print(arrayOfEvents[event].day);
  } else {
    display.setCursor(18, 35);
    display.print("0");
    display.setCursor(24, 35);
    display.print(arrayOfEvents[event].day);
  }  
  display.setCursor(30, 35);
  display.print("/");
  if (arrayOfEvents[event].month >= 10) {
    display.setCursor(36, 35);
    display.print(arrayOfEvents[event].month);
  } else {
    display.setCursor(36, 35);
    display.print("0");
    display.setCursor(42, 35);
    display.print(arrayOfEvents[event].month);
  }
  display.setCursor(48, 35);
  display.print("/");
  if (arrayOfEvents[event].year >= 10) {
    display.setCursor(54, 35);
    display.print(arrayOfEvents[event].year);
  } else {
    display.setCursor(54, 35);
    display.print("0");
    display.setCursor(60, 35);
    display.print(arrayOfEvents[event].year);
  }

  display.drawLine(82, 0, 82, 24, BLACK);
  display.drawLine(83, 0, 83, 24, BLACK);
  }

  if (numberOfPage == 1) {
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);  
    display.setCursor(7, 0); 
    display.print("Description:");
    display.setCursor(2,9);
    display.print(arrayOfEvents[event].desOfEvent);
    display.drawLine(82, 24, 82, 48, BLACK);
    display.drawLine(83, 24, 83, 48, BLACK);
  }

  if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {   
      numberOfPage = 1;
      delay(100);   
    }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      numberOfPage = 0;
      delay(100); 
    }

  
  
  display.display();
}


void showNameMenu(){
   display.clearDisplay();
   display.setTextColor(BLACK, WHITE);
   display.setCursor(28, 0); 
   display.print("Name:");   
   int markerX = 0;
   int markerY = 0;
    

   for (int j = 0; j < 1; ++j) {
      for (int i = 0; i < 13 * (j + 1) && i < nameOfEvent.length(); ++i) {
      markerX = (i - j * 13) * 6 + 1;
      markerY = 10 * (j + 1);
      if (i == positName) {
        display.setTextColor(WHITE, BLACK);
        display.fillRect(markerX - 1, markerY - 1, 7, 9, BLACK);
      } else {
        display.setTextColor(BLACK, WHITE);
        display.fillRect(markerX, markerY - 1, 6, 9, WHITE);
      }


      display.setCursor(markerX, markerY);
      display.print(nameOfEvent[i]);

      }
   } 

   display.display();
}

void showDesMenu(){
   display.clearDisplay();
   display.setTextColor(BLACK, WHITE);
   display.setCursor(7, 0); 
   display.print("Description:");   
   int markerX = 0;
   int markerY = 0;
    

   for (int j = 0; j < 4; ++j) {
      for (int i = 0; i < 13 * (j + 1) && i < desOfEvent.length(); ++i) {
      markerX = (i - j * 13) * 6 + 1;
      markerY = 10 * (j + 1);
      if (i == positOfDes) {
        display.setTextColor(WHITE, BLACK);
        display.fillRect(markerX - 1, markerY - 1, 7, 9, BLACK);
      } else {
        display.setTextColor(BLACK, WHITE);
        display.fillRect(markerX, markerY - 1, 6, 9, WHITE);
      }


      display.setCursor(markerX, markerY);
      display.print(desOfEvent[i]);

      }
   } 

   display.display();
}
