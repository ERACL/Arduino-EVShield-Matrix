//doc matrix
//https://robopedia.kad-office.com/w/images/c/c9/MATRIX-Controller-Specification-v1.2.pdf

#define EVs_Matrix_H


#include "SHDefines.h"
#include "EVShieldI2C.h"


/*-------read register-------*/
//Servo : le bit associé est à 1 si le servo est fonctionelle (ex: servo 2 et 3 fonctionnels => 0110=6)
#define SH_TIMEOUT      0x42  

//Start flag, un compteur pour les motors
#define STRT_FL         0x44

//Servo : le bit associé est à 1 si le servo est fonctionelle (ex: servo 2 et 3 fonctionnels => 0110=6)
#define SH_S            0x45    

//servo 1
#define SH_S1_SPEED     0x46
#define SH_S1_TARGET    0x47

//servo 2
#define SH_S2_SPEED     0x48
#define SH_S2_TARGET    0x49

//servo 3
#define SH_S3_SPEED     0x4A
#define SH_S3_TARGET    0x4B

//servo 4
#define SH_S4_SPEED     0x4C
#define SH_S4_TARGET    0x4D

//motor 1     
#define SH_M1_POS       0x4E // /!\ si on ne le solicite pas pendant plus de 3s il se remet à 0
#define SH_M1_TARGET    0x52
#define SH_M1_SPEED     0x56
#define SH_M1_MODE      0x57

//motor 2
#define SH_M2_POS       0x58
#define SH_M2_TARGET    0x5C
#define SH_M2_SPEED     0x60
#define SH_M2_MODE      0x61

//motor 3
#define SH_M3_POS       0x62
#define SH_M3_TARGET    0x66
#define SH_M3_SPEED     0x6A
#define SH_M3_MODE      0x6B

//motor 4
#define SH_M4_POS       0x6C
#define SH_M4_TARGET    0x70
#define SH_M4_SPEED     0x74
#define SH_M4_MODE      0x75

//batterie (lecture du niveau)
#define SH_BATTERIE     0X43

//Block Matrix
#define SH_ETAT         0x41

// Adresse
//Block Matrix
#define SH_Block        0x10
/*--------- Variables utilisées---------*/




/*----------Groupe d'objet simplifiant l'utilisation des fonctions----------*/
//choix du motor controllé
typedef enum {
  SH_MM1    = 0x4E,
  SH_MM2    = 0x58,
  SH_MM3    = 0x62,
  SH_MM4    = 0x6C
} SH_Matrix_Motor;

//choix du motor controllé
typedef enum {
  SH_MS1    = 0x46,
  SH_MS2    = 0x48,
  SH_MS3    = 0x4A,
  SH_MS4    = 0x4C
} SH_Matrix_Servo;

//Classe Matrix avec comme parent EVSieldI2C
class EVs_Matrix : public EVShieldI2C
{
public:
  //fonctions classiques de controle des registres

    // fonction constructeur avec par defaut 0x10 pour adresse du block matrix
    EVs_Matrix(uint8_t i2c_address);

    //fontion etat de fonctionnement de la batterie
    int get_battery_state();

    //fonction qui renvoie le niveau de batterie en mV
    int get_battery_level();

    //fonction qui passe le flag à 1 / passe les panding à 0
    void set_timeout(int time);

    //fonction qui passe le flag à 1 / passe les panding à 0
    void start_flag();

    //fonction pour lire la position d'un motor
    long get_motor_position(SH_Matrix_Motor motor);

    //fonction pour lire/écrire la position visée par un motor
    void set_motor_target(SH_Matrix_Motor motor, long position);
    long get_motor_target(SH_Matrix_Motor motor);

    //fonction pour lire/écrire la vitesse d'un motor
    void set_motor_speed(SH_Matrix_Motor motor, int vitesse);
    int get_motor_speed(SH_Matrix_Motor motor);

    //fonction pour lire/écrire le mode de fonctionnement du motor
    void set_motor_mode(SH_Matrix_Motor motor, int mode);
    int get_motor_mode(SH_Matrix_Motor motor);

    //fonction qui dit si le busy est à 1
    bool is_motor_busy(SH_Matrix_Motor motor);
};