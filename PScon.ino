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
int val; //サーボの角度を格納するための変数

void setup(){
  pinMode(7,OUTPUT); // モータ1信号用ピン
  pinMode(8,OUTPUT); // モータ1信号用ピン

  pinMode(2,OUTPUT); // モータ2信号用ピン
  pinMode(4,OUTPUT); // モータ2信号用ピン

  pinMode(14,OUTPUT); // モータ3信号用ピン
  pinMode(15,OUTPUT); // モータ3信号用ピン

  myservo1.attach(5);// サーボ1信号用ピン
  myservo2.attach(6); // サーボ2信号用ピン
  val = 90;

  delay(2000); //起動直後にコントローラ触ると何故か操作受け付けなくなるからちょっと待つ

  Serial.begin(57600);

  //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  error = ps2x.config_gamepad(13,11,10,12, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

  //Serial.print(ps2x.Analog(1), HEX); 
  type = ps2x.readType(); 
  switch(type) {
  case 0:
    Serial.println("Unknown Controller type");
    break;
  case 1:
    Serial.println("DualShock Controller Found");
    break;
  case 2:
    Serial.println("GuitarHero Controller Found");
    break;
  }

}

void loop(){
  myservo1.write(speed);

  if(error == 1) //skip loop if no controller found
    return; 

  //DualShock Controller
  ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed

  if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
    Serial.println("Start is being held");
  if(ps2x.Button(PSB_SELECT))
    Serial.println("Select is being held");

  if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
    Serial.print("Up held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);

    digitalWrite(14,HIGH);
    digitalWrite(15,LOW);  
    delay(50);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);   
  }


  if(ps2x.Button(PSB_PAD_RIGHT)){
    Serial.print("Right held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
  }

  if(ps2x.Button(PSB_PAD_LEFT)){
    Serial.print("LEFT held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
  }

  if(ps2x.Button(PSB_PAD_DOWN)){
    Serial.print("DOWN held this hard: ");
    Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);

    digitalWrite(14,LOW);
    digitalWrite(15,HIGH);
    delay(50);
    digitalWrite(14,LOW);
    digitalWrite(15,LOW);
  }   

  vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
  //how hard you press the blue (X) button    

  if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
  {
    if(ps2x.Button(PSB_L3))
      Serial.println("L3 pressed");
    if(ps2x.Button(PSB_R3))
      Serial.println("R3 pressed");
    if(ps2x.Button(PSB_L2))
      Serial.println("L2 pressed");
    if(ps2x.Button(PSB_R2))
      Serial.println("R2 pressed");
    if(ps2x.Button(PSB_GREEN))
      Serial.println("Triangle pressed");
  }   

  if(ps2x.ButtonPressed(PSB_RED))             //will be TRUE if button was JUST pressed
    Serial.println("Circle just pressed");
  if(ps2x.ButtonReleased(PSB_PINK))             //will be TRUE if button was JUST released
    Serial.println("Square just released");
  if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
    Serial.println("X just changed");


  if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
  {
    Serial.print("Stick Values:");
    Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_LX), DEC); 
    Serial.print(",");
    Serial.print(ps2x.Analog(PSS_RY), DEC); 
    Serial.print(",");
    Serial.println(ps2x.Analog(PSS_RX), DEC); 
  }

  if(ps2x.Analog(PSS_LY)>245)
  {
    digitalWrite(7,HIGH);
    digitalWrite(8,LOW);
    analogWrite(9,255);
  }
  if(ps2x.Analog(PSS_LY)<245 && ps2x.Analog(PSS_LY)>10)
  {
    digitalWrite(7,LOW);
    digitalWrite(8,LOW);
  }
  if(ps2x.Analog(PSS_LY)<10)
  {
    digitalWrite(7,LOW);
    digitalWrite(8,HIGH);
    analogWrite(9,255);
  }

  if(ps2x.Analog(PSS_RY)>245)
  {
    digitalWrite(2,HIGH);
    digitalWrite(4,LOW);
    analogWrite(3,255);    
  }
  if(ps2x.Analog(PSS_RY)<245 && ps2x.Analog(PSS_RY)>10)
  {
    digitalWrite(2,LOW);
    digitalWrite(4,LOW);
  }
  if(ps2x.Analog(PSS_RY)<10)
  {
    digitalWrite(2,LOW);
    digitalWrite(4,HIGH);
    analogWrite(3,255); 
  }

  if(ps2x.Button(PSB_L1) && val <= 180)
  {
    val = val + 2 ;
    myservo1.write(val);
    myservo2.write(180 - val);
    delay(5);
  }

  if(ps2x.Button(PSB_R1) && val >= 0)
  {
    val = val - 2 ;
    myservo1.write(val);
    myservo2.write(180 - val);
    delay(5);
  }

  delay(10);

}
