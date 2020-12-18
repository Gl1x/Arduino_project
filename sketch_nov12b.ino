#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 10, 11, 12, 13);

const byte START_MENU_LENGTH = 2;
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

int positName = 0;
int codeOfSymbol = 32;
char Symbol = ' ';
String nameOfEvent = " ";

int positOfDes = 0;
int codeOfSymbolOfDes = 32;
char SymbolOfDes = ' ';
String desOfEvent = " ";

struct Event{
  byte year;
  byte month;
  byte day;
  byte hour;
  byte minute;
  byte second;
  String nameOfEvent;
  String descriptionOfEvent;
};

String startMenu[2] = {
  "Add event",
  "Events"
};

String addMenu[4] = {
  "Time",
  "Name",
  "Description",
  "Save"
};

String eventsMenu[2] = {
  "First",
  "Second"
};

void setup() {
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

  showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
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

void loop() {
  if (levelMenu == 0){
    if (analogRead(PIN_ANALOG_Y) < Y_THRESHOLD_LOW) {   
      if (startMenuPos < START_MENU_LENGTH - 1) {
        startMenuPos++;
  
        if (startMenuPos > 3) {
          startMenuStartAt++;
        }
        
        showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
      }
      
      delay(100);   
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      if (startMenuPos > 0) {
        startMenuPos--;
  
        if (startMenuStartAt > 0) {
          startMenuStartAt--;
        }
        
        showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
      }
      
      delay(100);
    }

    if (isButtonDown(BTN_C)) {
      levelMenu = levelMenu + 1 + startMenuPos * 4;
      if (levelMenu == 1){
        showMenu(addMenuStartAt, addMenuPos, ADD_MENU_LENGTH, addMenu);
      }
      if (levelMenu == 5){
        showMenu(eventsMenuStartAt, eventsMenuPos, eventsMenuLength, eventsMenu);
      }
    }
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
      showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
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
        addMenuPos = 0;
        levelMenu = 0;
        showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
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
      if (eventsMenuPos < eventsMenuLength - 1) {
        eventsMenuPos++;
  
        if (eventsMenuPos > 3) {
          eventsMenuStartAt++;
        }
        
        showMenu(eventsMenuStartAt, eventsMenuPos, eventsMenuLength, eventsMenu);
      }
      
      delay(100);   
  }

    if (analogRead(PIN_ANALOG_Y) > Y_THRESHOLD_HIGH) {    
      if (eventsMenuPos > 0) {
        eventsMenuPos--;
  
        if (eventsMenuStartAt > 0) {
          eventsMenuStartAt--;
        }
        
        showMenu(eventsMenuStartAt, eventsMenuPos, eventsMenuLength, eventsMenu);
      }
      
      delay(100);
    }

    if (isButtonDown(BTN_B)) {
      levelMenu = 0;
      showMenu(startMenuStartAt, startMenuPos, START_MENU_LENGTH, startMenu);
    }

    if (isButtonDown(BTN_C)) {
    }
  }
  
}


template <typename T>
void showMenu(int menuStartAt, int menuPos, T MENU_LENGTH, String menu[]){
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
