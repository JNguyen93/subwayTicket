#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN   9
#define SS_PIN    10

const char* master = "65F42D6A";

const char* ticket0 = "FD2EAD5";
const char* ticket0bu = "4DC3C640";
const char* ticket0bu0 = "DDDECA40";

const char* ticket1 = "1D5FD740";
const char* ticket1bu = "EDAC75";
const char* ticket1bu0 = "CD32BB40";


const char* ticket2 = "7D14B940";
const char* ticket2bu = "7DCEA16";
const char* ticket2bu0 = "CD67A36";

const char* ticket3 = "DD95D040";
const char* ticket3bu = "1D23CA40";
const char* ticket3bu0 = "DD7CCC40";

const byte maglock = 7;
const byte button = 6;
byte readCard[4];
char* approvedTicket0 = ticket3bu0;
char* approvedTicket1 = ticket3bu;
char* myTags[100] = {};
char* preapproved[3] = {master, approvedTicket0, approvedTicket1};
int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
int buttonCount = 0;

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // Initiating
  SPI.begin();        // SPI bus
  mfrc522.PCD_Init(); //  MFRC522
  Serial.begin(9600);
  Serial.println("RFID Reader");
  pinMode(maglock, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  if(digitalRead(button)){
    Serial.println("Reset");
    digitalWrite(maglock, LOW);
    delay(1000);
    buttonCount++;
  }
  else{
    buttonCount = 0;
  }

  if(buttonCount == 3){
    programmingMode();
  }
  
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    correctTag = false;
    tagID = getTagID();
    // Checks whether the scanned tag is authorized
    Serial.print("Validating tagID: ");
    Serial.println(tagID);
    for (int j = 0; j < 3; j++) {
      if (tagID == preapproved[j]) {
        Serial.println("Access Granted!");
        Serial.println("Preapproved Tag.");
        correctTag = true;
      }
    }
    if (!correctTag) {
      for (int i = 0; i < 100; i++) {
        if (tagID == myTags[i]) {
          Serial.println("Access Granted!");
          Serial.println("Approved Tag.");
          correctTag = true;
        }
      }
    }
    if (!correctTag) {
      Serial.println("Access Denied.");
      Serial.println("LOW");
      digitalWrite(maglock, LOW);
    }
    else{
      Serial.println("Unlocking...");
      digitalWrite(maglock, HIGH);
    }
  }
}

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

String getTagID(){
  String tag = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tag.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tag.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return tag;
}

void programmingMode(){
  correctTag = false;
  Serial.print("Programming mode:");
  Serial.println(" Add/Remove Tag");
  while (!successRead || digitalRead(button)) {
    tagID = "";
    successRead = getID();
    if (successRead == true) {
      for (int i = 0; i < 100; i++) {
        if (tagID == myTags[i]) {
          myTags[i] = "";
          Serial.println("Tag Removed!");
          return;
        }
      }
      myTags[tagsCount] = strdup(tagID.c_str());
      Serial.println("Tag Added!");
      tagsCount++;
      return;
    }
  }
  Serial.println("Programming Mode Exited");
}

