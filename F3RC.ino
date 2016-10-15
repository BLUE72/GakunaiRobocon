#include <PS2X_lib.h>  //for v1.6
#include <Servo.h>

#define PS2X_DEBUG
#define PS2X_COM_DEBUG

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;

Servo myservo1; //サーボ1用のオブジェクトを作成
Servo myservo2; //サーボ2用のオブジェクトを作成
int val1; //サーボの角度を格納するための変数
int val2;

int speedM1;
int speedM2;
int Lstate;
int Rstate;
int Kinkyu = 0;

void setup(){
  pinMode(7,OUTPUT); // モータ1信号用ピン
  pinMode(8,OUTPUT); // モータ1信号用ピン
  pinMode(11,OUTPUT);

  pinMode(2,OUTPUT); // モータ2信号用ピン
  pinMode(4,OUTPUT); // モータ2信号用ピン
  pinMode(3,OUTPUT);

  pinMode(14,OUTPUT); // モータ3信号用ピン
  pinMode(15,OUTPUT); // モータ3信号用ピン

  pinMode(16,INPUT_PULLUP); //swichU
  pinMode(17,INPUT_PULLUP); //swichT
  pinMode(18,INPUT_PULLUP); //緊急停止

  myservo1.attach(5);// サーボ1信号用ピン
  myservo2.attach(6); // サーボ2信号用ピン
  val1 = 126;
  val2 = 55;
  myservo1.write(val1);
  myservo2.write(val2);

  delay(1000); //起動直後にコントローラ触ると何故か操作受け付けなくなるからちょっと待つ

  Serial.begin(57600);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  error = ps2x.config_gamepad(13,9,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  //Serial.print(ps2x.Analog(1), HEX); 
  type = ps2x.readType(); 
  
}

void loop(){

  if(error == 1) //skip loop if no controller found
    return; 

  //DualShock Controller
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  /*if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
  if(ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");*/

  if(ps2x.Button(PSB_PAD_UP) && digitalRead(17) == HIGH) {         //will be TRUE as long as button is pressed
    /*Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);*/

    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);  
    delay(15);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);   
  }

  if(ps2x.Button(PSB_PAD_DOWN) && digitalRead(16) == HIGH){
    /*Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);*/

    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);
    delay(10);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }   

  vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 


  if(ps2x.Analog(PSS_LY) > 128)
  {
    Lstate = ps2x.Analog(PSS_LY);
    speedM1 = (Lstate-128)*255/127;
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    analogWrite(11,speedM1);
  }
  if(ps2x.Analog(PSS_LY) == 128)
  {
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
  }
  if(ps2x.Analog(PSS_LY) < 128)
  {
    Lstate = ps2x.Analog(PSS_LY);
    speedM1=(127-Lstate)*255/127;
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);
    analogWrite(11,speedM1);
  }

  if(ps2x.Analog(PSS_RY) > 128)
  {
    Rstate = ps2x.Analog(PSS_RY);
    speedM2 = (Rstate-128)*255/127;
    digitalWrite(2,HIGH);
    digitalWrite(4,LOW);
    analogWrite(3,speedM2); 
  }
  if(ps2x.Analog(PSS_RY) == 128)
  {
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
  }
  if(ps2x.Analog(PSS_RY) < 128)
  {
    Rstate = ps2x.Analog(PSS_RY);
    speedM2=(127 - Rstate)*255/127;
    digitalWrite(2,LOW);
    digitalWrite(4,HIGH);
    analogWrite(3,speedM2);
  }

  if(ps2x.Button(PSB_L1) )
  {
    val1 = val1 + 1 ;
    val2 = val2 - 1 ;
    
    if(val1 > 171)
      val1 = 171;
    if(val2 < 20) 
      val2 = 20;
      
    myservo1.write(val1);
    myservo2.write(val2);
  }

  if(ps2x.Button(PSB_R1) )
  {
    val1 = val1 - 1 ;
    val2 = val2 + 1 ;
    
    if(val1 < 77)
      val1 = 77;
    if(val2 > 111) 
      val2 = 111;
      
    myservo1.write(val1);
    myservo2.write(val2);
  }

  if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
  {
    val1 = 80 ;
    val2 = 108 ;
    myservo1.write(val1);
    myservo2.write(val2);
  }
  
  if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
  {
    val1 = 171 ;
    val2 = 20 ;
    myservo1.write(val1);
    myservo2.write(val2);
  }

  /*
  Serial.print(speedM1);
  Serial.print(",");
  Serial.print(speedM2);
  Serial.print(",");
  Serial.print(Lstate);
  Serial.print(",");
  Serial.print(Rstate);
  Serial.print("\n");
  */
  delay(10);

}
