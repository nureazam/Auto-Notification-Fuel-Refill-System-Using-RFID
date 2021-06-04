#include <SPI.h>
#include <Ethernet.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>

/*.............................................Keypad Setting........................*/

const byte rows = 4;
const byte cols = 4;

char keyMap [rows][cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins [rows] = {22,23,24,25};
byte colPins [cols] = {26,27,28,29};

Keypad myKeypad = Keypad(makeKeymap(keyMap), rowPins, colPins, rows, cols);

/*.............................................Keypad Setting........................*/

/*.............................................RFID Setting........................*/

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

/*.............................................RFID Setting........................*/

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 0, 11 }; //ip of ethernet shield
byte server[] = { 192, 168, 0, 9 }; // Router ip

EthernetClient client;

/*----------------------------------------Global Variables------------------------------------*/
  int echo = 2; // Echo pin of Sonar sensor
  int trig = 3; // Trig pin of onar sensor
  float duration = 0.0; //Duration of wave travel from sonar sensor 
  int distance = 0; // distance measured my sonar sensor
  String rfid_no = ""; // Stores RFID No
  String password_fetched = ""; //Stores user password fetched from server
  String password_1 = ""; //Strores user inputed password
  int relay = 4; //relay to control pump. Relay is active low module
  int full_tank = 2; //sonar value when tank is full
  int empty_tank = 10; //sonar value when tank is empty
  int rate_of_dispense = 1; // 1 liter = 1 unit
  boolean input_quantity = false; //inidicates entered inputs from keypad is fuel quantity
  boolean password_entered = false; //inidicates user finished his password input
  boolean amount_of_fuel_entered = false; //checks if user puted the amount of fuel or not
  boolean out_of_fuel = false; //indicates tank is out of fuel or not
  boolean rfid_scanned = false; //checks if rfid is scanned or not
  boolean password_fetch = false; //checks if password is fetched from server or not
  boolean password_match = false; //checks if password is matched or not
  boolean data_sent = false;
  boolean fuel_refilled = false;
  boolean get_authorized = false;
  boolean send_statement = false;
  boolean valid_order = false;//trace if order is valid or not
  String quantity = ""; //quantity of fuel user wants to buy
/*----------------------------------------Global Variables------------------------------------*/

void setup() {

  /*------------------------------------------Defining pin mode--------------------------*/
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(relay,OUTPUT);
  /*------------------------------------------Defining pin mode--------------------------*/
 
  /*------------------------------------initialize variable to default---------------------*/
  distance = 0; // distance measured my sonar sensor
  rfid_no = ""; // Stores RFID No
  rfid_scanned = false; //checks if rfid is scanned or not
  password_match = false; //checks if password is matched or not
  amount_of_fuel_entered = false; //checks if user puted the amount of fuel or not
  digitalWrite(relay,HIGH); //Relay is active low
  get_authorized = true;
  send_statement = true;
  digitalWrite(trig, LOW);
  
  /*------------------------------------initialize variable to default---------------------*/
 
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  myKeypad.addEventListener(keypadEvent); //Keypad Event Initialization

  /*------------------------------------connecting client to server------------------------*/ 
  Ethernet.begin(mac, ip);
  client.connect(server, 80);
  if(client.connected())
  {
    Serial.println("Connected");
  }
  /*------------------------------------connecting client to server------------------------*/

}

void loop() {
  // put your main code here, to run repeatedly:

  distance = findDistance();
  //Serial.println(distance);

   if(distance <= 10)
   {
      
      if(!rfid_scanned)
      {
        Serial.println("Scan your RFID");
        scanRFID();
      }
      else if(rfid_scanned && !password_fetch)
      {
        Serial.println("Please Wait....");
        fetchPassword();      
      }
      else if(rfid_scanned && password_fetch && !password_match)
      {
        Serial.println("Enter Password");
        getPassword();
      }
      else if(rfid_scanned && password_fetch && password_match && !amount_of_fuel_entered)
      {
        getOrder();
      }
      else if(rfid_scanned && password_fetch && password_match && amount_of_fuel_entered && !data_sent)
      {
        sendQuantityToDB();
      }
      else if(rfid_scanned && password_fetch && password_match && amount_of_fuel_entered && data_sent && !fuel_refilled)
      {
        refillFuel();
      }
   }

   else
   {
    Serial.println("Pump is out of fuel");
   }

      
  //delay(2000);
 
}

/*----------------------------------------------------sendStatement Function--------------------------------------*/
/*
 * Checks the connection with client and server.
 * If connection is OK, then sends data to pump_to_server.php
 * If all the the datas are valid, then datas should be written in statement table of DATA BASE.
 */

 /*
void sendStatement(float balance, float p_balance, float cost, char phone_no[], char fuel_type[], float fuel_qnt)
{
  
  if(client.connect(server,80))
  {
    Serial.println("Connected");

    client.print("GET /statement_to_server.php?");
    client.print("balance=");
    client.print(balance);
    client.print("&p_balance=");
    client.print(p_balance);
    client.print("&cost=");
    client.print(cost);
    client.print("&phone_no=");
    client.print(phone_no);
    client.print("&fuel_type=");
    client.print(fuel_type);
    client.print("&fuel_qnt=");
    client.println(fuel_qnt);
    client.println(" HTTP/1.1");
    //client.println("Host: 127.0.0.1");
    client.println("Connectoin: close");
    client.stop();
    
    
    Serial.print("Balance: ");
    Serial.println(balance);
    Serial.print("Previous Balance: ");
    Serial.println(p_balance);
    Serial.print("Cost: ");
    Serial.println(cost);
    Serial.print("Phone No: ");
    Serial.println(phone_no);
    Serial.print("Fuel Type: ");
    Serial.println(fuel_type);
    Serial.print("Fuel Quantity: ");
    Serial.println(fuel_qnt);
  }
  else {
  // if you didn't get a connection to the server:
  Serial.println("connection failed");
  }
}

*/


/*---------------------------------------------------Scan RFID CARD-----------------------*/
/*
 * Look for new RFID CARD
 * Scans the RFID and convers it to readable string
 * Makes all strings to uppercase 
 * Saves RFID number to a variable named content
 */
void scanRFID()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  //Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  //Serial.print("Message : ");
  content.toUpperCase();
  rfid_no = content;
  rfid_scanned = true;
  
}


/*---------------------------------------------------Get Password-----------------------*/
/*
 * User must type B before entering password to erase previous data
 * After that he gives his password and finishes by pressing *
 * This function will compare the fetched password and user given password 
 * If matched, it will make match_password = True
 */

void getPassword()
{
  myKeypad.getKey();
  if (password_entered)
  {
    if(password_fetched.indexOf(password_1) > 0)
    {
      Serial.print("access granted!");
      password_match = true; 
    }
    else
    {
      Serial.println("Wrong Password!");
      password_1 = "";
      password_match = false;
      password_entered = false;
    }
  }
  
}

/*---------------------------------------------------Get Order from user-----------------------*/
/*
 * Takes the amount of fuel in Liter
 * Stores it into a variable quantity
 * Makes amount_of_fuel_entered == true
 */
void getOrder()
{
  Serial.println("Amount of fuel: (Liter)");
  myKeypad.getKey();
}

/*---------------------------------------------------Keypad Event Listener-----------------------*/
/*
 * Listen the keypad's key
 * If 'A' is pressed, then the variable quantity becomes empty. 
 * "#" key defines user has finished his/her input for amount of fuel
 */
void keypadEvent (KeypadEvent eKey)
{
  switch(myKeypad.getState())
  {
  case PRESSED:
    switch (eKey)
    {
      case 'A':
        quantity = "";
        input_quantity = true;
        break;
      case '#':
        amount_of_fuel_entered = true;
        input_quantity = false;
        Serial.println(quantity);
        break;
      case 'B':
          input_quantity = false;
          password_1 = "";
          break;
      case '*':
          if(password_1.length() >= 8)
          {
            password_entered = true;
            Serial.println(password_1);
            break;
          }
          else if(password_1.length()<8)
          {
            password_entered = false;
            Serial.println("Wrong Password");
            password_1 = "";
            break;
          }
          
      default:
        //Serial.print(eKey);
        if(input_quantity)
        {
          quantity.concat(String(eKey)); 
        }
        else if (!input_quantity)
        {
          password_1.concat(String(eKey));
          Serial.println(password_1);  
        }
    }
  }
}
/*---------------------------------------------------Keypad Event Listener-----------------------*/

/*-------------------------------------------------Find Distance through sonar sensor------------*/
/*
 * Finds the depth of fuel in fuel container & returns the value
 */
int findDistance()
{
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration*0.034)/2;

  return distance;
}

/*-------------------------------------------------Find Distance through sonar sensor------------*/


/*-------------------------------------------------Control Motor/pump-------------------------*/

void pumpOn()
{
  digitalWrite(relay,LOW);
}

void pumpOff()
{
  digitalWrite(relay,HIGH);
}

/*-------------------------------------------------Control Motor/pump-------------------------*/


/*----------------------------------------------------fetchPassword Function--------------------------------------*/
/*
 * checks the connection of client to server.
 * if the connection is ok, then sends the RFID_NO of user to get_password.php.php file.
 * The php file returns users password if RFID_NO is valid.
 * Client server recieves many more informations with user's phone no. So inside client.available() checking -
 * We filtered the password and stored it into password1 variable.
 * Finally Disconnect the clinet from the server.
 */
 
void fetchPassword()
{
  if (client.connected()) {
    //Serial.println("connected");
    client.print("GET /get_password.php?");
    client.print("rfid_no=");
    client.print(rfid_no);
    client.println(" HTTP/1.0");
    client.println();
    
    if (client.available()) 
    {
      char c = client.read();
      //Serial.print(c);
      password_fetched.concat(String(c));
      
    }    

    if (!client.connected()) {
      password_fetch = true;
      Serial.println(password_fetched);
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      //for(;;);
  }
  } 
  else 
  {
    Serial.println("connection failed");
  }
  
}

/*...................................................Calculate cost function...................................*/

void sendQuantityToDB()
{
  int quantity_n = quantity.toInt();
  
  if (client.connected()) {
    //Serial.println("connected");
    client.print("GET /cost_to_server.php?");
    client.print("rfid_no=");
    client.print(rfid_no);
    client.print("&&quantity=");
    client.print(quantity_n);
    client.println(" HTTP/1.0");
    client.println();
    
    if (client.available()) 
    {
      char c = client.read();
      Serial.print(c);
    }    

    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      data_sent = true;
      client.stop();
      //for(;;);
  }
  } 
  else if(!client.connected()) 
  {
    client.connect(server, 80);
    Serial.println("connecting...");
  }
  else
  {
    Serial.println("connection failed");
  }
}


void refillFuel()
{
  int quantity_n = quantity.toInt();

  pumpOn();
  delay(quantity_n*1000);
  pumpOff();

  input_quantity = false; //inidicates entered inputs from keypad is fuel quantity
  password_entered = false; //inidicates user finished his password input
  amount_of_fuel_entered = false; //checks if user puted the amount of fuel or not
  out_of_fuel = false; //indicates tank is out of fuel or not
  rfid_scanned = false; //checks if rfid is scanned or not
  password_fetch = false; //checks if password is fetched from server or not
  password_match = false; //checks if password is matched or not
  data_sent = false;
  fuel_refilled = false;
  get_authorized = false;
  send_statement = false;
  valid_order = false;
}
