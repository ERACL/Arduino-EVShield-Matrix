/*
Ce Programme permet de faire fonctionner deux motors :
le controler matrix doit être branché sur le port BAS2 de l'EVshield et doit être en mode On,
ces motors doivent être branché : motor droite sur motor3 et motor gauche sur motor1

1) dans un premier temps, les informations du mode du motor seront donner 
  dans le moniteur série qui devra être ouverte et réglé à 115200 bauds
  les informations recus doivent être : "\nI2C Scanner"
                                        "système fonctionnellle"
                                        un entier représentant la tension de la batterie "mV"
                                        "attente bouton"
2) La led du robot sera allumé en blanc jusqu'à appuis sur le bouton go.
3) a. Le robot va faire un tour à droite (autour de la roue droite) par une méthode target.
   b. Puis de même à gauche
   c. Enfin, le robot va avancer tout droit pendant une demi seconde. Avec utilisation du flag
*/

#include <EVShield.h>                           //bibliothèques utile au programme
#include <EVs_Matrix.h>

#define MC_ADDR 0x10                            //adresse de la carte matrix

EVShield          evshield(0x34,0x36);          // définition de l'objet EVShield (BankA à l'adresse 0x34 et BankB à 0x36)
EVs_Matrix        controler(MC_ADDR);           // définition de l'objet controler matrix

int vitesse=40;                                 // la vitesse de déplacement des motors (entre 0 et 100)
long target=4000;                               // pour que le robot fasse un tour complet sur lui même (à changer en fonction de la taille des roues)


void setup() {

  // initialisation 
    evshield.init(SH_HardwareI2C);              // initialisation des echanges I2C
    controler.init(&evshield, SH_BAS2);         // initialisation de la communication avec le controleur branché sur la sortie BAS2 de l'EVshield

  // initialisation du moniteur série
    Serial.begin(115200);                       // initialisation du moniteur série à la fréquence 115200 bauds
    while (!Serial);                            // attente de fonctionnement du moniteur série
    Serial.println("\nI2C Scanner");            // confirmation du fonctionnement 

  // verification état motor
    controler.get_battery_state();             // vérification de l'état de la batterie
    int a;
    a=controler.get_battery_level()*40;        // mesure la batterie et la convertie en mV
    Serial.print(a);                            // écrit la tension de la batterie dans le moniteur série
    Serial.println(" mV");

  // mise à l'état initiale des motors
    Serial.println("------------------attente boutton------------------");
    Serial.println();
    delay(100);
    controler.set_motor_mode(SH_MM1,4);        // réinitialisation du motor 1 (gauche)
    controler.set_motor_mode(SH_MM3,4);        // réinitialisation du motor 3 (droite)
    evshield.bank_b.ledSetRGB(100,100,100);     // allume la led en blanc
    delay(100);
    while(!evshield.getButtonState(BTN_GO));
}





void loop() {

// tour à droite 
  // allume la led en rouge
    evshield.bank_b.ledSetRGB(100,0,0);       

  // mise aux états initiaux des motors gauches puis droit
    controler.set_motor_target(SH_MM1,target);                   // la position originalement à 0, va s'incrémenté jusqu'au 'target' puis s'arrêter
    controler.set_motor_speed(SH_MM1,vitesse);                   // le motor gauche va tourner à la vitesse 'vitesse' (sur une echelle de 0 à 100)
    controler.set_motor_speed(SH_MM3,0);                         // mise à 0 de la vitesse du motor droit
    controler.set_motor_mode(SH_MM3,1);                          // mise en mode brake du motor droit (il est bloqué pour une vitesse nulle)

  // affichage sur le moniteur des informations sur le motor gauche
    Serial.println("--------------------- Etat motor gauche ----------------------------");
    Serial.println("--------------------- initial ----------------------------");  
    Serial.print("position à :");               
    Serial.println(controler.get_motor_position(SH_MM1));  
    Serial.print("target à :");
    Serial.println(controler.get_motor_target(SH_MM1));
    Serial.print("la vitesse est à :");
    Serial.println(controler.get_motor_speed(SH_MM1));
    controler.get_motor_mode(SH_MM1);
    Serial.println();

  // mise en marche du motor gauche
    controler.set_motor_mode(SH_MM1,19);      //3=>utilisation du targer, mettre 3 fera tourner le robot dans l'autre sens
    delay(50);
    while (controler.is_motor_busy(SH_MM1)==1);

  // affichage sur le moniteur des informations sur le motor gauche
    Serial.println("--------------------- final ----------------------------");
    Serial.print("position à :");               
    Serial.println(controler.get_motor_position(SH_MM1));  
    Serial.print("target à :");
    Serial.println(controler.get_motor_target(SH_MM1));
    Serial.print("la vitesse est à :");
    Serial.println(controler.get_motor_speed(SH_MM1));
    controler.get_motor_mode(SH_MM1);
    delay(100);

  // réinitialisation des motors pour la prochaine tâche
    controler.set_motor_mode(SH_MM1,4);        // réinitialisation du motor 1 (gauche)
    controler.set_motor_mode(SH_MM3,4);        // réinitialisation du motor 3 (droite)    
    delay(100);
    Serial.println();
    

// tour à gauche
  // allumage de la led en vert
    evshield.bank_b.ledSetRGB(0,100,0);     

  // mise aux états initiaux des motors droit puis gauche
    controler.set_motor_target(SH_MM3,target);     // la position originalement à 0, va s'incrémenté jusqu'à 'target' puis s'arrêter
    controler.set_motor_speed(SH_MM3,vitesse);     // le motor gauche va tourner à la vitesse 'vitesse' (sur une echelle de 0 à 100)
    controler.set_motor_speed(SH_MM1,0);            // mise à 0 de la vitesse du motor gauche
    controler.set_motor_mode(SH_MM1,1);            // mise en mode brake du motor gauche (il est bloqué pour une vitesse nulle)
  
  // affichage sur le moniteur des informations sur le motor droit 
    Serial.println("--------------------- motor droit ----------------------------");
    Serial.println("--------------------- initial ----------------------------");
    Serial.print("position à :");               
    Serial.println(controler.get_motor_position(SH_MM3));  
    Serial.print("target à :");
    Serial.println(controler.get_motor_target(SH_MM3));
    Serial.print("la vitesse est à :");
    Serial.println(controler.get_motor_speed(SH_MM3));
    controler.get_motor_mode(SH_MM3);
    Serial.println();
  
  // mise en marche du motor
    controler.set_motor_mode(SH_MM3,3);     //3 => utilisation du targer, mettre 19 fera tourner le robot dans l'autre sens
    delay(50);
    while (controler.is_motor_busy(SH_MM3)==1);        //

  // affichage sur le moniteur des informations sur le motor droit
    Serial.println("--------------------- final ----------------------------");
    Serial.print("position à :");               
    Serial.println(controler.get_motor_position(SH_MM3));  
    Serial.print("target à :");
    Serial.println(controler.get_motor_target(SH_MM3));
    Serial.print("la vitesse est à :");
    Serial.println(controler.get_motor_speed(SH_MM3));
    controler.get_motor_mode(SH_MM3);

  // réinitialisation des motors pour la prochaine tâche    
    delay(100);
    controler.set_motor_mode(SH_MM1,4);        // réinitialisation du motor 1 (gauche)
    controler.set_motor_mode(SH_MM3,4);        // réinitialisation du motor 3 (droite)    
    delay(100);
    Serial.println();


//tout droit
    // allume la led en bleu
    evshield.bank_b.ledSetRGB(0,0,100);         
    
    // choix de la vitesse
    controler.set_motor_speed(SH_MM1,vitesse);
    controler.set_motor_speed(SH_MM3,vitesse);
  
    // choix du mode 
    controler.set_motor_mode(SH_MM1,24);
    controler.set_motor_mode(SH_MM3,8);
    delay(100);
    
    // lancement du flag
    controler.start_flag();
    Serial.println("--------------------- tout droit ----------------------------");  
    
    // attente de 0,5s
    delay(500);

    //arrêt des 2 motors
    controler.set_motor_mode(SH_MM1,4);
    controler.set_motor_mode(SH_MM3,4);
    
    // allume la led en blanc
    evshield.bank_b.ledSetRGB(100,100,100);    

    // fin du programme
    while(1);
}
