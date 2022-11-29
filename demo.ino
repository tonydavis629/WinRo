

//encoder

int val;
int enc_L_PinA = 2; //brown
int enc_R_PinA = 3;
int enc_L_PinB = 30; //grey
int enc_R_PinB = 31;
int enc_L_Pos = 0;
int enc_R_Pos = 0;
int enc_L_PinA_Last = LOW;
int enc_R_PinA_Last = LOW;
int n_L = LOW;
int n_R = LOW;


//ultrasonic sensors and LIDAR

#include <Wire.h>
#include <LIDARLite.h>

int US_L_trig = 42; //white and grey
int US_R_trig = 44;
int US_L_echo = 46;
int US_R_echo = 48;
long duration_L, dist_US_L;
long duration_R, dist_US_R;

LIDARLite myLidarLite;


//motor control, servos, brush motors
#include <Servo.h>

int pwm_L = 8; //blue
int pwm_R = 9;
int inb_L = 22; //green
int ina_L = 24; //yellow
int inb_R = 26;
int ina_R = 28;
//int enb=7;
//int ena=8; always high

Servo servo_L;
Servo servo_R;

int brush_L = 4; //pwm for the cleaning brushes, blue
int brush_R = 5;

int W = 75; //horizontal space
int Li = 50; // length of the left side cable after calibration
int Ri = 50; //length of the right side cable after calibration
int x_pos = 0;
int y_pos = 0;
int L = 0;
int R = 0;

void setup() {
  Serial.begin(9600);

  //encoder setup
  pinMode(enc_L_PinA, INPUT);
  pinMode(enc_L_PinB, INPUT);


  pinMode(enc_R_PinA, INPUT);
  pinMode(enc_R_PinB, INPUT);
  //pinMode(enc_R_PinX, INPUT);

 // attachInterrupt(0, encoderL, CHANGE);
 // attachInterrupt(1, encoderR, CHANGE);


  //LIDAR setup

  myLidarLite.begin(0, true);
  myLidarLite.configure(0);

  //Ultrasonic sensor setup

  pinMode(US_L_trig, OUTPUT);
  pinMode(US_L_echo, INPUT);
  pinMode(US_R_trig, OUTPUT);
  pinMode(US_R_echo, INPUT);


  //motor control setup

  servo_L.attach(6);
  servo_R.attach(7);

  pinMode(pwm_L, OUTPUT);
  pinMode(pwm_R, OUTPUT);
  pinMode(inb_L, OUTPUT);
  pinMode(ina_L, OUTPUT);
  pinMode(inb_R, OUTPUT);
  pinMode(ina_R, OUTPUT);

  servo_L.write(180);
  servo_R.write(0);
  analogWrite(brush_L, 0);
  analogWrite(brush_R, 0);
  delay(2000);

}


void loop() {
  
  Serial.print("what");
  armsdown();
  delay(3000);
  motorson();
  delay(3000);
  motorsoff();
  delay(3000);
  armsup();
  delay(9999999);
  

/*

  //initial wipe
  armsdown();
  delay(4000);
  motorson();
  delay(4000);
  motorsoff();
  delay(4000);
  armsup();
  delay(4000);

  //move up
  while (enc_L_Pos < 20) {
    moveup();
  }
  freeze();

  delay(4000);
  armsdown();
  delay(4000);
  motorson();
  delay(4000);

  //move right
  while (enc_L_Pos > 0) {
    moveright();
  }
  freeze();
  delay(4000);
  motorsoff();

  delay(4000);
  armsup();
  delay(4000);

  //move down
  while (enc_L_Pos > -20) {
    movedown();
  }
  freeze();

  delay(4000);
  armsdown();
  delay(4000);
  motorson();
  delay(4000);

  //move left
  while (enc_L_Pos < -10 ) {
    moveleft();
  }

  freeze();
  delay(4000);
  motorsoff();
  delay(4000);
  armsup();


  delay(100000000);
  
  /*
  */

}

void moveright() {
  //counterclockwise

  US_ping();

  // while (x_pos<10){
  int L_speed = 90;
  digitalWrite(ina_L, LOW);
  digitalWrite(inb_L, HIGH);

  //arccos or inv cos?
  int ga = acos((-W ^ 2 + L ^ 2 + R ^ 2) / (2 * L * R)); //angle between L and R
  int th = acos((-R ^ 2 + W ^ 2 + L ^ 2) / (2 * L * W)); //angle between L and horizontal
  int fi = 180 - ga - th;  //angle between L2 and horizontal
  int x_pos = L * cos(th);
  int  dL_L = 2 * 3.14159 * enc_L_Pos / 48; //change in length
  int  dR_R = 2 * 3.14159 * enc_R_Pos / 48; //change in length

  int L = L + dL_L;
  int R = R + dR_R;

  int R_speed = 180; //* cos(th) / cos(fi);
  digitalWrite(ina_R, LOW);
  digitalWrite(inb_R, HIGH);

  Serial.print(enc_L_Pos);
  Serial.print("/");

  //analogWrite(pwm_R, R_speed); //change value to change speed

  if (dist_US_L > 10) {
    analogWrite(pwm_L, L_speed); //change value to change speed
    analogWrite(pwm_R, R_speed);
  } else {
    analogWrite(pwm_L, 0);
    analogWrite(pwm_R, 0);
  }
}
void moveleft() {

  US_ping();

  int L_speed = 160;
  digitalWrite(ina_L, HIGH);
  digitalWrite(inb_L, LOW);

  //arccos or inv cos?
  int ga = acos((-W ^ 2 + L ^ 2 + R ^ 2) / (2 * L * R)); //angle between L1 and L2, L1 is left wire
  int th = acos((-R ^ 2 + W ^ 2 + L ^ 2) / (2 * L * W)); //angle between L1 and horizontal
  int fi = 180 - ga - th;  //angle between L2 and horizontal
  int x_pos = L * cos(th);
  int  dL_L = 2 * 3.14159 * enc_L_Pos / 48; //change in length
  int  dR_R = 2 * 3.14159 * enc_R_Pos / 48; //change in length

  int L = L + dL_L;
  int R = R + dR_R;

  int R_speed = 80; //* cos(th) / cos(fi);
  digitalWrite(ina_R, HIGH);
  digitalWrite(inb_R, LOW);

  Serial.print(enc_L_Pos);
  Serial.print("/");

  if (dist_US_L > 10) {
    analogWrite(pwm_L, L_speed); //change value to change speed
    analogWrite(pwm_R, R_speed);
  } else {
    analogWrite(pwm_L, 0);
    analogWrite(pwm_R, 0);
  }
}



void moveup() {
  //counterclockwise

  US_ping();

  int L_speed = 140;
  digitalWrite(ina_L, HIGH);
  digitalWrite(inb_L, LOW);

  /*
    int ga = acos((-W ^ 2 + L ^ 2 + R ^ 2) / (2 * L * R)); //angle between L1 and L2, L1 is left wire
    int th = acos((-R ^ 2 + W ^ 2 + L ^ 2) / (2 * L * W)); //angle between L1 and horizontal
    int fi = 180 - ga - th;  //angle between L2 and horizontal
    int x_pos = L * cos(th);
    int y_pos = L * sin(th);
  */

  int y_pos = myLidarLite.distance();

  int  dL_L = 2 * 3.14159 * enc_L_Pos / 48; //change in length
  int  dR_R = 2 * 3.14159 * enc_R_Pos / 48; //change in length

  Serial.print(enc_L_Pos);
  Serial.print("/");


  int L = Li + dL_L;
  int R = Ri + dR_R;


  //int R_speed = L_speed * cos(th) / cos(fi);
  int R_speed = 140;

  digitalWrite(ina_R, LOW);
  digitalWrite(inb_R, HIGH);
  analogWrite(pwm_R, R_speed); //change value to change speed


  if (dist_US_L > 10) {
    analogWrite(pwm_L, L_speed); //change value to change speed
    analogWrite(pwm_R, R_speed);
  } else {
    analogWrite(pwm_L, 0);
    analogWrite(pwm_R, 0);
  }
}

void movedown() {
  //counterclockwise

  US_ping();

  int L_speed = 40;
  digitalWrite(ina_L, LOW);
  digitalWrite(inb_L, HIGH);

  /*
    int ga = acos((-W ^ 2 + L ^ 2 + R ^ 2) / (2 * L * R)); //angle between L1 and L2, L1 is left wire
    int th = acos((-R ^ 2 + W ^ 2 + L ^ 2) / (2 * L * W)); //angle between L1 and horizontal
    int fi = 180 - ga - th;  //angle between L2 and horizontal
    int x_pos = L * cos(th);
  */

  int  dL_L = 2 * 3.14159 * enc_L_Pos / 48; //change in length
  int  dR_R = 2 * 3.14159 * enc_R_Pos / 48; //change in length

  int L = Li + dL_L;
  int R = Ri + dR_R;

  //y_pos = myLidarLite.distance();

  int R_speed = 40; // * cos(th) / cos(fi);
  digitalWrite(ina_R, HIGH);
  digitalWrite(inb_R, LOW);

  Serial.print(enc_L_Pos);
  Serial.print("/");


  if (dist_US_L > 10) {
    analogWrite(pwm_L, L_speed); //change value to change speed
    analogWrite(pwm_R, R_speed);
  } else {
    analogWrite(pwm_L, 0);
    analogWrite(pwm_R, 0);
  }
}


void US_ping() {
  pinMode(US_L_trig, OUTPUT);
  digitalWrite(US_L_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(US_L_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_L_trig, LOW);

  pinMode(US_L_echo, INPUT);
  duration_L = pulseIn(US_L_echo, HIGH);
  dist_US_L = duration_L / (2 * 29.1);

  pinMode(US_R_trig, OUTPUT);
  digitalWrite(US_R_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(US_R_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_R_trig, LOW);

  pinMode(US_R_echo, INPUT);
  duration_R = pulseIn(US_R_echo, HIGH);
  dist_US_R = duration_R / (2 * 29.1);

}

void encoderL() {
  n_L = digitalRead(enc_L_PinA);
  if ((enc_L_PinA_Last == LOW) && (n_L == HIGH)) {
    if (digitalRead(enc_L_PinB) == LOW) {
      enc_L_Pos--;
    } else {
      enc_L_Pos++;
    }

  }
  enc_L_PinA_Last = n_L;

}

void encoderR() {
  n_R = digitalRead(enc_R_PinA);
  if ((enc_R_PinA_Last == LOW) && (n_R == HIGH)) {
    if (digitalRead(enc_R_PinB) == LOW) {
      enc_R_Pos--;
    } else {
      enc_R_Pos++;
    }

  }
  enc_R_PinA_Last = n_R;

}

void freeze() {
  analogWrite(pwm_L, 0);
  analogWrite(pwm_R, 0);
  analogWrite(brush_L, 0);
  analogWrite(brush_R, 0);
}

void armsup() {

  int angle_L = 0; int angle_R = 180; int num = 0;
  while (angle_L < 180) {
    angle_L = angle_L + 1;
    angle_R = angle_R - 1;
    servo_L.write (angle_L);
    servo_R.write (angle_R);
    delay (30);
    Serial.print(angle_L);
    Serial.print("/");
  }
}


void armsdown() {

  int angle_L = 180; int angle_R = 0;
  for (angle_L > 0 && angle_R < 180; angle_L -= 1; angle_R += 1) {
    servo_L.write (angle_L);
    servo_R.write (angle_R);
    delay (30);
  }
}

void motorson() {
  analogWrite(brush_L, 250);
  analogWrite(brush_R, 250);
}

void motorsoff() {
  analogWrite(brush_L, 0);
  analogWrite(brush_R, 0);
}


