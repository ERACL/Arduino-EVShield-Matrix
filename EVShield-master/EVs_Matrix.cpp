#include "EVs_Matrix.h"
#include "Wire.h"
#include "EVShieldI2C.h"
#include <math.h>


EVs_Matrix::EVs_Matrix(uint8_t i2c_address)
: EVShieldI2C(i2c_address)
{
}

void EVs_Matrix::get_batterie_state()
{
  int etat=readByte(SH_ETAT);
  if (etat==0) Serial.println("Système fonctionnelle");
  else Serial.println("Problème batterie, la mettre à charger");
}

int EVs_Matrix::get_batterie_level()
{
  return readByte(SH_BATTERIE);
}

long EVs_Matrix::get_moteur_position(SH_Matrix_Motor moteur)
{
  long mot = readLong(moteur);
  byte liste[8];
  for (int i=4;i<8;i++){
    liste[i]=mot;
    mot=mot/256;
  }
  for (int i=0;i<4;i++){
    liste[2*i]=liste[i+4]/16;
    liste[(2*i)+1]=liste[i+4]-liste[2*i]*16;
  }
  mot=0;
  for (int i=0;i<8;i++){
    mot=mot*16;
    mot=mot+liste[i];
  }
  return mot;
}

void EVs_Matrix::set_moteur_target(SH_Matrix_Motor moteur, long position)
{  
  long mot = position;
  byte liste[8];
  for (int i=4;i<8;i++){
    liste[i]=mot;
    mot=mot/256;
  }
  for (int i=0;i<4;i++){
    liste[2*i]=liste[i+4]/16;
    liste[(2*i)+1]=liste[i+4]-liste[2*i]*16;
  }
  mot=0;
  for (int i=0;i<8;i++){
    mot=mot*16;
    mot=mot+liste[i];
  }
  writeLong(moteur+4, mot);
}

long EVs_Matrix::get_moteur_target(SH_Matrix_Motor moteur)
{
  long mot = readLong(moteur+4);
  byte liste[8];
  for (int i=4;i<8;i++){
    liste[i]=mot;
    mot=mot/256;
  }
  for (int i=0;i<4;i++){
    liste[2*i]=liste[i+4]/16;
    liste[(2*i)+1]=liste[i+4]-liste[2*i]*16;
  }
  mot=0;
  for (int i=0;i<8;i++){
    mot=mot*16;
    mot=mot+liste[i];
  }
  return mot;
}

void EVs_Matrix::set_moteur_speed(SH_Matrix_Motor moteur, int vitesse)
{
  int decalage = 8;
  if (vitesse <= 100 and vitesse >= -100)  writeByte(moteur+decalage,vitesse);
  else  Serial.println("La vitesse doit être comprise entre 0 et 100");
}

int EVs_Matrix::get_moteur_speed(SH_Matrix_Motor moteur)
{
  return readByte(moteur+8);
}


//0 = M0, 1=M1, 2=M2, 3=M3, 4=reset, ...
void EVs_Matrix::set_moteur_mode(SH_Matrix_Motor moteur, int mode)
{
  writeByte(moteur+9,mode);
  char bit[8];
  for (int i=0 ; i<8 ; i++){
    if (mode%2==1){
      bit[i]=1;
      mode=(mode-1)/2;
    }  
    else{
    bit[i]=0;
    mode=mode/2;
    }
  } 
    if (bit[2]==1){
    Serial.print("reset du moteur :");
    Serial.println(moteur);
    }
}

void EVs_Matrix::get_moteur_mode(SH_Matrix_Motor moteur)
{
  int mode = readByte(moteur+9);
  char bit[8];
  for (int i=0 ; i<8 ; i++){
    if (mode%2==1){
      bit[i]=1;
      mode=(mode-1)/2;
    }  
    else{
    bit[i]=0;
    mode=mode/2;
    }                  
  }
  if (bit[7]==1)  Serial.println("position et target différents");
  else            Serial.println("position = target");
  if (bit[4]==1)  Serial.println("marche arrière");
  else            Serial.println("marche avant");
  if (bit[3]==1)  Serial.println("compteur en cours en attente du strat flag à 1");
  else            Serial.println("pas de compteur : start flag = 1");
  if (bit[1]==0){
    if (bit[0]==0)  Serial.println("mode 0 : à l'arrêt le moteur est relaché");
    else            Serial.println("mode 1 : à l'arrêt le moteur est bloqué");
  }
  else  {
    if (bit[0]==0)  Serial.println("mode 2 : simplement contré par la vitesse");
    else            Serial.println("mode 3 : la position du moteur va jusqu'au targer à la vitesse définie dans le registre vitesse") ;
  }
}


