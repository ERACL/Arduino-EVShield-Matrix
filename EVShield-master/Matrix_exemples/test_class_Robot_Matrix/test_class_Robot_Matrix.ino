//references (n° ligne): 4, 5, 7, 8, 9, 19, 21, 29, 30

//================================================
#include <EVShield.h>                           // bibliotheque controlant l'EVShield (cf. Wiki - Communication Arduino-EVShield-Matrix)
#include <Robot_Matrix.h>                       // bibliotheque controlant le robot (cf. Wiki - Descriptif de la bibliotheque Robot_Matrix)

#define MC_ADDR 0x10                            // adresse I2C du controleur Matrix (cf. Wiki - Communication Arduino-EVShield-Matrix)
#define BANKA_ADDR 0x34                         // adresse I2C de la BankA de l'EVShield (cf. Wiki - Communication Arduino-EVShield-Matrix)
#define BANKB_ADDR 0x36                         // adresse I2C de la BankB de l'EVShield (cf. Wiki - Communication Arduino-EVShield-Matrix)

EVShield      evshield(BANKA_ADDR,BANKB_ADDR);  // instanciation, permettant de parametrer l'EVShield (en renseignant les adresses de ses banques A et B)
Robot_Matrix  robot(MC_ADDR);                   // instanciation, permettant de controler le robot via le controleur Matrix (en renseignant son adresse)


//================================================
void setup() {

  // initialisations
    evshield.init(SH_HardwareI2C);              // initialisation de l'EVShield (sans quoi il ne transmet aucune information)(cf. Wiki - Communication Arduino-EVShield-Matrix)
    robot.init(&evshield, SH_BAS2);             // initialisation de l'objet 'robot' pour qu'il s'adresse au controleur Matrix, branche sur le port SH_BAS2 de l'EVShield
    robot.config(                               // configuration et parametrage du robot (cf. Wiki - Descriptif de la bibliotheque Robot_Matrix) 
      SH_MM1,         // moteur gauche
      SH_MM3,         // moteur droit
      true,           // moteur gauche sens inverse ?
      false,          // moteur droit sens inverse ?
      2750,           // increments par metre
      215             // voie (= distance entre les 2 roues motrices)
    ); 
    robot.set_motor_mode(SH_MM1,0b00000100);    // reset du moteur gauche (cf. Wiki - Documentation du controleur Matrix)
    robot.set_motor_mode(SH_MM3,0b00000100);    // reset du moteur droit (cf. Wiki - Documentation du controleur Matrix)
  
  // initialisation du moniteur serie
    Serial.begin(115200);                       // initialisation du moniteur serie a la frequence 115200 bauds
    while (!Serial);                            // attente de fonctionnement du moniteur serie
    Serial.println("\n> Serial started");       // confirmation du fonctionnement 

  // ecriture des informations sur la batterie
    int a; int b;
    a = robot.get_battery_state();              // obtention de l'etat de la batterie
    b = robot.get_battery_level();              // obtention du niveau de batterie en mV (il faut ~1V)
    Serial.print("Battery state (0:high, 1:low) : "); Serial.println(a);
    Serial.print("Battery level (mV)            : "); Serial.println(b);

  // appuyer sur le bouton 'GO' pour demarrer le programme 
    Serial.println("------------------------");
    Serial.print("Waiting for button...");
    delay(100);
    evshield.bank_b.ledSetRGB(100,100,100);     // allume la led blanche pour indiquer que le programme est pret
    delay(100);
    while(!evshield.getButtonState(BTN_GO));    // le programme attend que l'utilisateur appuie sur le bouton 'GO'
    Serial.println(" GO \n"); 
}


//================================================
void loop() {

  // réglage de la vitesse
  robot.set_speed(30);

  // tout droit en avant
  robot.go(400);
  delay(3000);

  // tour complet
  robot.turn(360);
  delay(3000);
  
  // tout droit en arrière
  robot.go(-400);
  delay(3000);

  // position relative à 45° à droite
  robot.go_to(300,-300);
  delay(3000);

  while(1);
}
