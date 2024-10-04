#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>// include the library code:

LiquidCrystal_I2C lcd(0x27, 16, 2);  // initialize the Liquid Crystal Display object with the I2C address 0x27, 16 columns and 2 rows

#define SS_PIN 10 // RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

byte card_ID[4]; // Card UID

// Add as many cards as you want
byte Name1[4] = {0xC3, 0x7F, 0xF9, 0xE3}; // First UID card
byte Name2[4] = {0x34, 0x98, 0xF1, 0xA7}; // Second UID card
// String Name3="15353114169";//third UID card
// String Name4="13937143185";//fourth UID card
// String Name5="2174114201";//
// String Name6="254161178115";//and so on.

int NumbCard[2] = {0}; // The number of cards. In this case, we have 2 cards.
int statu[2] = {0};    // The number of cards. In this case, we have 2 cards.

int const RedLed = 6;
int const GreenLed = 5;
int const Buzzer = 8;

String Name; // User name
long Number; // User number
String ExcelName = "Logs";
int L = 0;
int n; // The number of cards you want to detect (optional)
int ID = 1;

void setup() {
  //lcd.begin(16, 2);
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  lcd.init();       // initialize the LCD
  lcd.clear();      // clear the LCD display
  lcd.backlight();  // Make sure backlight is on
  
  Serial.println("CLEARSHEET"); // Clears starting at row 1
  Serial.println("LABEL,ID,Date,Name,Number,Card ID,Time IN,Time OUT"); // Make four columns (Date,Time,[Name:"user name"],[Number:"user number"])

  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  
  lcd.setCursor(2,0);
  lcd.print("Good Morning");// Welcome Message will appear in the beginning. 
  delay(200);
}

void loop() {
  // Look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Go to start of loop if there is no card present
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // If read card serial(0) returns 1, the uid struct contains the ID of the read card.
  }

  // Read the card ID
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
  }

  //Serial.println(card_ID);// Uncomment this line to scan the card ID and appear it on Serial monitor.

  // Check the card ID against known IDs
  int j = -1; // Initialize j to an invalid index
  int s = -1; // Initialize s to an invalid index

  if (compareArrays(card_ID, Name1, 4)) {
    Name = "Saurav Kumar"; // User name
    Number = 123456; // User number
    j = 0; // Index for Name1
    s = 0; // Status index for Name1
  }
  else if (compareArrays(card_ID, Name2, 4)) {
    Name = "Srijan"; // User name
    Number = 789101; // User number
    j = 1; // Index for Name2
    s = 1; // Status index for Name2
  }
  else {
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, HIGH);
    delay(1000);
    digitalWrite(RedLed, LOW);
    clearCardID();
    return;
  }

  if (j != -1 && s != -1) {
    if (NumbCard[j] == 1 && statu[s] == 0) {
      statu[s] = 1;
      NumbCard[j] = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Good Bye");
      lcd.setCursor(0,1);
      lcd.print(Name);
      Serial.print("DATA,"); // Send the Name to excel
      Serial.print(ID);
      Serial.print(",");
      Serial.print("DATE");
      Serial.print(",");
      Serial.print(Name);
      Serial.print(",");
      Serial.print(Number); // Send the Number to excel
      Serial.print(",");
      printCardID(); // Send the card ID to excel
      Serial.print(",");
      Serial.print("");
      Serial.print(",");
      Serial.println("TIME");
      ID = ID + 1;
      n++; // Optional
      digitalWrite(GreenLed, HIGH);
      digitalWrite(RedLed, LOW);
      digitalWrite(Buzzer, HIGH);
      delay(30);
      digitalWrite(Buzzer, LOW);
    }
    else if (NumbCard[j] == 0) {
      NumbCard[j] = 1;
      statu[s] = 0;
      n++; // Optional
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print("Welcome ");
      lcd.setCursor(0,1);
      lcd.print(Name);
      Serial.print("DATA,"); // Send the Name to excel
      Serial.print(ID);
      Serial.print(",");
      Serial.print("DATE");
      Serial.print(",");
      Serial.print(Name);
      Serial.print(",");
      Serial.print(Number); // Send the Number to excel
      Serial.print(",");
      printCardID(); // Send the card ID to excel
      Serial.print(",");
      Serial.print("TIME");
      Serial.print(",");
      Serial.println("");
      digitalWrite(GreenLed, HIGH);
      digitalWrite(RedLed, LOW);
      digitalWrite(Buzzer, HIGH);
      delay(30);
      digitalWrite(Buzzer, LOW);
      ID = ID + 1;
    }
  }
  
  delay(1000);
  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);
  clearCardID();
}

bool compareArrays(byte a[], byte b[], int length) {
  for (int i = 0; i < length; i++) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

void printCardID() {
  for (byte i = 0; i < 4; i++) {
    Serial.print(card_ID[i], HEX);
  }
}

void clearCardID() {
  for (byte i = 0; i < 4; i++) {
    card_ID[i] = 0;
  }
}
