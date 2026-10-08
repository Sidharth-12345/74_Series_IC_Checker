// SETUP FOR LCD
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(0);
/// SETUP FOR NAND, AND, OR, XOR GATES
int outputA[4]={2,5,9,12};
int outputB[4]={3,6,8,11};
int inputY[4]={4,7,10,13};
int defect=0;
// pins for NOT GATE
int notinput[6]={2,4,6,8,10,12};
int notoutput[6]={3,5,7,9,11,13};
// pins for NOR Gate
int norinputA[4]={3,6,9,12};
int norinputB[4]={4,7,10,13};
int noroutputY[4]={2,5,8,11};
// setup for insert ic pushbutton
int insert_button=A2;
int test_button=A3;
String ic_codes[]= {"7400","7402","7404","7408","7432","7486"};
int total_ic=6;
int ic_index=0;
bool lastnextstate=HIGH;
bool lastselectstate=HIGH;
bool ic_shown= false;
// LED CONNECTION
int red_led=A0;
int green_led=A1;
void defect_output() {
  digitalWrite(red_led,HIGH);
  digitalWrite(green_led,LOW);
  delay(300);
  digitalWrite(red_led,LOW);
}
void okay_output(){
  digitalWrite(red_led,LOW);
  digitalWrite(green_led,HIGH);
  delay(300);
  digitalWrite(green_led,LOW);
}

// gate test functions
bool andgatetest(int A, int B, int Y) {
  int andtable[4][2]={{0,0},{0,1},{1,0},{1,1}};
  int andcheck[4]={0,0,0,1};
  for(int i=0; i<4; i++) {
    digitalWrite(A,andtable[i][0]);
    digitalWrite(B,andtable[i][1]);
    delay(100);
    
    if(digitalRead(Y)!=andcheck[i]){
      return false;
    }
  }
  return true;
}

bool orgatetest(int A, int B, int Y) {
  int ortable[4][2]={{0,0},{0,1},{1,0},{1,1}};
  int orcheck[4]={0,1,1,1};
  for(int i=0; i<4; i++) {
    digitalWrite(A,ortable[i][0]);
    digitalWrite(B,ortable[i][1]);
    delay(100);
    
    if(digitalRead(Y)!= orcheck[i]){
      return false;
    }
  }
  return true;
}

bool notgatetest(int g){
  for(int i=0;i<6;i++){
    pinMode(notinput[i],INPUT);
  }
    pinMode(notinput[g],OUTPUT);
    pinMode(notoutput[g],INPUT);
    digitalWrite(notinput[g],LOW);
  	delay(50);

  	int y1=digitalRead(notoutput[g]);

  	digitalWrite(notinput[g],HIGH);
  	delay(50);

  	int y2=digitalRead(notoutput[g]);

    if(y1==1 && y2==0)
    	return true;
  	else
    	return false;
}

bool nandgatetest(int A, int B, int Y) {
  int nandtable[4][2]={{0,0},{0,1},{1,0},{1,1}};
  int nandcheck[4]={1,1,1,0};
  for(int i=0; i<4; i++){
    digitalWrite(A,nandtable[i][0]);
    digitalWrite(B,nandtable[i][1]);
    if(digitalRead(Y)!=nandcheck[i]){
      return false;
    }
  }
  return true;
}

bool norgatetest(int A, int B, int Y) {
  int nortable[4][2]={{0,0},{0,1},{1,0},{1,1}};
  int norcheck[4]={1,0,0,0};
  for(int i=0; i<4; i++){
    digitalWrite(A,nortable[i][0]);
    digitalWrite(B,nortable[i][1]);
    if(digitalRead(Y)!=norcheck[i]){
      return false;
    }
  }
  return true;
}

bool xorgatetest(int A, int B, int Y) {
  int xortable[4][2]={{0,0},{0,1},{1,0},{1,1}};
  int xorcheck[4]={0,1,1,0};
  for(int i=0; i<4; i++){
    digitalWrite(A,xortable[i][0]);
    digitalWrite(B,xortable[i][1]);
    if(digitalRead(Y)!=xorcheck[i]){
      return false;
    }
  }
  return true;
}

void gate_prints(int gate_val) {
  Serial.print("GATE ");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GATE ");
  Serial.print(gate_val);
  //lcd.setCursor(0,4);
  lcd.print(gate_val);
  Serial.print(" ");
  Serial.println("LOADING....");
  lcd.setCursor(0,7);
  lcd.print("LOADING...");
  delay(50);
  lcd.clear();
  Serial.println("TESTING....");
  lcd.setCursor(0,0);
  lcd.print("TESTING....");
  delay(100);
}

void testing_result(int defect_gates){
  if(defect_gates==0){
    Serial.println("IC OKAY!!!!");
    lcd.clear();
    lcd.print("IC OKAY!!!!");
  }
  else{
    Serial.println("IC DEFECTED!!!");
    lcd.clear();
    lcd.print("IC DEFECTED!!!");
  }
}

void setup()
{ 
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.noCursor();
  lcd.noBlink();
  for(int i=0;i<6;i++){
    pinMode(notinput[i],INPUT);
    pinMode(notoutput[i],INPUT);
  }
  pinMode(insert_button,INPUT_PULLUP);
  pinMode(test_button,INPUT_PULLUP);
  pinMode(red_led,OUTPUT);
  pinMode(green_led,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("HELLO EVERYONE");
  delay(10);
  lcd.setCursor(0,7);
  lcd.print("IC CHECKER");
  delay(50);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("BUTTON 1 > NEXT");
  lcd.setCursor(0,7);
  lcd.print("BUTTON 2 > ENTER");
  delay(500);
  lcd.clear();
}

void loop()
{
  
  int nextstate= digitalRead(insert_button);
  int selectstatebutton= digitalRead(test_button);
  if(nextstate==LOW && lastnextstate==HIGH){
    lcd.clear();
    delay(50);
    Serial.print("IC: ");
    Serial.println(ic_codes[ic_index]);
    lcd.setCursor(0,0);
    lcd.println(ic_codes[ic_index]);
    ic_index++;
    ic_shown=true;
    if(ic_index>=total_ic) ic_index=0;
  }
  if(selectstatebutton==LOW && lastselectstate==HIGH){
    delay(50);
    if(ic_shown==true){
      int selected_index= ic_index-1;
      if(selected_index<0){
      selected_index=total_ic-1;
    }
      Serial.print("YOU SELECTED: ");
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("IC SELECTED ");
      Serial.println(ic_codes[selected_index]);
      
      // NAND GATE CHECK COMMAND
      if(ic_codes[selected_index]=="7400"){
        for(int gate=1; gate<5; gate++){
          gate_prints(gate);
          if(nandgatetest(outputA[gate-1],outputB[gate-1],inputY[gate-1])){
      	    okay_output();
            Serial.println("GATE OKAY!");
            lcd.clear();
            lcd.print("GATE OKAY!");
            okay_output();
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            lcd.clear();
            lcd.print("GATE DEFECTED");
            defect_output();
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
      
      // NOR GATE CHECK COMMAND
      else if(ic_codes[selected_index]=="7402"){
        for(int gate=1; gate<5; gate++){
          gate_prints(gate);
          if(norgatetest(norinputA[gate-1],norinputB[gate-1],noroutputY[gate-1])){
      	    okay_output();
            Serial.println("GATE OKAY!");
            lcd.clear();
            lcd.print("GATE OKAY!");
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            lcd.clear();
            lcd.print("GATE DEFECTED");
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
   
      // NOT GATE CHECK COMMAND
      else if(ic_codes[selected_index]=="7404"){
        for(int gate=0; gate<6; gate++){
          gate_prints(gate+1);
          if(notgatetest(gate)){
            okay_output();
      	    Serial.println("GATE OKAY!");
            lcd.clear();
            lcd.print("GATE OKAY!");
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            lcd.clear();
            lcd.print("GATE DEFECTED");
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
      
      // AND GATE CHECK COMMAND
      else if(ic_codes[selected_index]=="7408"){
        for(int gate=1; gate<5; gate++){
          gate_prints(gate);
          if(andgatetest(outputA[gate-1],outputB[gate-1],inputY[gate-1])){
      	    okay_output();
            Serial.println("GATE OKAY!");
            lcd.clear();
            lcd.print("GATE OKAY!");
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            lcd.clear();
            lcd.print("GATE DEFECTED");
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
      
      // OR GATE CHECK COMMAND
      else if(ic_codes[selected_index]=="7432"){
        for(int gate=1; gate<5; gate++){
          gate_prints(gate);
          if(orgatetest(outputA[gate-1],outputB[gate-1],inputY[gate-1])){
            okay_output();
      	    Serial.println("GATE OKAY!");
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
      
      // XOR GATE CHECK COMMAND
      else if(ic_codes[selected_index]=="7486"){
        for(int gate=1; gate<5; gate++){
          gate_prints(gate);
          if(xorgatetest(outputA[gate-1],outputB[gate-1],inputY[gate-1])){
      	    okay_output();
            Serial.println("GATE OKAY!");
            lcd.clear();
            lcd.print("GATE OKAY!");
            delay(200);
    	  }
          else{
            defect++;
            defect_output();
            Serial.println("GATE DEFECTED");
            lcd.clear();
            lcd.print("GATE DEFECTED");
            delay(200);
          }
        }
        testing_result(defect);
        defect=0;
      }
    }
    
    delay(50);
 }
 lastnextstate=nextstate;
 lastselectstate=selectstatebutton;
}

// ------------ END OF CODE -------------------//
  