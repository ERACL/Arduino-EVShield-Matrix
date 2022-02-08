#include "EVs_Matrix.h"
#include "Wire.h"
#include "EVShieldI2C.h"
#include <math.h>


EVs_Matrix::EVs_Matrix(uint8_t i2c_address)
: EVShieldI2C(i2c_address)
{
}

int EVs_Matrix::get_battery_state()
{
  return(readByte(SH_ETAT)/2);
}

int EVs_Matrix::get_battery_level()
{
  return readByte(SH_BATTERIE)*40;
}

long EVs_Matrix::get_motor_position(SH_Matrix_Motor motor)
{
  long mot = readLong(motor);
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

void EVs_Matrix::set_timeout(int time)
{
  writeByte(SH_TIMEOUT,time);
}

void EVs_Matrix::start_flag()
{
  writeByte(STRT_FL,1);
}

void EVs_Matrix::set_motor_target(SH_Matrix_Motor motor, long position)
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
  writeLong(motor+4, mot);
}

long EVs_Matrix::get_motor_target(SH_Matrix_Motor motor)
{
  long mot = readLong(motor+4);
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

void EVs_Matrix::set_motor_speed(SH_Matrix_Motor motor, int vitesse)
{
  if (vitesse <= 100 and vitesse >= -100)  writeByte(motor+8,vitesse);
  else  vitesse = 0;
}

int EVs_Matrix::get_motor_speed(SH_Matrix_Motor motor)
{
  return readByte(motor+8);
}


//0 = M0, 1=M1, 2=M2, 3=M3, 4=reset, ...
void EVs_Matrix::set_motor_mode(SH_Matrix_Motor motor, int mode)
{
  writeByte(motor+9,mode);
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
    Serial.println("reset motor :");
    }
}

int EVs_Matrix::get_motor_mode(SH_Matrix_Motor motor)
{
  return(readByte(motor+9));
}

bool EVs_Matrix::is_motor_busy(SH_Matrix_Motor motor)
{
  int mode = readByte(motor+9);
  return(mode/128);
}
