//#include <Wire.h>
#include <EVShield.h>
//#include <EVShieldI2C.h>
#include <EVs_NXTTouch.h>
//#include <EVShieldAGS.h>
#include <EVs_EV3Ultrasonic.h>
#include <EVs_Matrix.h>

#define MC_ADDR 0x10

int fil = 7;
unsigned long time0;
unsigned long time1;
unsigned long timePressed;
unsigned long dist;
bool flag = false;
int val=HIGH;
long target = 3000;
int etape = 0;
int vitesse = 0;
int k =0;


EVShield          evshield(0x34,0x36);
EVs_NXTTouch      buttonStop;
EVs_EV3Ultrasonic capteurUS;
EVs_Matrix        controller(MC_ADDR);

#define moteur SH_MM1

void setup() {
  pinMode(fil, INPUT);
  evshield.init(SH_HardwareI2C);
  controller.init(&evshield, SH_BAS2);
  buttonStop.init(&evshield,SH_BAS1);
  capteurUS.init(&evshield, SH_BBS2);
  Serial.begin(115200);
  
  capteurUS.setMode(MODE_Sonar_CM);
  
  while (!Serial);
  Serial.println("\nI2C Scanner");
  controller.get_batterie_state();
  while (digitalRead(fil)==1);
  Serial.println(" fil retirer, ces parti mon kiki");
  time0 = millis();
  controller.set_moteur_mode(SH_MM1,4);
  controller.set_moteur_mode(SH_MM3,4);
  delay(200);
  controller.set_moteur_mode(SH_MM1,19);
  controller.set_moteur_mode(SH_MM3,3);
  delay(200);
    /*controller.get_moteur_mode(SH_MM1);
    Serial.println();
    controller.get_moteur_mode(SH_MM3);
    Serial.println();
    Serial.print("target moteur 1 à : ");
    Serial.print(controller.get_moteur_target(SH_MM1));
    Serial.print("           position moteur 1 à : ");
    Serial.println(controller.get_moteur_position(SH_MM1));
    Serial.print("target moteur 3 à : ");
    Serial.print(controller.get_moteur_target(SH_MM3));
    Serial.print("           position moteur 3 à : ");
    Serial.println(controller.get_moteur_position(SH_MM3));
    Serial.println();*/
}

void loop() {

  dist = capteurUS.getDist();
  
  controller.set_moteur_speed(SH_MM1,vitesse);
  controller.set_moteur_speed(SH_MM3,vitesse);
  controller.set_moteur_target(SH_MM1,target);
  controller.set_moteur_target(SH_MM3,target);

  vitesse=40;
  
  if (millis()-time0>k*1000){
    k++;
  }

  
  if (controller.get_moteur_position(SH_MM3) == target+1){
    etape++ ;
    vitesse=0;
    //controller.set_moteur_mode(SH_MM3,3);
    //controller.set_moteur_mode(SH_MM1,19);
    Serial.println("-------------------------------------------------------------------------");
  }
  
  if (dist>20 || dist==0){
    vitesse = 40;
    controller.set_moteur_mode(SH_MM3,3);
    controller.set_moteur_mode(SH_MM1,19);    
  }
  else {
    vitesse = 0;
    controller.set_moteur_mode(SH_MM1,18);
    controller.set_moteur_mode(SH_MM3,2);
  }
  
  if ((millis()-time0>96000)&&(flag==false)){
    evshield.bank_a.motorRunDegrees(SH_Motor_1,SH_Direction_Forward,40,105,SH_Completion_Dont_Wait,SH_Next_Action_BrakeHold);
    flag = true;
  }
    
  if (millis()-time0>99000){
    while(1){}
  }

  if (!buttonStop.isPressed()){timePressed = millis();}
    if (millis()-timePressed > 100){
      Serial.println("Arrêt d'urgence !!!");
      vitesse=0;
      controller.set_moteur_speed(SH_MM1,vitesse);
      controller.set_moteur_speed(SH_MM3,vitesse);
      while(1){}
  }
  delay(10);
}
