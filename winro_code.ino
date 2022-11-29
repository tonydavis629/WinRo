#include <servo.h>

Servo leftservo;
Servo rightservo;
Servo leftdust;
Servo rightdust;


void setup() {
Serial.begin(9600);
const int Lchainsenspin=A0 //calibration sensors
const int Rchainsenspin=A1
const int Ldist=A2 //ultrasonic sensors
const int Rdist=A3
const int Lenc=5 //encoders
const int Renc=6

pinMode(Lchainsenspin, INPUT)
pinMode(Rchainsenspin, INPUT)
pinMode(Ldist, INPUT)
pinMode(Rdist, INPUT)
pinMode(Lenc, INPUT)
pinMode(Renc, INPUT)

leftservo.attach(1)
rightservo.attach(2)
leftdust.attach(3)
rightdust.attach(4)

float counts_full= //number of counts per revolution on the encoders

float W= //width between supports

float leftwall_1=0 //x=0 position in first (left) section
float rightwall_l = W//right side of first section 
float leftwall_2= //x_pos of left side of 2nd (right) section
float rightwall_2=leftwall_2+W
float vertical = //vertical distance between beams

float L1= // length of the left side cable after calibration
float L2= //length of the right side cable after calibration

float ga= acos((w^2-L1^2-L2^2)/(2*L1*L2)); //angle between L1 and L2, L2 is left wire
float fi= acos((L2^2-L1^2-W^2/(2*L1*W));   //angle between L1 and horizontal
float th=180-ga-fi;                        //angle between L2 and horizontal
float x_pos=L1*cos(fi);
if x_pos~=W~L1*cos(th)
printf('Potential error in cable length calibration')
end

float y_pos=H-L2*sin(th)
if y_pos~=H-L1*sin(fi)
printf('Potential error in cable length calibration')
end

}


void loop() {
  //calibration procedure
while analogRead(Lchainsenspin)~=1 || analogRead(Rchainsenspin)~=1
leftservo.Write(180)
rightservo.Write(0) //moves up both belts at the same rate
end
leftservo.Write(90)
rightservo.Write(90)
  //end calibration procedure

//move to first position
movetostart();

//movement routine

//run horizontal 
moveright();

//move down
movedown();

moveleft();

movedown();

moveright();

movedown();

moveleft();

//go to top left of right section
movetostart2();

moveright();

movedown();

moveleft();

movedown();

moveright();

movedown();

moveleft();

reset();

}

void movetostart(){
  while x_pos > leftwall
    leftservo.Write(0) //move left
    rightservo.Write(0) 
    counts_L=digitalRead(Lenc)
    counts_R=digitalRead(Renc)
    
    L1=L1+(2*counts_L)/counts_full; //calculate new lengths of wire L1 and L2
    L2=L2+(2*counts_R)/counts_full;

    float ga= acos((w^2-L1^2-L2^2)/(2*L1*L2)); //angle between L1 and L2
    float fi= acos((L2^2-L1^2-W^2/(2*L1*W));   //angle between L1 and horizontal
    float th=180-ga-fi;   //L2 and horizontal
    float x_pos=L2*cos(th);
    
    if analogRead(Ldist) < 1 //safe distance
    break
    end
  end

  while y_pos > 0
    leftservo.Write(180) //move up
    rightservo.Write(0) 
  end
    
  

}

void moveright(){

  while x_pos < rightwall

  end
  
}

void moveleft(){

}

void movedown(){
}

void movetostart2(){
}

void reset(){
}


