/*
    Centrale D17

    Cette centrale DCC genere:
    - un signal DCC pour conduire ou programmer des locomotives numeriques DCC
    - ou un signal PWM (+sens) pour une locomotive analogique
    La centrale pilote aussi des IO
    - en sortie: aiguillages, led, sorties, pwm, servos, decodeurs d'accessoires basiques et etendus
    - en entree: entrees numeriques d'un bus S88, et courant du booster
    La centrale se piloter par:
    - des clients TCP WIFI utilisant le protocle D17
    - des clients Websockets WIFI utilisant le ptotocole D17
    - un client USB utilisant le protocole D17 ou DCC++ ou DS
    La centrale dispose de son propre point d'acces Wifi integre.

    historique:
    - 2024/12/06: Sébastien Colas      Portage vers M5atom + TF-Card-Reader, ajout du support de mDNS, Serveur Web sur Core 0, Correction bug Upload de fichier
    - 2021/12/04: Ulysse Delmas-Begue  Si une page web est demandee hors AU, propose de passer en AU    
    - 2020/10/19: Ulysse Delmas-Begue  Mise en ligne, Activation par defaut des souris html
    - 2020/04/27: Ulysse Delmas-Begue  Possibilité de choisir le mode analogique/dcc independament de l'utilisation de l'adresse analogique par une souris
    - 2020/04/20: Ulysse Delmas-Begue  Ajout d'une impulsion bidirectionelle en analogique pour detecter les locos avec les detecteurs de courant  dcc
    - 2020/04/10: Ulysse Delmas-Begue  Ajout d'une constante pour regler la repetition des commandes accessoires
    - 2020/04/03: Ulysse Delmas-Begue  Correction d'un bug sur la commande des accessoires DCC (pendant un court instant) an analogique  
    - 2020/03/28: Ulysse Delmas-Begue  Augmentation du nombre des variables u de 96 a 192
    - 2020/03/28: Ulysse Delmas-Begue  Ajout des extensions BMVU, ITI, aubinage, SIG (experimental)
    - 2020/03/20: Ulysse Delmas-Begue  Mise en ligne de la version HTML
    - 2020/03/12: Ulysse Delmas-Begue  Liberation du mode analogique si une souris se deconnecte
    - 2020/03/12: Ulysse Delmas-Begue  Possibilite de regler le nombre de trames envoyees a chaque changement de fonction auxiliaire
    - 2020/03/11: Ulysse Delmas-Begue  Ajout de 5 canaux separes pour les websockets
    - 2020/03/10: Ulysse Delmas-Begue  Ajout du serveur Web HTTP et Websockets (5 clients websocket max en // des clients TCP et pages web)
    - 2020/03/09: Ulysse Delmas-Begue  Corection d'un bug sur le mask des variables u
                                       Inversion du sens pour DS
    - 2019/04/08: Ulysse Delmas-Begue  Correction d'un bug dans le protocole D17 wifi qui affecté les fonctions auxiliaires >= 10
    - 2019/03/05: Ulysse Delmas-Begue  Support de CDM Rail avec le protocole DCC++
    - 2019/02/25: Ulysse Delmas-Begue  Correction de bugs (envoie des adresses longues,  protocole desktop station, analogwrite)
    - 2019/02/22: Ulysse Delmas-Begue  Ajout des adresses longues 0-10239
    - 2019/02/21: Ulysse Delmas-Begue  Possibilité de connecter 8 clients wifi (qui peuvent etre sur des appareils differents)
                                       contre 4 auparavant (ce nombre est parametrable et mis a 4 par defaut)
    - 2019/02/21: Ulysse Delmas-Begue  Passage de 2 locos max a 4 locos max par souris (pour etre compatible avec JM)
    - 2019/02/21: Ulysse Delmas-Begue  Passage aux slots
    - 2019/01/30: Ulysse Delmas-Begue  Ajout du protocol Desktop Station
    - 2019/01/24: Ulysse Delmas-Begue  Correction d'un bug qui n'envoyait que la valeur 0 aux decodeurs DCC
    - 2018/03/23: Ulysse Delmas-Begue  Passage des preambules à 16 "1" sur les conseils de JM afin de restpecter la norme NEM671
                                       Il semblerait que ca fasse du bien a certains decodeurs recents
    - 2018/03/08: Ulysse Delmas-Begue  Merge des changements de JM
    - 2018/03/07: Jean-Michel Dubois   Proposition pour les changements suivants:
                                       - Ajout du support du mode station (en plus du mode point d'accès)
                                       - Generation des trames DCC plus claire
                                       - Ajout d'un mode pour gérer différement les fonctions auxiliaires
                                         (car les mises a jour permanentes des fonctions posent des problemes aux decodeurs ESU pour les sons)
                                       - Ajout des pointeurs pour simplifier la lisibilite du code --> TBD
    - 2018/02/22: Ulysse Delmas-Begue  Ajout du protocole DCC++ (light) pour connecter la centrale a RocRail par exemple
    - 2018/02/13: Ulysse Delmas-Begue  Ajout du choix de la polarite du bouton d'arret d'urgence
                                       Memorisation des entrees pour chaque souris afin de ne pas manquer les contacts fugitifs comme les ILS tres utile pour le mode script)
                                       Limitation a une seule reponse de demande de status par souris et par boucle (afin de ne pas surcharger le wifi)
                                       Choix de la commande des decodeurs d'accessoires en analogique (interdir, autoriser mais avec booster inactif, autoriser avec booster actif)
    - 2018/02/08: Ulysse Delmas-Begue  Ajout de la rotation lente des servos
    - 2018/02/07  Ulysse Delmas-Begue  Ajout du mode 126 crans en DCC. Vous pouvez choisir entre le mode 28 crans ou 126 crans (28 crans par defaut)
                                       Ajout d'un facteur pour diminuer la plage PWM en analogique si vous utilisez une alim > 12V
    - 2018/01/31  Ulysse Delmas-Begue  Passage de la resolution des tempos de 20ms a 250ms car on ne pouvait pas tenir cette resolution et les pca/led ne sont mis a jour que toute les 250ms
                                       La mise a jour des pca9685 est maintenant faite uniquement toute les 250ms (et seulement sur les canaux necessaires)
                                       Ajout d'un bit 0 au signal dcc lorsque la boucle peut prendre trop de temps afin de rester dans les tolerances de la norme DCC (bit0 = 10ms max par état, 12ms au total)
    - 2018/01/29  Ulysse Delmas-Begue  Ajout du bouton d'arrêt d'urgence sur D5
                                       Retrait temporaire de la variation lente des servos
    - 2018/01/22  Ulysse Delmas-Begue  Ajout de la commande u
                                       Ajout a l'etat des variables u
                                       Ajout de l'environement user de D18
                                       Optimisation de la transmission des fonctions auxiliaires f1+f2-f3+... -> f1+-+
    - 2018/01/21  Ulysse Delmas-Begue  Rajout de la commande des decodeurs d'accessoires etendus
    - 2018/01/18  Ulysse Delmas-Begue  Test avec le decodeur d'accessories D18
    - 2018/01/07  Ulysse Delmas-Begue  Test avec TCO sur tablette pour in et aig ok
    - 2018/01/06  Ulysse Delmas-Begue  La LED "sign of life" peut etre dispo maintenant sur le max
                                       Rajout de la commande des decodeurs d'accessoirs basiques
                                       Ajout d'une commande pour identifier la centrale
    - 2018/01/05  Ulysse Delmas-Begue  Passage a 96entrees, 128leds, 48srv/pwm
                                       Rajout de la commande des leds/srv/pwm par wifi
                                       Rajout variation lente des servos
    - 2017/12/29  Ulysse Delmas-Begue  Ajout des fonctions F13-F28
                                       Ajout des packets de 3bytes DCC (en plus du preambule et checksum)
                                       Ajout de la commande des decodeurs d'accesoires DCC
    - 2017/12/27  Ulysse Delmas-Begue  Passage de 0-28 crans a 0-100% pour les souris
                                       Ajout des fonctions F5-F12 (elle ne sont transmises que lorsque necessaire pour ne pas perdre de la bande passante)
                                       Ajout d'un second canal dcc pour chaque souris pr piloter 2 locos simultanees par souris
    - 2017/12/26  Ulysse Delmas-Begue  Ajout de l'I2C afin de proposer 16 PWM ou SRV par PCA9685 (16)
                                       Ajout du S88 (64)
    - 2017/12/24  Ulysse Delmas-Begue  Ajout des leds (64)
    - 2017/12/23  Ulysse Delmas-Begue  Ajout programation des CV
    - 2017/12/22  Ulysse Delmas-Begue  Ajout (temporaire) de 6 sorties controlees par le wifi
    - 2017/12/21  Ulysse Delmas-Begue  Correctif la centrale ne plante plus en DCC
                                       Gere les passages entre DCC et ANA.
                                       Ajout de la mesure du courant
    - 2017/12/16  Ulysse Delmas-Begue  Genere un signal PWM pour une locomotive ANAlogique en fonction d'une souris wifi
                                       Passage de 1kHz a 20KHz afin de rendre la PWM inaudible
    - 2017/06/27  Ulysse Delmas-Begue  Creation: Genere un signal DCC en fonction de 4 souris wifi max

    a faire:
    - ajouter les images dans tco html   <<<<<<<<<<<<<<<<<<<<< 
    - en html, faire une souris double sou2
    - ajouter sur tablette appli de test pour servo, pwm, led, in, aig, acc   <-- ersatz pour l'instant avec msg
    
    - ajouter gestion des aiguillage  <-- ersatz pour l'instant    
    - ID a la connection IP
    - voir combien de souris on peut avoir si D17 est en mode station
    - mettre le choix FCT_AUX_REPEAT au niveau des decodeurs, plutot qu'au niveau global.
    - idem pour le mode 126 crans
    - supporter le protocole D17 sur l'USB
    - supporter le protocle DesktopStation sur le WIFI (je n'y suis pas arrivé jusqu'a maintenant)
    - supporter le protocole DCC++ sur le WIFI
    - rendre le code compatible avec ESP32 (proto de base deja OK)
    
    a voir ???:
    - gestion de l'automatisation   <-- ersatz pour l'instant
    - voir si on passe la boucle de 250ms à 125ms comme sur le D18
    - voir, si on met plus de leds et pca comme sur le D18
    - voir si on met plus d'entrees
    - voir si on met du mouvement lent sur les servos
    - arreter les locos d'une souris qui ne donne plus de signe de vie (plus de demande de status)
    - cst pour autoriser web pendant non AU, la centrale se mettant en AU le temps de servir la page. (doit aussi marcher pour l'upload)
 
    a tester:
    - aubinage       <<<<<<<<<<<<
    - signalisation  <<<<<<<<<<<<
    - rien
  
    bugs:
    - voir incompatibilite iti du haut
    - aucun

    notes:
    - La centrale demarre en AU (Arret d'urgence = PowerOFF) avec le booster desactive
    - Si une souris TCP utilise l'adrresse 1, la centrale passe en mode analogique ANA (PWM)
    - Si aucune souris TCP n'utilise l'adresse 1, la centrale passe en mode DCC
    - Si le mode (DCC ou ANA) n'est pas autorise, la centrale passe quand meme dans ce mode mais se met en AU
    - Pour inactiver une souris, selectionner l'adresse 0 ou deconnectez la du wifi
    
    - La centrale utilise maintenant une liste de slots. Chaque slot peut contenir les parametres pour une locomotive (adresse, sens, vitesse, fonctions auxilaiaire)

        Les slots sont émis en permanence et en boucle. Ainsi si une loco perd le contact et donc sa configuration, elle sera mise a jour lorsqu'elle retrouvera le contact.
        Les souris (ou logiciel PC) modifient ces slots suivant les actions de l'utilisateur.
        Lorsque une nouvelle loco est utilisée, celle ci est rajouté dans un slot disponible.
        S'il n'y a pas de slot disponible, la loco remplace la loco dont le slot a la plus vielle modification.
        
   - Le programme ne doit pas bloquer la stack wifi pendant plus de 50ms (ajouter yield(0); or delay(0);)

        From: https://github.com/adafruit/ESP8266-Arduino
        Remember that there is a lot of code that needs to run on the chip besides the sketch when WiFi is connected.
        WiFi and TCP/IP libraries get a chance to handle any pending events each time the loop() function completes, OR when delay(...) is called.
        If you have a loop somewhere in your sketch that takes a lot of time (>50ms) without calling delay(),
        you might consider adding a call to delay function to keep the WiFi stack running smoothly.
        There is also a yield() function which is equivalent to delay(0).
        The delayMicroseconds function, on the other hand, does not yield to other tasks, so using it for delays more than 20 milliseconds is not recommended.

    pins:
                    RST #### TX
          SENCE -->  A0 #### RX
  DIN+DOUT/OUT1 <--  D0 #### D1  --> OUT0/LDIN
 AU + CLK /OUT2 <--  D5 #### D2  --> EN  +PWM
      SDA /OUT3 <--  D6 #### D3  --> DCC +SENS             (PU)
      SCK /OUT4 <--  D7 #### D4  --> LED (on board) / LLED (PU)
(PD) RSTIN/OUT5 <--  D8 #### GND
                   3.3V #### 5V

    gpio ds la datasheet de l'esp 8266
    - D0=GPIO16 / D1=GPIO5  / D2=GPIO4  / D3=GPIO0
    - D4=GPIO2  / D5=GPIO14 / D6=GPIO12 / D7=GPIO13
    - D8=GPIO15 / D9=GPIO3  / D1=GPIO1  / A0=NA

    durees des mises a jours:
    - in: 96bits @ 50us            -> 4.8ms
    - led: 2*8*16 = 256bits @ 10us -> 2.5ms
    - pca: 6*10bits @ 10us -> 0.6ms /ch
    - pca: 48* 0.6                 -> 29ms
    TOTAL: 36ms / 250ms

    protocoles:
                    WIFI     USB
      D17           TCP      --- : Souris Android (simple et double), souris D17 JM (Windows,Linux), souris potentiometriques esp8266
      D17           HTTP/WS  --- : Souris Web (en javascripts qui communiquent au travers de websockets)
      DCC++ leger   ---      oui : RocRail
      DS            ---      oui : DesktopStation

*/


#include <M5Atom.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>         // HTTP server
#include <FS.h>                // File System support
#include <SPIFFS.h>            // SPIFFS filesystem
#include <SPI.h>               // Needed by TFCard for M5Atom
#include <SD.h>                // Needed by TFCard for M5Atom
#include <WebSocketsServer.h>  // Websockets server

//==============================================================================================
//     USER PART
//==============================================================================================

//--------------------------------------------------------------------------------
// Constantes utilisateur (vous pouvez changer ces valeurs)
//----------------------------------------------------------------------------------

/* 1) WIFI configuration */

#define USE_WIFI 1        // 1 = activation du WIFI (defaut), 0 = pas de WIFI (par exemple si la centrale est uniquement controlee par USB)

#define APMODE  1         // 1 = access point (default), 0 = wifi station (D17 need to be connected to an external WIFI AP)

#if APMODE
    const char AP_name[] = "D17-0001";
    const char AP_pass[] = "ulysse31";  //pour etre valide, doit avoir au moins 8 caracteres.
#else
    const char STA_name[] = "your wifi SSID";  //ssid = wifi network name
    const char STA_pass[] = "your wifi password";
    #define IPADDRESS (192, 168, 0, 251)
    #define GATEWAY   (192, 168, 0, 1)
    #define SUBNET    (255, 255, 255, 0)
#endif

// Setting hostname
#if USE_WIFI == 1
  const char hostname[] = "centrale-d17";
#endif

// TCP Server configuration
#define USE_TCP 0             // 1 pour activer le serveur TCP d'origine de D17 (pour les souris RFO basic, mini cab et potentiomètriques) 
int tcp_port = 1234;

// Support du HTTP et websockets
#define USE_HTTP 1            // 1 pour activer le serveur HTTP et Websockets (0 pour desactiver)

// Nombre max de connexions
#define MAX_WIFI_CLIENTS 4      // nombre d'appareils WIFI max (0 a 8, 4 par defaut) mais semble ne pas vraiment servir
#define MAX_TCP_CLIENTS 4       // nombre de souris  TCP max   (0 a 8, 4 par defaut) mais semble ne pas vraiment servir
#define MAX_WEBSOCKET_CLIENTS 5 // nombre de souris  WEBSOCKET (0 a 8, 5 par defaut) mais semble ne pas vraiment servir


/* 2. DCC configuration */

// Slot number used to store loco commands and send them in loop.
#define NB_SLOTS 12      // 1 min, 12 by default

// CV programmation code
#define CV_PROG_CODE 1234

// speed mode
#define USER_USE_126_CRANS 0  // 0 to use 28 crans, 1 to use 126 crans
#define USER_PWM_FACTOR 100   // Plage d'utilisation de la pwm pour les locos analogiques (en %)
                              // afin de reduire la tension moyenne max si si l'alim > 12V
                              // (ex: 12V * 100% = 12V, 15V * 80% = 12V, 18V * 67% = 12V)
// dcc & analog modes
#define USER_DCC_OK  1        // 0 ne jamais utiliser le DCC, 1 ok pour utiliser le DCC
#define USER_ANA_OK  0        // 0 ne jamais utiliser l'analogique, 1 ok pour utilise l'analogique
#define USER_ACC_ANA 0        // commande des accessoires en analogique: 0 ne pas commander, 1 commander sans le booster, 2 commander avec le booster
#define USER_ANA_ADR_WS 0     // 1 = L'utilisation de l'adresse analogique par au moins une souris html fait passer la centrale automatiquement an analogique
                              // 0 = La souris doit envoyer une demande de changement de mode pour changer de mode (l'utilisateur appuie sur le bouton ANA/DCC)
#define USER_ANA_ADR_TCP 1    // idem pour les souris tcp (laisser a 1 pour la compatibilite avec les souris tcp actuelles qui ne disposent pas du bouton ANA/DCC)                              

// repetition des commandes accessoires
#define NB_TX_ACC 2 // nombre de transmission pour une commande accessoire (2x par defaut)
  
// repetition des fonctions auxiliaires
#define FCT_AUX_NB_REPEAT 4   // nombre de trames envoyees lorsqu'une fonction auxiliaire change ON ou OFF (generalement 1 a 4 par exemple)
                              // (cela peut être necessaire si PB de captage pendant l'envoie d'une trame) (1 par defaut)
                              
#define FCT_AUX_ON_REPEAT 1   // 1 (defaut) envoyer des trames tant que les fonctions auxiliaires actives  (bien pour les decodeurs basiques qui ne memorisent pas les fonctions en EEPROM en cas de perte de captage)
                              // 0 mettre a jour les fonctions auxiliaires seulement lorsqu'elles changent (bien pour les decodeurs qui memorisent les fonctions en EEPROM en cas de perte de captage)
                              // (pour les decodeurs ESU, mettre 0, sinon on a des problemes de son)
                              
#define EN_ANALOG_DET_PULSE 0 // activation de l'impulsion de detection bidirectionelle en analogique
#define EN_ANALOG_DET_PULSE_AT_V0 0 // si EN_ANALOG_DET_PULSE, activation aussi a l'arret V0 (bien pour la detection mais peut faire clignoter les leds donc pas tres estehtique)
/* 3. Others */

// configuration des entrees/sorties
#define USER_USE_S88  0       // 1 to use S88 IN     (out D0, D5, D1, D8 no more available)
#define USER_USE_MAX  0       // 1 to use MAX7219/21 (out D0, D5, D4     no more available) (la led etant sur D4 clignote en permanence) 
#define USER_USE_PCA  0       // 1 to use PCA9685    (out D6, D7         no more available)
#define USER_CLI_LED_MAX -1   // indicate which LED of MAX should blink to indicate sign of life (-1 not used)
#define USER_USE_AU   0       // 1 to use AU button on pin D5 (it can be used in // with S88, MAX, OUT) (very small glitch with OUT). Read the doc because 2 resistors must be used !
#define USER_AU_LEVEL 0       // 0 AU actif a l'etat bas, 1 AU actif a l'etat haut

// mode de l'USB
#define USB_MODE 1            // 0 inactive
                              // 1 D17 debug
                              // 2 protocole DCC++ (en version simplifiee sans configuration, mais ok pour Rocrail)
                              // 3 protocole Desktop station
                              // 4 protocole D17 (le meme que sur le WIFI) (pas encore dispo sur l'USB) UU: implementer
                              // 5 protocole DCC++ (en version simplifiee sans configuration, mais ok pour CDM-Rail)

// parametres pour le protocol DCC++
#define DCCPP_SENSOR_NB 16    // number of IN to use with DCC++ (96 max) Need to be a multiple of 8
#define DCCPP_MAX_CRAN  28    // MAX cran sent by DCC++ (generally 28 or 126)

// Version de la centrale
#define CENTRAL_NAME "D17 v20201204a"

// options de debug
#define DBG_1S 0          // 1 pour afficher la valeur de la mesure CAN toute les seconde
#define DBG_STATUS_MSG 1  // 1 pour afficher le message de status renvoye suite a la commande '?'

// option des extensions
// taille des tableaux (on peut en utiliser moins)
#define NB_BMVU        2  // nb max de cantons bmvu
#define NB_ITI        40  // nb d'iti max DA+TP
#define NB_SIG_ITI    15  // nb max de carré protegeant les iti
#define IO_LOCK_NB    16  // nb max de verrous d'itineraires (souvent utilises contre les nez-a-nez)
#define NB_AUBINAGE   10  // nb max d'aubinages
// constantes des extensions (be pas modifier)
#define DIR 0
#define DEV 1
#define X   255
#define DA  0
#define TP  1




//--------------------------------------------------------------------------------
// Code utilisateur (vous pouvez ajouter la gestion des accessoires)
//----------------------------------------------------------------------------------

// functions available for user
void user_out(byte num, byte val);
void user_led(byte num, byte val);
void user_led_cli(byte num, byte val);
void user_led_pha(byte num, byte val);
void user_pwm_0_100(byte num, byte val);
void user_servo_500_2500(byte num, unsigned int val);
void user_servo_speed(byte num, byte speed);  //delta pulse us / 250ms
void user_tempo_start(byte num_tempo, unsigned int duration_ms);
void user_bas_acc_dec_tx(unsigned int adr, byte out, byte val);  // UU: ajouter user_bas_acc_dec_tx(unsigned int num, byte val);
void user_ext_acc_dec_tx(unsigned int adr, byte val);
void user_set_d_e(byte num, byte val_d, byte val_e);
void user_set_u(byte num, byte val);
byte user_get_u(byte num);
byte user_get_in(byte num);
byte user_get_e(byte num);
byte user_get_d(byte num);

void bmvu_add(byte num, byte u, byte in_sem1, byte in_rel1, byte in_rel2, byte in_sem2);
void bmvu_click(byte u);
void iti_add(byte num, byte u, byte tp, byte cf, byte a0, byte a1, byte a2, byte a3, byte a4, byte a5, byte i0, byte i1, byte i2,  byte i3, byte i4, byte i5, byte lock, byte num_ind, byte u_ind);
void iti_click(byte num);
void aubinage_add(byte num, byte u_cmd, byte u_sig, byte in);


// User defines


// User functions

void user_init(void)
{
}


void user_notify_u(byte num, byte val)
{
    // Par defaut, variables u mappées sur decodeurs d'accessoires basiques
    // u0 -> accesoire 101 ->  adresse decodeur $26 sortie0,1 (si sorties numérotée 0-7), sortie 1,2 (si 1-8)
    // u1 -> acc102 dec $26 sortie2/3
    // u2 -> acc103 dec $26 sortie4/5
    // u3 -> acc104 dec $26 sortie6/7
    // u4 -> acc105 dec $27 sortie0/1
    // ...
    // u95 -> acc196 dec $49 sortie6/7

    byte adr = 26 + num/4;
    byte out = 2*(num%4)+val;  
    user_bas_acc_dec_tx(adr, out, 1);

    /* config ulysse
    if(num<=15)   //u0-u15  (adr10 & 11)
    {
        byte adr = 10 + num/8;
        byte out = num%8;  
        user_bas_acc_dec_tx(adr, out, val);
    }
    */
    
    user_set_u(num, val);
}


void user_notify_aig(byte num, byte cmd) // cmd=0=direct cmd=1=devie
{
    // Par defaut, aiguillages sur decodeurs d'accessoires basiques
    // aiguillage 0 -> accesoire 1 -> adresse decodeur $0 sortie0,1 (si sorties numérotée 0-7), sortie 1,2 (si 1-8)
    // aiguillage 1 -> acc2 dec $0 sortie2/3
    // aiguillage 2 -> acc3 dec $0 sortie4/5
    // aiguillage 3 -> acc4 dec $0 sortie6/7
    // aiguillage 4 -> acc5 dec $1 sortie0/1    
    // ...
    // aiguillage 47 -> acc48 dec $12 sortie6/7
    
    byte adr = 1 + num/4; 
    byte out = 2*(num%4)+cmd;  
    user_bas_acc_dec_tx(adr, out, 1);      

    /* config ulysse
    if(num<=15)   // a0-a15 (adr20,21,22,23)
    {
        byte adr = 20 + num/4; 
        byte out = 2*(num%4)+cmd;  
        user_bas_acc_dec_tx(adr, out, 1);
    }
    */

    user_set_d_e(num, 1 - cmd, cmd);
}


void user_notify_tempo_end(byte num_tempo)
{
}


void user_250ms()
{
}


void user_dccpp_acc(int adr1_510, byte pair0_3, byte val0_1)
{
    if(val0_1) user_bas_acc_dec_tx(adr1_510, 2*pair0_3+1, 1);
    else       user_bas_acc_dec_tx(adr1_510, 2*pair0_3  , 1);
}


void user_cdm_acc(int acc1_2048, byte val0_1)
{
    // conversion faite par le code par defaut
    //     acc1_2048 = 1 --> adr = 1, pair = 0
    //     acc1_2048 = 2 --> adr = 1, pair = 1
    //     acc1_2048 = 3 --> adr = 1, pair = 2
    //     acc1_2048 = 4 --> adr = 1, pair = 3
    //     acc1_2048 = 5 --> adr = 2, pair = 0
    //     ...
    //     acc1_2048 = 2048 --> adr = 512, pair = 3

    unsigned int adr_dec_1_512 = (acc1_2048 + 3) / 4;
    byte pair0_3 = (acc1_2048 + 3) % 4;

    if(val0_1) user_bas_acc_dec_tx(adr_dec_1_512, 2 * pair0_3 + 1, 1);
    else       user_bas_acc_dec_tx(adr_dec_1_512, 2 * pair0_3    , 1);
}


void user_ds_acc(int acc1_320, byte val0_1)
{
    // conversion faite par le code par defaut

    // acc1_320 = 1 --> adr = 1, pair = 0
    // acc1_320 = 2 --> adr = 1, pair = 1
    // acc1_320 = 3 --> adr = 1, pair = 2
    // acc1_320 = 4 --> adr = 1, pair = 3
    // acc1_320 = 5 --> adr = 2, pair = 0
    // ...
    // acc1_320 = 320 --> adr = 80, pair = 3

    unsigned int adr_dec_1_80 = (acc1_320 + 3) / 4;
    byte pair0_3 = (acc1_320 + 3) % 4;

    if(val0_1) user_bas_acc_dec_tx(adr_dec_1_80, 2 * pair0_3 + 1, 1);
    else       user_bas_acc_dec_tx(adr_dec_1_80, 2 * pair0_3    , 1);
}



//==============================================================================================
//     END of USER PART
//==============================================================================================



//--------------------------------------------------------------------------------
// Constantes system
//----------------------------------------------------------------------------------

// Analog Address
#define ADR_ANA 1

// Booster modes
#define BOOST_DCC 0
#define BOOST_ANA 1

// pins
#define S_PIN   G32         // Grove port Yellow cable
#define P_PIN   G26         // Grove port White cable
#define DCC_PIN S_PIN

// #define LED_PIN LED_BUILTIN //D4

#define CLK_PIN  0
#define DAT_PIN  0
#define LLED_PIN 0
#define LDIN_PIN 0
#define RST_PIN  0
#define SCK_PIN  0
#define SDA_PIN  0

#define AU_PIN   0   //can be used in // of CLK_PIN & OUT PIN (with small glitch for OUT)

// Set To 0 to disable the sensor
#define ANALOG_SENSOR 0

//------------------------------------------------------------------------------
// DIRECT OUT (7)
//------------------------------------------------------------------------------

void user_out(byte num, byte val)
{
    if(val == 1) val = HIGH; else val = LOW;
#if USER_USE_PCA == 0
            if(num == SDA_PIN) digitalWrite(SDA_PIN, val); //pin6
            if(num == SCK_PIN) digitalWrite(SCK_PIN, val); //pin7
#endif
#if USER_USE_S88 == 0
            if(num == LDIN_PIN) digitalWrite(LDIN_PIN, val); //pin1
            if(num == RST_PIN) digitalWrite(RST_PIN, val); //pin8
#endif
#if USER_USE_MAX == 0
    #if USER_USE_PCA == 0
            if(num == DAT_PIN) digitalWrite(DAT_PIN, val); //pin0
            if(num == CLK_PIN) digitalWrite(CLK_PIN, val); //pin5
    #endif
#endif
}



//------------------------------------------------------------------------------
// S88 (96)
//------------------------------------------------------------------------------

#define IO_IN_NB 96              // mettre un multiple de 16, max 96:  0,16,32,48,64,80,96

byte io_in[IO_IN_NB / 8];        // 96 entrees
byte io_in_old[IO_IN_NB / 8];    // memorisation pour identifier les chnagements
byte io_inmem_wifi[MAX_TCP_CLIENTS][IO_IN_NB / 8];  // memorisation des entrees pour chaque souris WIFI TCP
byte io_inmem_websocket[MAX_WEBSOCKET_CLIENTS][IO_IN_NB / 8];  // memorisation des entrees pour chaque souris WIFI websocket
byte io_inmem_usb[IO_IN_NB / 8];    //  memorisation des entrees pour un logiciel

// utilisation de 25us sur S88 soit une periode de 50us et une frequence de 20kHz
// Cette basse frequence permet de fonctionner avec des uc qui emulent lentement le S88 au lieu des 4014/2021
// Duree totale theorique pour 96 entrees: 4.8ms
byte in_shift8(void)
{
    byte i, dat;

    //pinMode(DAT_PIN, INPUT);

    for(i = 0; i < 8; i++)
    {
        dat = dat >> 1;
        if(digitalRead(DAT_PIN) == HIGH) dat |= 0x80; else dat &= 0x7f;
        digitalWrite(CLK_PIN, HIGH); delayMicroseconds(25); digitalWrite(CLK_PIN, LOW); delayMicroseconds(25);
    }
    return dat;
}


void s88_maj(void)
{
    byte i, j, v;

    pinMode(DAT_PIN, INPUT);

    digitalWrite(CLK_PIN , LOW ); delayMicroseconds(25);  //mise a 0 car utilisee par d'autres fonctions qui peuvent l'avoir mise a 1
    digitalWrite(LDIN_PIN, HIGH); delayMicroseconds(25);
    digitalWrite(CLK_PIN , HIGH); delayMicroseconds(25);
    digitalWrite(CLK_PIN , LOW ); delayMicroseconds(25);
    digitalWrite(LDIN_PIN, LOW ); delayMicroseconds(25);
    digitalWrite(RST_PIN , HIGH); delayMicroseconds(25);
    digitalWrite(RST_PIN , LOW ); delayMicroseconds(25);

    for(i = 0; i < (IO_IN_NB / 8); i++)
    {
        v = in_shift8();
        io_in_old[i] = io_in[i];
        io_in[i] = v;
        for(j = 0; j < MAX_TCP_CLIENTS; j++) io_inmem_wifi[j][i] |= v;
        for(j = 0; j < MAX_WEBSOCKET_CLIENTS; j++) io_inmem_websocket[j][i] |= v;; 
        io_inmem_usb[i] |= v;
        yield();
    }
}


void s88_init(void)
{
    byte i, j;

    for(i = 0; i < (IO_IN_NB / 8); i++)
    {
        io_in[i] = io_in_old[i] = io_inmem_usb[i] = 0;
        for(j = 0; j < MAX_TCP_CLIENTS; j++) io_inmem_wifi[j][i] = 0;
        for(j = 0; j < MAX_WEBSOCKET_CLIENTS; j++) io_inmem_websocket[j][i] = 0;        
    }
}


byte user_get_in(byte num)
{
    if(num < IO_IN_NB) if(io_in[num / 8] & 1 << (num & 7)) return 1;

    return 0;
}


void simu_set_in(byte num, byte val) // pour debug, injection de i lorsque USER_USE_S88 est desactive
{
#if USER_USE_S88 == 0
    if(num >= IO_IN_NB) return;

    byte ind = num / 8;
    byte msk = 1 << (num & 7);

    io_in_old[ind] = io_in[ind];
    if(val==2) io_in[ind] ^= msk;        // change
    if(val==1) io_in[ind] |= msk;        // set
    if(val==0) io_in[ind] &= (0xff^msk); // clr
#endif
}
void simu_in_update(void) // pour debug, injection de i lorsque USER_USE_S88 est desactive
{
#if USER_USE_S88 == 0
    byte i,j;
    
    for(i = 0; i < (IO_IN_NB / 8); i++)
    {
        for(j = 0; j < MAX_TCP_CLIENTS; j++) io_inmem_wifi[j][i] |= io_in[i];
        for(j = 0; j < MAX_WEBSOCKET_CLIENTS; j++) io_inmem_websocket[j][i] |= io_in[i];; 
        io_inmem_usb[i] |= io_in[i];
        yield();
    }
#endif
}



//--------------------------------------------------------------------------------
// Leds (128)
//----------------------------------------------------------------------------------

#define IO_LED_NB 128

byte io_led[IO_LED_NB / 8] = { 0, 0, 0, 0 ,0, 0, 0, 0,    0, 0, 0, 0 ,0, 0, 0, 0 };  // possibilite d'initialiser les valeurs
byte io_cli[IO_LED_NB / 8] = { 0, 0, 0, 0 ,0, 0, 0, 0,    0, 0, 0, 0 ,0, 0, 0, 0 };  // ex led clignotante
byte io_pha[IO_LED_NB / 8] = { 0, 0, 0, 0 ,0, 0, 0, 0,    0, 0, 0, 0 ,0, 0, 0, 0 };
byte led_cpt = 0;


void user_led(byte num, byte val)
{
    byte msk;
    msk = 1 << (num & 7);
    if(val) io_led[num / 8] |= msk;
    else    io_led[num / 8] &= (0xff ^ msk);
}


void user_led_cli(byte num, byte val)
{
    byte msk;
    msk = 1 << (num & 7);
    if(val) io_cli[num / 8] |= msk;
    else    io_cli[num / 8] &= (0xff ^ msk);
}


void user_led_pha(byte num, byte val)
{
    byte msk;
    msk = 1 << (num & 7);
    if(val) io_pha[num / 8] |= msk;
    else    io_pha[num / 8] &= (0xff ^ msk);
}


void led_shift8(byte dat)
{
    byte i;

    pinMode(DAT_PIN, OUTPUT);

    for(i = 0; i < 8; i++)
    {
         if(dat & 0x80) digitalWrite(DAT_PIN, HIGH);
         else           digitalWrite(DAT_PIN, LOW);

         delayMicroseconds(4);
         digitalWrite(CLK_PIN, HIGH);
         delayMicroseconds(4);  //periode=8us = 125KHz (ok sur longue distances)
         digitalWrite(CLK_PIN, LOW);

         dat = dat<<1;
    }
}


void led_reg(byte reg0, byte dat0, byte reg1, byte dat1)
{
     digitalWrite(CLK_PIN, LOW);
     digitalWrite(LLED_PIN, HIGH); delayMicroseconds(10); digitalWrite(LLED_PIN, LOW); delayMicroseconds(10);
     led_shift8(reg1);
     led_shift8(dat1);
     led_shift8(reg0);
     led_shift8(dat0);
     digitalWrite(LLED_PIN, HIGH); delayMicroseconds(10);
     yield();
}


byte compute_led_on_cli_phase(byte on, byte cli, byte phase)
{
    byte led;

    if(led_cpt & 1) led = 0xff;  else led = 0;   //clignotement 1Hz
    led ^= phase;                                //changement de phase
    led |= ~cli;                                 //allumer si pas clignotant
    led &= on;                                   //eteindre si pas on

    return led;
}


void led_maj(void)
{
    byte i, dat0, dat1;

    for(i = 0; i < 8; i++)
    {
        dat0 = compute_led_on_cli_phase(io_led[    i], io_cli[    i], io_pha[    i]);
        dat1 = compute_led_on_cli_phase(io_led[8 + i], io_cli[8 + i], io_pha[8 + i]);
        led_reg(1 + i, dat0, 1 + i, dat1);
    }
}


void led_init(void)
{
                              // R0    : bypass
                              // R1-8  : data
    led_reg(9  ,  0,  9,  0); // R9    : decode mode off (1ere fois pas sure que ca marche)
    led_reg(9  ,  0,  9,  0); // R9    : decode mode off
    led_reg(0xa,0x4,0xa,0x4); // R10   : intensite 4/15 (0xf=max)
    led_reg(0xb,  7,0xb,  7); // R11   : 8 digit
    led_reg(0xc,  1,0xc,  1); // R12   :shutdown mode off
                              // R13-14: NA
    led_reg(0xf,  0,0xf,  0); // R15   :test mode off (1=on)

    led_maj();
}

/*
void led_sign_of_life(byte val)
{
    #if USER_USE_MAX == 0
        if(val) digitalWrite(LED_PIN, LOW);  //active LOW
        else    digitalWrite(LED_PIN, HIGH);
    #endif
    #if USER_USE_MAX == 1
        #if USER_CLI_LED_MAX != -1
            user_led(USER_CLI_LED_MAX, val);
        #endif
    #endif
}
*/

void led_rgb_sign_of_life(CRGB color)
{
  M5.dis.drawpix(0,color);
}


//------------------------------------------------------------------------------
// I2C (I2C fait a la main pour pouvoir utiliser une basse frequence et n'importe quelle patte)
//------------------------------------------------------------------------------

//on emule un collecteur ouvert en mettant les sorties a 0 et en jouant sur la direction.
#define SDA_1 pinMode(SDA_PIN, INPUT)
#define SDA_0 pinMode(SDA_PIN, OUTPUT)
#define SCK_1 pinMode(SCK_PIN, INPUT)
#define SCK_0 pinMode(SCK_PIN, OUTPUT)


void i2c_init(void)
{
    pinMode(SCK_PIN, INPUT); digitalWrite(SCK_PIN, LOW);
    pinMode(SDA_PIN, INPUT); digitalWrite(SDA_PIN, LOW);
}


void i2c_start(void)
{
    SDA_1; SCK_1; delayMicroseconds(5);
    SDA_0; delayMicroseconds(5);
    SCK_0; delayMicroseconds(5);
}


void i2c_stop(void)
{
    SDA_0; SCK_0; delayMicroseconds(5);
    SCK_1; delayMicroseconds(5);
    SDA_1; delayMicroseconds(5);
}


void i2c_wr_8(byte data)
{
    byte i;

    for(i = 0; i < 8;i++)
    {
        if(data & 0x80) { SDA_1; } else { SDA_0; } delayMicroseconds(4);
        SCK_1; delayMicroseconds(4);
        SCK_0; delayMicroseconds(1);
        data = data << 1;
    }
    SDA_1; delayMicroseconds(1);
}


byte i2c_rd_ack(void)
{
    byte ack;

    delayMicroseconds(4);  //before 0
    SCK_1; delayMicroseconds(4);
    if(digitalRead(SDA_PIN) == HIGH) ack = 0; /*nack*/ else ack = 1; /*ack*/
    SCK_0; delayMicroseconds(1);
    return ack;
}


byte i2c_rd_8(void)
{
    byte i, data;

    data = 0;
    for(i = 0; i < 8; i++)
    {
        delayMicroseconds(4);  //before 0
        SCK_1; delayMicroseconds(4);
        if(digitalRead(SDA_PIN) == HIGH) data |= 1;
        SCK_0; delayMicroseconds(1);
        data = data << 1;
    }
    return data;
}


void i2c_wr_ack(byte ack)   //1=ack, 0=nack
{
    if(ack) { SDA_0; } else { SDA_1; } delayMicroseconds(4);
    SCK_1; delayMicroseconds(4);
    SCK_0; delayMicroseconds(1);
}



//------------------------------------------------------------------------------
// PCA9685 (3x16=48)
//------------------------------------------------------------------------------

#define PCA9685_MODE1_REG    0x00
#define PCA9685_MODE2_REG    0x01
#define PCA9685_LED0_REG     0x06
#define PCA9685_PRESCALE_REG 0xfe

#define IO_PCA9685_NB 3
#define IO_PCA9685_OUT_NB (16 * IO_PCA9685_NB)

byte pca9685_tx_srv[ IO_PCA9685_OUT_NB];  // bit0=tx  bit1=srv
unsigned int pca9685_on[ IO_PCA9685_OUT_NB];
unsigned int pca9685_off[IO_PCA9685_OUT_NB];
unsigned int pca9685_off_target[IO_PCA9685_OUT_NB];
byte srv_speed[IO_PCA9685_OUT_NB];


byte set_pca9685_reg(byte i2c_adr, byte adr, byte dat)
{
    i2c_start();
    i2c_wr_8(0x80 + 2 * i2c_adr);  //A6=1,A5-A0=pin,R/W#
    if(i2c_rd_ack() == 0)
    {
        i2c_stop();
        return 0;
    }
    i2c_wr_8(adr);
    if(i2c_rd_ack() == 0)
    {
        i2c_stop();
        return 0;
    }
    i2c_wr_8(dat);
    if(i2c_rd_ack() == 0)
    {
        i2c_stop();
        return 0;
    }
    i2c_stop();
    return 1;
}


void set_pca9685_on_off(byte i2c_adr, byte out, unsigned int on, unsigned int off)
{
    byte on_h, on_l, off_h, off_l;

    on_h  = byte(on  >> 8); on_l  = byte(on  & 255);  //existe aussi lowByte highByte
    off_h = byte(off >> 8); off_l = byte(off & 255);

    i2c_start();  // A6=1,A5-A0=pin,R/W#
    i2c_wr_8(0x80 + 2 * i2c_adr);          i2c_rd_ack();
    i2c_wr_8(PCA9685_LED0_REG + 4 * out);  i2c_rd_ack();
    i2c_wr_8(on_l);                        i2c_rd_ack();
    i2c_wr_8(on_h);                        i2c_rd_ack();
    i2c_wr_8(off_l);                       i2c_rd_ack();
    i2c_wr_8(off_h);                       i2c_rd_ack();
    i2c_stop();
    yield();
}


void set_pca9685_only_off_out(byte i2c_adr, byte out, unsigned int off)
{
    byte on_h, on_l, off_h, off_l;

    off_h = byte(off >> 8); off_l = byte(off & 255);

    i2c_start();  // A6=1,A5-A0=pin,R/W#
    i2c_wr_8(0x80 + 2 * i2c_adr);              i2c_rd_ack();
    i2c_wr_8(PCA9685_LED0_REG + 4 * out + 2);  i2c_rd_ack();
    i2c_wr_8(off_l);                           i2c_rd_ack();
    i2c_wr_8(off_h);                           i2c_rd_ack();
    i2c_stop();
    yield();
}


// only used at init
void set_pca9685_pwm0_100(byte num, byte pwm)
{
    byte i2c_adr;
    byte out;
    unsigned int off;

    i2c_adr = num >> 4;
    out = num & 15;
    off = map(pwm, 0, 100, 0, 4096); //c bien 4096 et pas 4095 !

    if(off == 0)         set_pca9685_on_off(i2c_adr, out,    0, 4096);
    else if(off == 4096) set_pca9685_on_off(i2c_adr, out, 4096,    0);
    else                 set_pca9685_on_off(i2c_adr, out,    0, off );
}


// no more used
void set_paca9685_srv50_250(byte num, byte pulse)  //50 -> 150 -> 250
{
    byte i2c_adr;
    byte out;
    unsigned int off;

    i2c_adr = byte(num >> 4);
    out = byte(num & 15);
    off = map(pulse, 50, 250, 102, 512);

    set_pca9685_on_off(i2c_adr, out, 0, off );
}


byte i2c_pca9685_online[IO_PCA9685_NB];

void pca9685_init(void)
{
    byte i;
    byte i2c_adr;

    for(i2c_adr = 0; i2c_adr < IO_PCA9685_NB; i2c_adr++)
    {
        // reset et config MODE1
        i2c_pca9685_online[i2c_adr] = set_pca9685_reg(i2c_adr, PCA9685_MODE1_REG, 0xB0); //reset=1, extclk=0, autoincrement=1, sleep=1 (all leds are off after reset)
        if(i2c_pca9685_online[i2c_adr] == 0) return;
        delay(1);

        // Frequence de decoupage
        // 50Hz pour les servos: 25.000.000/(4096*50Hz) - 1 = 122-1 = 121.
        // 60Hz pour les servos: 25.000.000/(4096*60Hz) - 1 = 102-1 = 101.
        // mesure:
        // 121 -> 18.7ms     -1.3ms     err -6.5%
        // 130 -> 19.92ms    -0.08ms    err  0.4%
        // 131 -> 20.07ms    +0.07ms    err  0.4%
        set_pca9685_reg(i2c_adr, PCA9685_PRESCALE_REG, 130);

        // exit from sleep
        set_pca9685_reg(i2c_adr, PCA9685_MODE1_REG, 0x20);    //reset=0, extclk=0, autoincrement=1, sleep=0
        delay(1); //wait exit from low power mode

        // config MODE2
        set_pca9685_reg(i2c_adr, PCA9685_MODE2_REG, 0x04);     // invert=0 change=onSTOP out=totem off=led_0
    }

    /* toutes les sorties a 0 */
    for(i = 0; i < IO_PCA9685_OUT_NB; i++)
    {
        if(i2c_pca9685_online[i / 16] == 0) continue;

        set_pca9685_pwm0_100(i, 0);      //uu: voir si on fait pas avant d'activer les sorties
        pca9685_on[i] = 0;
        pca9685_off[i] = 4096;
        pca9685_tx_srv[i] = 0;
        srv_speed[i] = 0;
    }

    /* spares
    set_pca9685_on_off(0,0 ,4096,0); //out 0 always on
    set_pca9685_on_off(0,1 ,0,4096); //out 0 always off
    set_pca9685_on_off(0,8 ,0,2048); //out 50%
    set_pca9685_on_off(0,9 ,0,100);  //servo 0.5ms   102  -> ok LA 457us
    set_pca9685_on_off(0,10,0,200);  //servo 1.0ms   206
    set_pca9685_on_off(0,11,0,300);  //servo 1.5ms   308  -> ok LA 1.46ms
    set_pca9685_on_off(0,12,0,400);  //servo 2.0ms   410
    set_pca9685_on_off(0,13,0,500);  //servo 2.5ms   512*4.882us=2.5ms
    set_paca9685_pwm0_100(0, 0);
    set_paca9685_srv50_250(11, 150);
    */
}


//variation lente des servos
void pca9685_slow_srv()
{
    byte num;
    unsigned int off, off_target;

    for(num = 0; num < IO_PCA9685_OUT_NB; num++)
    {
        if(i2c_pca9685_online[num / 16] == 0) continue; // PCA9685 module is not present

        if((pca9685_tx_srv[num] & 2) == 0) continue; // servos only

        if(pca9685_off_target[num] == pca9685_off[num]) continue;

        off_target = pca9685_off_target[num];
        off = pca9685_off[num];

        if(srv_speed[num]==0)
        {
            off = off_target;
        }
        else
        {
            if(off_target > off) { off += srv_speed[num]; if(off > off_target) off = off_target; }
            if(off_target < off) { off -= srv_speed[num]; if(off < off_target) off = off_target; }
        }
        if(pca9685_off[num] != off)
        {
            pca9685_off[num] = off;
            pca9685_tx_srv[num] |= 1;  //update
        }
    }
}


// return 1 when update is finished
byte pca9685_maj(byte index)
{
    byte i, num;
    byte i2c_adr;
    byte out;

    for(i = 0; i < 4; i++)
    {
        num = 4 * index + i;
        if(num >= IO_PCA9685_OUT_NB) return 1;

        if(i2c_pca9685_online[num / 16] == 0) continue;   // PCA9685 module is not present

        if(pca9685_tx_srv[num] & 1)
        {
            pca9685_tx_srv[num] &= 0xfe;

            i2c_adr = num / 16;
            out = num & 15;

            set_pca9685_on_off(i2c_adr, out, pca9685_on[num], pca9685_off[num]);
        }
    }

    return 0;
}


void user_pwm_0_100(byte num, byte val)
{
    unsigned int off;

    if(num >= IO_PCA9685_OUT_NB) return;

    off = map(val, 0, 100, 0, 4096);

    if(off == 0)         { pca9685_on[num] = 0;    pca9685_off[num] = 4096; }
    else if(off == 4096) { pca9685_on[num] = 4096; pca9685_off[num] =    0; }
    else                 { pca9685_on[num] = 0;    pca9685_off[num] =  off; }

    pca9685_tx_srv[num] |= 1;
    pca9685_tx_srv[num] &= 0xfd;  //not a servo
}


void user_servo_500_2500(byte num, unsigned int val)
{
    unsigned int off;

    if(num >= IO_PCA9685_OUT_NB) return;

    off = map(val, 500, 2500, 102, 512);    //4096=20ms > 102=0.5ms 512=2.5ms

    pca9685_on[num] = 0;    pca9685_off_target[num] = off;

    pca9685_tx_srv[num] |= 2;  //is a servo (no update, done in pca9685_slow_srv)

}


void user_servo_speed(byte num, byte speed)
{
    if(num >= IO_PCA9685_OUT_NB) return;

    srv_speed[num] = speed;
}



//--------------------------------------------------------------------------------
// Aiguillages
//----------------------------------------------------------------------------------

byte io_lock[IO_LOCK_NB];  //lock pour les itineraires incompatibles

#define IO_AIG_NB 48

byte io_aig_cmd[IO_AIG_NB];
byte io_aig_dir[IO_AIG_NB];
byte io_aig_dev[IO_AIG_NB];
byte io_aig_lock[IO_AIG_NB];  //uniquement utilise par les itineraires actuellement

void aig_init(void)
{
    byte i;

    for(i = 0; i < IO_AIG_NB; i++)
    {
        io_aig_cmd[i] = 0;
        io_aig_dev[i] = 0;
        io_aig_dir[i] = 0;
        io_aig_lock[IO_AIG_NB] = 0;
    }

    for(i = 0; i < IO_LOCK_NB; i++) io_lock[i] = 0;
}

void user_set_d_e(byte num, byte val_d, byte val_e)
{
    if(num >= IO_AIG_NB) return;
    io_aig_dir[num] = val_d;
    io_aig_dev[num] = val_e;
}

byte user_get_d(byte num)
{
    return io_aig_dir[num];
}

byte user_get_e(byte num)
{
    return io_aig_dev[num];
}

#if 0
void aig_maj(void)
{
    // stub pour tester
    byte i;
    for(i = 0; i < IO_AIG_NB; i++)
    {
        if(io_aig_cmd[i] == 1) io_aig_dev[i] = 1; else io_aig_dev[i] = 0;
        io_aig_dir[i] = io_aig_dev[i] ^ 1;
    }
}
#endif



//--------------------------------------------------------------------------------
// User vars
//----------------------------------------------------------------------------------

#define VAR_U_NB 192
#define VAR_U_NB_TXU 96
#define VAR_U_NB_TXV 96  // extension (pour les clients html)

byte var_u[VAR_U_NB / 8];

void set_var_u(byte num, byte val)
{
    byte ind = num / 8;
    byte msk = 1 << (num & 7);

    if(num >= VAR_U_NB) return;
    if(val) var_u[ind] |= msk;
    else    var_u[ind] &= (0xff ^ msk);
}

void inv_var_u(byte num)
{
    if(num >= VAR_U_NB) return;

    byte ind = num / 8;
    byte msk = 1 << (num & 7);

    var_u[ind] ^= msk;
}

byte get_var_u(byte num)
{
    if(num >= VAR_U_NB) return 0;

    byte ind = num / 8;
    byte msk = 1 << (num & 7);

    if(var_u[ind] & msk) return 1; else return 0;
}

void user_set_u(byte num, byte val)
{
    if(num < VAR_U_NB) set_var_u(num, val);
}

byte user_get_u(byte num)
{
    if(num >= VAR_U_NB) return 0;
    return get_var_u(num);
}



//--------------------------------------------------------------------------------
// User tempos
//----------------------------------------------------------------------------------

#define TEMPO_NB 80  //mettre un multiple de 8
unsigned int tempo_dcpt[TEMPO_NB];
byte tempo_flag[TEMPO_NB / 8];

void user_tempo_start(byte num_tempo, unsigned int duration_ms)
{
    if(num_tempo >= TEMPO_NB) return;

    //tempo_dcpt[num_tempo] = duration_ms / 20; //20ms increment
    tempo_dcpt[num_tempo] = duration_ms / 250; //250ms increment
    if(duration_ms > 0 && duration_ms < 250) tempo_dcpt[num_tempo] = 1;

    if(duration_ms) tempo_flag[num_tempo / 8] |= (1 << (num_tempo & 7));
    else            tempo_flag[num_tempo / 8] &= (0xff ^ (1 << (num_tempo & 7)));
}


void tempo_maj(void)  //a appeller toutes les 250ms
{
    byte i, j, num;

    for(j = 0; j < (TEMPO_NB / 8); j++) if(tempo_flag[j])
    for(i = 0; i < 8; i++) if(tempo_dcpt[i])
    {
        num = 8*j + i;
        tempo_dcpt[num]--;
        if(tempo_dcpt[num] == 0)
        {
            tempo_flag[num / 8] &= (0xff ^ (1 << (num & 7)));
            user_notify_tempo_end(num);
        }
    }
    yield();
}



//--------------------------------------------------------------------------------
// BMBU / ITI
//----------------------------------------------------------------------------------
//#include "bmvu.ino": no need other ino files are added at the end of main ino file
/*
void sig_init(void);
void sig_update(void);
void iti_update(void);
void sig_iti_update(void);
void bmvu_update(void);
void aubinage_update(void);
*/


//--------------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------------

byte boost = BOOST_DCC;
byte oldboost = BOOST_DCC;
byte au = 1;   //maintenant la centrale demarre en AU (donc alimenation off, cela est plus sure et permet de choisir le mode avant alimentation)
byte req_mode = 255;
byte pwmsens = 0;
unsigned int pwm1024 = 0;
unsigned int cv_adr = 1;
byte cv_dat = 3;
unsigned int an;   //derniere lecture du convertisseur analogique/numerique




//--------------------------------------------------------------------------------
// DCC signal   part
//----------------------------------------------------------------------------------

// DCC
// "1"
//    typ      58us
//    station  55-61us  (delta max entre periodes 3us)  --> measure LA OK: 1=57.5 or 61us
//    decodeur 52-64us  (delta max entre periodes 6us)
// "0"
//    typ      100us
//    station  95-9990us (total 12000us)
//
// IDLE     : 1111111111 0 11111111 0 00000000            0 EEEEEEEE 1
//
// VIT  28  : 1111111111 0 0AAAAAAA 0 01DFSSSS            0 EEEEEEEE 1
// VIT 127  : 1111111111 0 0AAAAAAA 0 00111111            0 DSSSSSSS 0 EEEEEEEE 1
//
// FCT 0-4  : 1111111111 0 0AAAAAAA 0 100-FL-F4-F3-F2-F1  0 EEEEEEEE 1
// FCT 5-8  : 1111111111 0 0AAAAAAA 0 1011-F8-F7-F6-F5    0 EEEEEEEE 1
// FCT 9-12 : 1111111111 0 0AAAAAAA 0 1010-F12-F11-F10-F9 0 EEEEEEEE 1
// FCT 13-20: 1111111111 0 0AAAAAAA 0 11011110            0 F20-F19-F18-F17-F16-F15-F14-F13 0 EEEEEEEE 1
// FCT 11-28: 1111111111 0 0AAAAAAA 0 11011111            0 F28-F27-F26-F25-F24-F23-F22-F21 0 EEEEEEEE 1
//
// ACC 1-511: 1111111111 0 10AAAAAA 0 1AAA1DDD            0 EEEEEEEE 1 attention, les 3xAAA sont inverses, ils forment aussi le MSB adr0 est reserve le 1 entre AetD peut etre mis a 0 pour mettre la sortie a 0, mais normalement allumer la 0 eteint la 1 et inversement. ou alors remise a 0 automatique apres une pulse
// ACC1-2047: 1111111111 0 10AAAAAA 0 0AAA0AA1 0 000xxxxx 0 EEEEEEEE 1
//
// une explication accessible sur le DCC est dispo sur: http://trainminiature.discutforum.com/t12784-dcc-comment-ca-marche-place-a-la-technique
//
// Pour les adresses longues (seulement pour les decodeurs mobiles):
// - Le champ d'adresse (0-127):  0AAAAAAA  (0 pour le broadcast)    cv1
// - est remplace par (0-10239):  11AAAAAA AAAAAAAA (0-39)           cv17 192+AAAAAA(0-39)  cv18 AAAAAAAA (0-255)
// - choix cv29.5 xx1xxxxx=long (126/28 crans independant de l'adressage court/long)

#define T1 55   // half period DCC bit 1
#define T0 100  // half period DCC bit 0


void dcc_byte(byte a)
{
  byte i;

  // separator
  digitalWrite(DCC_PIN, LOW); delayMicroseconds(T0); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T0); delay(0);

  for(i = 8; i; i--)    //MSB transmit first
  {
      if(a & 128) { digitalWrite(DCC_PIN, LOW); delayMicroseconds(T1); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T1); }
      else        { digitalWrite(DCC_PIN, LOW); delayMicroseconds(T0); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T0); }
      a <<= 1;
  }
}

byte dcc_tx(byte p_len, byte nb, byte a, byte b, byte c, byte d)
{
    byte x, i;

    // preambule
    for(i = p_len; i; i--)
    {
        digitalWrite(DCC_PIN, LOW); delayMicroseconds(T1); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T1);
    }

    // data
    dcc_byte(x = a);

    x ^= b;
    dcc_byte(b);

    if (nb >= 3)
    {
        x ^= c;
        dcc_byte(c);
    }

    if (nb >= 4)
    {
        x ^= d;
        dcc_byte(d);
    }

    // checksum
    dcc_byte(x);

    // end
    digitalWrite(DCC_PIN, LOW); delayMicroseconds(T1); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T1);

    // start a 0 (can last 9.9ms max)
    digitalWrite(DCC_PIN, LOW); delayMicroseconds(T0); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T0); delay(0);
    return x;
}


void dcc_tx2(byte p_len, byte a, byte c)
{
    dcc_tx(p_len, 2, a, c, 0, 0);
}


void dcc_tx3(byte p_len, byte a, byte b, byte c)
{
    dcc_tx(p_len, 3, a, b, c, 0);
}


void dcc_tx4(byte p_len, byte a, byte b, byte c, byte d)
{
    dcc_tx(p_len, 4, a, b, c, d);
}


void dcc_keepalive(void)   //a appeler si une fonction dure plus de 9.9ms pour que les decodeurs restent en mode DCC
{
    if(boost != BOOST_DCC) return

    // start a 0 (can last 9.9ms max)
    digitalWrite(DCC_PIN, LOW); delayMicroseconds(T0); digitalWrite(DCC_PIN, HIGH); delayMicroseconds(T0); delay(0);
}


// envoie d'une trame pour activer une sortie sur un decodeur d'accessoire basique
// normalement val=1 car les decodeurs d'accesoires sont souvent configures soit:
// - allumer la 0 eteint la 1 et vice versa
// - generer des pulses
// adr: 1-511
// out: 0-7
// val: 0/1
// ACC1-511: 1111111111111111 0 10AAAAAA 0 1AAAVDDD 0 EEEEEEEE 1
void user_bas_acc_dec_tx(unsigned int adr, byte out, byte val)
{
    byte b1 = 128 | ((byte)adr & 0x3f);
    byte b2 = (((byte)(adr >> 2) & 0x70) ^ 0x70) | 0x80 | (out & 7) | ((val & 1) << 3);

    if(boost == BOOST_ANA)
    {
        #if USER_ACC_ANA == 0   // pas de transmission en ANA
            return;
        #endif
        #if USER_ACC_ANA == 1   // desactivation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, 0);
        #endif
        #if USER_ACC_ANA == 2   // activation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, 1024);
        #endif
    }

    for(byte i = 0; i < NB_TX_ACC; i++) dcc_tx2(16, b1, b2);

    if(boost == BOOST_ANA)
    {
        if(pwmsens) digitalWrite(S_PIN, HIGH);
        else        digitalWrite(S_PIN, LOW);

        #if USER_ACC_ANA == 1   // desactivation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, pwm1024);
        #endif
        #if USER_ACC_ANA == 2   // activation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, pwm1024);
        #endif
    }
}


// envoie d'une valeur 0-31 a un decodeur d'accessoire etendu
// adr: 1-2044
// val: 031
// ACC1-2044: 1111111111111111 0 10AAAAAA 0 0AAA0AA1 0 000DDDDD 0 EEEEEEEE 1
void user_ext_acc_dec_tx(unsigned int adr, byte val)
{
    adr--; // 0-2043
    byte low = adr & 3; //0-3
    adr = adr >> 2; //0-510
    adr++; //1-511

    byte b1 = 128 | (byte)(adr & 0x3f);
    byte b2 = (((byte)(adr >> 2) & 0x70) ^ 0x70) | (low << 1) | 1;

    if(boost == BOOST_ANA)
    {
        #if USER_ACC_ANA == 0   // pas de transmission en ANA
            return;
        #endif
        #if USER_ACC_ANA == 1   // desactivation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, 0);
        #endif
        #if USER_ACC_ANA == 2   // activation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, 1024);
        #endif
    }

    for(byte i = 0; i < NB_TX_ACC; i++) dcc_tx3(16, b1, b2, val);

    if(boost == BOOST_ANA)
    {
        if(pwmsens) digitalWrite(S_PIN, HIGH);
        else        digitalWrite(S_PIN, LOW);

        #if USER_ACC_ANA == 1   // desactivation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, pwm1024);
        #endif
        #if USER_ACC_ANA == 2   // activation du booster durant la transmission
            if(au == 0) analogWrite(P_PIN, pwm1024);
        #endif
    }
}


// en mode analogique cette fonction alimente les voies dans un sens pendant 50us puis dans l'autre penadnt le meme temps
// afin que les detecteurs de courant dcc qui ne fonctionnent souvent que dans un sens detecte aussi les locos analogiques.
// 100us toutes les 250ms (1/2500=0.04% n'abime pas les locos) 
void analog_det_pulse(void)
{
    if(boost != BOOST_ANA) return;
    if(au) return;
    #if EN_ANALOG_DET_PULSE_AT_V0 == 0
    if(pwm1024==0) return;
    #endif

    analogWrite(P_PIN, 1024); // alimentation a 100%
    digitalWrite(DCC_PIN, LOW);  delayMicroseconds(50);  // dans un sens
    digitalWrite(DCC_PIN, HIGH); delayMicroseconds(50);  // puis dans l'autre
    
    // restoration
    if(pwmsens) digitalWrite(S_PIN, HIGH);
    else        digitalWrite(S_PIN, LOW);
    analogWrite(P_PIN, pwm1024);
}


// sens: 1=AV, 0=AR
// vit 0-28
byte vit28_2_dcc(byte sens, byte vit)
{
    if(vit > 0)
    {
        vit += 3;
        if(vit & 1) vit |= 32;
        vit = vit >> 1;
    }
    if(sens) vit |= 32;
    vit |= 64;

    return vit;
}
// vit 0-126
byte vit126_2_dcc(byte sens, byte vit)
{
    if(vit > 0) vit += 1;
    if(sens) vit |= 128;

    return vit;
}



//--------------------------------------------------------------------------------
// DCC prog   part
//----------------------------------------------------------------------------------

#define DCC_PROG_FRAME_IDLE  0
#define DCC_PROG_FRAME_RESET 1
#define DCC_PROG_FRAME_PAGE  2
#define DCC_PROG_FRAME_DAT   3

#define PROG_SEQ_SIZE 35
// 0x idle 3x reset 6x page 10x reset 6x dat 10x reset 0x idle
byte dcc_prog_seq[] = { \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
\
DCC_PROG_FRAME_PAGE, \
DCC_PROG_FRAME_PAGE, \
DCC_PROG_FRAME_PAGE, \
DCC_PROG_FRAME_PAGE, \
DCC_PROG_FRAME_PAGE, \
DCC_PROG_FRAME_PAGE, \
\
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
\
DCC_PROG_FRAME_DAT, \
DCC_PROG_FRAME_DAT, \
DCC_PROG_FRAME_DAT, \
DCC_PROG_FRAME_DAT, \
DCC_PROG_FRAME_DAT, \
DCC_PROG_FRAME_DAT, \
\
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
DCC_PROG_FRAME_RESET, \
};

// cv_adr = 1-1024
//     cv_pag = 1-255-0
//     cv_reg = 0-3
// cv_dat = 0-255
void prog_cv(unsigned int cv_adr, byte dat)
{
    byte seq_ind, a, c, cv_pag, cv_reg, cv_dat;

    cv_pag = ((cv_adr - 1) / 4) + 1;
    cv_reg = (cv_adr - 1) & 3;
    cv_dat = dat;

    for(seq_ind = 0; seq_ind < PROG_SEQ_SIZE; seq_ind++)
    {
        switch(dcc_prog_seq[seq_ind])
        {
            case DCC_PROG_FRAME_IDLE:
                    a = 0xff;
                    c = 0;
                    break;
            case DCC_PROG_FRAME_RESET:
                    a = 0;
                    c = 0;
                    break;
            case DCC_PROG_FRAME_PAGE:
                    a = 0x7D;  //0111CRRR C=1=WR RRR=5=page
                    c = cv_pag;
                    break;
            case DCC_PROG_FRAME_DAT:
                    a = (byte)0x78 + cv_reg;  //0111CRRR C=1=WR RRR=0(cv1),1(cv2),2(cv3),3(cv4) ...
                    c = cv_dat;
                    break;
        }
        dcc_tx2(22, a, c);
    }
}



//--------------------------------------------------------------------------------
// Booster management
//----------------------------------------------------------------------------------

byte slot_tx = 0;
                                          
unsigned int souris_adr[MAX_TCP_CLIENTS][4];  // stoque les adresses des locos pour chaque souris.
byte status_txed[MAX_TCP_CLIENTS];            // afin de savoir s'il y a l'adresse 1 et passer en analogique
unsigned int souris_websocket_adr[MAX_WEBSOCKET_CLIENTS][4]; //UU: trouver qc mieux que ca.
byte status_websocket_txed[MAX_WEBSOCKET_CLIENTS];

void init_souris_adr(void)
{
    byte i, j;
    for(i = 0; i < MAX_TCP_CLIENTS; i++)
    {
        status_txed[i] = 0;
        for(j = 0; j < 4; j++) souris_adr[i][j] = 0;
    }

    for(i = 0; i <MAX_WEBSOCKET_CLIENTS; i++)
    {
        status_websocket_txed[i] = 0;
        for(j = 0; j < 4; j++) souris_websocket_adr[i][j] = 0;
    }    
}

class C_dcc_ch
{
    public:
        unsigned int adr = 0;
        byte sens = 0;
        byte vit = 0;
        // UU: peut etre utiliser unsigned long pour les fonctions
        byte fct[29] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  //f0-f1---f28

        byte fct_0_4_modified = 0;
        byte fct_5_8_modified = 0;
        byte fct_9_12_modified = 0;
        byte fct_13_20_modified = 0;
        byte fct_21_28_modified = 0;
        byte vit_modified = 0;
};
C_dcc_ch slot_ch[NB_SLOTS];

void dcc_set_fct(byte slot, byte fct_num, byte val)
{
     if(slot_ch[slot].fct[fct_num] != val)
     {
         if(     fct_num<=4 ) slot_ch[slot].fct_0_4_modified   = FCT_AUX_NB_REPEAT;
         else if(fct_num<=8 ) slot_ch[slot].fct_5_8_modified   = FCT_AUX_NB_REPEAT;
         else if(fct_num<=12) slot_ch[slot].fct_9_12_modified  = FCT_AUX_NB_REPEAT;
         else if(fct_num<=20) slot_ch[slot].fct_13_20_modified = FCT_AUX_NB_REPEAT;
         else if(fct_num<=28) slot_ch[slot].fct_21_28_modified = FCT_AUX_NB_REPEAT;
     }
     slot_ch[slot].fct[fct_num] = val;
}

// Algo pour gérer les slots

byte first_slot = 0;  // slots le plus recement mis a jour
byte next_slot[NB_SLOTS];  // liste chainer des slots, du plus recent au plus vieux
                      // en dehors de la classe pour aller plus vite

byte get_slot_for_adr(unsigned int adr)
{
    yield();
    for(byte i = 0; i < NB_SLOTS; i++) if(slot_ch[i].adr == adr) return i;
    return 99; // error
}

byte get_free_or_oldest_slot(void)
{
    byte i = first_slot;
    for(byte j = 0; j < NB_SLOTS; j++)
    {
        if(slot_ch[i].adr == 0) { return i; /*Serial.print("NEW CH="); Serial.println(i);*/ }

        if(j == (NB_SLOTS - 1)) return i;
        i = next_slot[i];
    }
    yield();
    return 0; //never come here
}

void set_first_slot(byte slot)
{
    if(slot == first_slot) return;

    byte prev;
    for(byte j = 0; j < NB_SLOTS; j++) if(next_slot[j] == slot) prev = j;
    yield();

    next_slot[prev] = next_slot[slot];
    next_slot[slot] = first_slot;

    byte last = slot;
    for(byte j = 0; j < (NB_SLOTS - 1); j++) last = next_slot[last];
    yield();

    next_slot[last] = slot;
    first_slot = slot;
}

void slots_init(void)
{
    byte i;
    for(i = 0; i < NB_SLOTS; i++) next_slot[i] = i + 1;
    next_slot[NB_SLOTS - 1] = 0;
}

// ex de code PC pour une loco
/*
   printf("adr> "); scanf(" %d", &adr);
   slot = get_slot_for_adr(adr);     // <<<<< on regarde si elle existe deja
   if(slot == 99)
   {
        slot = get_free_or_older_ind();  // <<<<< si non on la rajoute sur un slot libre ou sur le plus vieux
        slot_ch[slot].adr = adr;
    }
    set_first_slot(slot);  // <<<<<< le slot de la loco devient le plus jeune
*/


// gestion du booster (AU, ANA, DCC)
void booster_maj(void)
{
    byte i, j;
    unsigned long adr;
    byte sens, vit;
    byte fct04_1, fct8_5, fct12_9, fct20_13, fct28_21;
    byte tx;

    // recherche parmis les souris si l'adresse analogique est utilidee
    byte find_ana = 0;
    
    #if USER_ANA_ADR_TCP == 1
    for(j = 0; j < MAX_TCP_CLIENTS; j++) for(i = 0; i < 4; i++) if(souris_adr[j][i] == ADR_ANA) find_ana = 1;
    #endif
    
    #if USER_ANA_ADR_WS == 1    
    for(j = 0; j < MAX_WEBSOCKET_CLIENTS; j++) for(i = 0; i < 4; i++) if(souris_websocket_adr[j][i] == ADR_ANA) find_ana = 1;
    #endif

    // choix du mode (du moins prioritaire au plus prioritaire)
    boost = BOOST_DCC;
    if(req_mode == BOOST_ANA) boost = BOOST_ANA;
    if(find_ana) boost = BOOST_ANA; 
    
    yield();

    if(boost != oldboost)
    {
        if(oldboost == BOOST_ANA) { analogWriteFrequency(50); analogWrite(P_PIN, 0);}
        oldboost = boost;
        if(boost == BOOST_ANA)
        {
            #if USB_MODE == 1
                Serial.println("BOOST ANA");
            #endif
            analogWriteFrequency(20000);
            analogWrite(P_PIN, 0);
        }
        if(boost == BOOST_DCC)
        {
            #if USB_MODE == 1
                Serial.println("BOOST DCC");
            #endif
        }
    }

    // si un mode est interdit, on y passe, mais on se met en AU
    #if USER_DCC_OK == 0
        if(boost == BOOST_DCC) au = 1;
    #endif
    #if USER_ANA_OK == 0
        if(boost == BOOST_ANA) au = 1;
    #endif

    if(boost == BOOST_ANA)
    {
        if(au) analogWrite(P_PIN, 0);
        else   analogWrite(P_PIN, pwm1024);

        if(pwmsens) digitalWrite(S_PIN, HIGH);
        else        digitalWrite(S_PIN, LOW);
    }

    if(boost == BOOST_DCC)
    {
        //maintent en cas d'arret d'urgence on coupe l'alimentation au lieu d'envoyer la trame d'arret d'urgence
        // ancienne solution
        //dcc_tx2(16, 0, 0x41); // AU : 1111111111111111 0 00000000 0 01000001 0 EEEEEEEE 1
        //notez qu'il est possible de transmettre des trames pour les acceoires par S_PIN (en dehors de cette fonction)
        if(au)
        {
            digitalWrite(P_PIN, LOW);
        }

        else
        {
            digitalWrite(P_PIN, HIGH);

            // GENERATION DU SIGNAL DCC via S_PIN

            byte nb_a = 0;
            byte nb_b = 0;
            while(1)
            {
                adr = slot_ch[slot_tx].adr;
                if(adr == 0) nb_b++;
                else
                {
                nb_a++;
                sens = slot_ch[slot_tx].sens;
                vit = slot_ch[slot_tx].vit;
                fct04_1  =  16*slot_ch[slot_tx].fct[0]  +  8*slot_ch[slot_tx].fct[4]  +  4*slot_ch[slot_tx].fct[3]  +  2*slot_ch[slot_tx].fct[2] + 1*slot_ch[slot_tx].fct[1];
                fct8_5   =   8*slot_ch[slot_tx].fct[8]  +  4*slot_ch[slot_tx].fct[7]  +  2*slot_ch[slot_tx].fct[6]  +  1*slot_ch[slot_tx].fct[5];
                fct12_9  =   8*slot_ch[slot_tx].fct[12] +  4*slot_ch[slot_tx].fct[11] +  2*slot_ch[slot_tx].fct[10] +  1*slot_ch[slot_tx].fct[9];
                fct20_13 = 128*slot_ch[slot_tx].fct[20] + 64*slot_ch[slot_tx].fct[19] + 32*slot_ch[slot_tx].fct[18] + 16*slot_ch[slot_tx].fct[17] + 8*slot_ch[slot_tx].fct[16] + 4*slot_ch[slot_tx].fct[15] + 2*slot_ch[slot_tx].fct[14] + 1*slot_ch[slot_tx].fct[13];
                fct28_21 = 128*slot_ch[slot_tx].fct[28] + 64*slot_ch[slot_tx].fct[27] + 32*slot_ch[slot_tx].fct[26] + 16*slot_ch[slot_tx].fct[25] + 8*slot_ch[slot_tx].fct[24] + 4*slot_ch[slot_tx].fct[23] + 2*slot_ch[slot_tx].fct[22] + 1*slot_ch[slot_tx].fct[21];

                // VIT  28: 1111111111111111 0 0AAAAAAA 0 01DFSSSS 0 EEEEEEEE 1
                // VIT 126  : 1111111111111111 0 0AAAAAAA 0 00111111 0 DSSSSSSS 0 EEEEEEEE 1
                #if USER_USE_126_CRANS == 0
                    if(adr <= 127) dcc_tx2(16, adr, vit28_2_dcc(sens, vit));
                    else           dcc_tx3(16, 0xC0 + highByte(adr), lowByte(adr), vit28_2_dcc(sens, vit));
                #endif
                #if USER_USE_126_CRANS == 1
                    if(adr <= 127) dcc_tx3(16, adr, 0x3f, vit126_2_dcc(sens, vit));
                    else           dcc_tx4(16, 0xC0 + highByte(adr), lowByte(adr), 0x3f, vit126_2_dcc(sens, vit));
                #endif
                slot_ch[slot_tx].vit_modified = 0;
                yield();

                // FCT 0-4: 1111111111111111 0 0AAAAAAA 0 100-FL-F4-F3-F2-F1 0 EEEEEEEE 1
                tx = 0;
                if(slot_ch[slot_tx].fct_0_4_modified) { tx = 1; slot_ch[slot_tx].fct_0_4_modified--; }
                #if FCT_AUX_ON_REPEAT == 1
                    else if(fct04_1) tx = 1;
                #endif
                if(tx)
                {
                    if(adr <= 127) dcc_tx2(16, adr, 128 + fct04_1);
                    else           dcc_tx3(16, 0xC0 + highByte(adr), lowByte(adr), 128 + fct04_1);
                }
                yield();

                // FCT 5-8: 1111111111111111 0 0AAAAAAA 0 1011-F8-F7-F6-F5 0 EEEEEEEE 1
                tx = 0;
                if(slot_ch[slot_tx].fct_5_8_modified) { tx = 1; slot_ch[slot_tx].fct_5_8_modified--; }
                #if FCT_AUX_ON_REPEAT == 1
                    else if(fct8_5) tx = 1;
                #endif
                if(tx)
                {
                    if(adr <= 127) dcc_tx2(16, adr, 0b10110000 | fct8_5);
                    else           dcc_tx3(16, 0xC0 + highByte(adr), lowByte(adr), 0b10110000 | fct8_5);
                }
                yield();

                // FCT 9-12: 1111111111111111 0 0AAAAAAA 0 1010-F12-F11-F10-F9  0 EEEEEEEE 1
                tx = 0;
                if(slot_ch[slot_tx].fct_9_12_modified) { tx = 1; slot_ch[slot_tx].fct_9_12_modified--; }
                #if FCT_AUX_ON_REPEAT == 1
                    else if(fct12_9) tx = 1;
                #endif
                if(tx)
                {
                    if(adr <= 127) dcc_tx2(16, adr, 0b10100000 | fct12_9);
                    else           dcc_tx3(16, 0xC0 + highByte(adr), lowByte(adr), 0b10100000 | fct12_9);
                }
                yield();

                // FCT 13-20: 1111111111111111 0 0AAAAAAA 0 11011110 0 F20-F19-F18-F17-F16-F15-F14-F13 0 EEEEEEEE 1
                if(slot_ch[slot_tx].fct_13_20_modified) { tx = 1; slot_ch[slot_tx].fct_13_20_modified--; }
                #if FCT_AUX_ON_REPEAT == 1
                    else if(fct20_13) tx = 1;
                #endif
                if(tx)
                {
                    if(adr <= 127) dcc_tx3(16, adr, 0b11011110, fct20_13);
                    else           dcc_tx4(16, 0xC0 + highByte(adr), lowByte(adr), 0b11011110, fct20_13);
                }
                yield();

                // FCT 21-28: 1111111111111111 0 0AAAAAAA 0 11011111 0 F28-F27-F26-F25-F24-F23-F22-F21 0 EEEEEEEE 1
                if(slot_ch[slot_tx].fct_21_28_modified) { tx = 1; slot_ch[slot_tx].fct_21_28_modified--; }
                #if FCT_AUX_ON_REPEAT == 1
                    else if(fct28_21) tx = 1;
                #endif
                if(tx)
                {
                    if(adr <= 127) dcc_tx3(16, adr, 0b11011111, fct28_21);
                    else           dcc_tx4(16, 0xC0 + highByte(adr), lowByte(adr), 0b11011111, fct28_21);
                }
                yield();

                }
                slot_tx++;
                if(slot_tx == NB_SLOTS) slot_tx = 0;
                if((4 * nb_a + nb_b) >= 8) break;  // exit after 2 locos slots or 8 empty slots (or a combination)
            } // while(1)

            if(nb_a == 0)  // transmit IDLE if 0 locos updated
            {
                // IDLE   : 1111111111111111 0 11111111 0 00000000 0 EEEEEEEE 1
                dcc_tx2(16, 0xff, 0);
            }
        }

    }  //(boost==BOOST_DCC)

    delay(0);
}



//--------------------------------------------------------------------------------
// Decode part - D17 protocol
//----------------------------------------------------------------------------------

char status_msg[128];

void update_status_message_with_in_for_tcp(byte ch)
{
    byte j, k, v;

    j = 9;

    for(k = 0; k < IO_IN_NB/8; k ++)
    {
        v = io_inmem_wifi[ch][k] & 0x0f;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        v = io_inmem_wifi[ch][k] >> 4;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        io_inmem_wifi[ch][k] = 0; // clr
    }
}

void update_status_message_with_in_for_websocket(byte ch)
{
    byte j, k, v;

    j = 9;

    for(k = 0; k < IO_IN_NB/8; k ++)
    {
        v = io_inmem_websocket[ch][k] & 0x0f;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        v = io_inmem_websocket[ch][k] >> 4;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        io_inmem_websocket[ch][k] = 0; // clr
    }
}
#define STATUS_EXT 91
void build_status_message(void)
{
    byte j, k, v;

    // au[0/1] ana/dcc iXXXXXXXXXXXXXXXXXXXXXXXX dXXXXXXXXXXXX eXXXXXXXXXXXX uXXXXXXXXXXXXXXXXXXXXXXXX cYY..vXXXXXXXXXXXXXXXXXXXXXXXX++
    // 01 2   3456    7890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567
    // 0                 1         2         3         4         5         6         7         8         9         10        11        12
    //..='\n'+'\0' pour les messages traditionnels ou remplace par ' '+' ' pour les messages html
    //++='\n'+'\0'
    j = 0;

    if(au) strcpy(status_msg, "au1 "); else strcpy(status_msg, "au0 ");
    j += 4;

    if(boost == BOOST_ANA) strcpy(&status_msg[j], "ana ");
    if(boost == BOOST_DCC) strcpy(&status_msg[j], "dcc ");
    j+=4;

    status_msg[j] = 'i'; j++;
    for(k = 0; k < IO_IN_NB/8; k ++)
    {
        v = io_in[k] & 0x0f;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        v = io_in[k] >> 4;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
    }
    status_msg[j] = ' '; j++;

    status_msg[j] = 'd'; j++;
    for(k = 0; k < IO_AIG_NB; k += 4)
    {
        v = 8 * io_aig_dir[k + 3] + 4 * io_aig_dir[k + 2] + 2 * io_aig_dir[k + 1] + io_aig_dir[k];
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
    }
    status_msg[j] = ' '; j++;

    status_msg[j] = 'e'; j++;
    for(k = 0; k < IO_AIG_NB; k += 4)
    {
        v = 8 * io_aig_dev[k + 3] + 4 * io_aig_dev[k + 2] + 2 * io_aig_dev[k + 1] + io_aig_dev[k];
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
    }
    status_msg[j] = ' '; j++;

    status_msg[j] = 'u'; j++;
    for(k = 0; k < VAR_U_NB_TXU/8; k ++)
    {
        v = var_u[k] & 0x0f;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        v = var_u[k] >> 4;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
    }
    status_msg[j] = ' '; j++;

    sprintf(&status_msg[j], "c%02d", map(an, 0, 1023, 0, 99));
    j += 3;
    status_msg[j] = '\n'; j++;
    status_msg[j] = '\0'; j++;

    status_msg[j] = 'v'; j++;
    for(k = 0; k < VAR_U_NB_TXV/8; k ++)
    {
        byte l = k + VAR_U_NB_TXU/8;
        v = var_u[l] & 0x0f;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
        v = var_u[l] >> 4;
        if(v < 10) status_msg[j] = '0' + v; else status_msg[j] = 'A' + v - 10;
        j++;
    }

    status_msg[j] = '\n'; j++;
    status_msg[j] = '\0'; j++;
}


// ex de trames:          // UU: mettre aussi ce protocol sur l'USB
//    au1
//    a3 s+ v8 f0-
//    cva0001 cvd0008 cvp1234

/* Protocole
 *
 * au1 = arret d'urgence, le booster est desactive
 * au0 = fin d'arret d'urgence
 *
 * aa = demande pour passer en mode analogique
 * ad = demande pour passer en mode dcc
 *
 * a<1-99>   = adresse de la locomotive et selection du canal A de la souris
 * b<1-99>   = adresse de la locomotive et selection du canal B de la souris
 * s+        = sens 1 (pour le canal selectionne)
 * s-        = sens 0
 * v<0-100>  = vitesse en %
 * f<0-28>+  = allumage fonction auxiliaire
 * f<0-28>-  = extinction fonction auxiliaire
 *
 * cva<1-1024> cvd<0-255> cvp<XXXX> = programation d'un decodeur de locomotive cva=adresse du CV, cvd=data cvp=code de protection a fixer dans ce programme afin d'interdir les programmation non autorisees
 *
 * x<1-511>.<0-7>+  = activation d'une sortie d'un decodeur d'accessoire
 * x<1-511>.<0-7>-  = desactivation d'une sortie d'un decodeur d'accessoire (normalement jamais utilise car l'activation de 0 desactive 1 et vice versa, ou alors si le decodeur est configure en impulsion, les sorties reviennent a 0 toutes seules)
 * y<1-2044>=<0-31> = afectation de la valeur 0-31 a un decodeur d'accessoire etendu
 *
 * o<0-5>+   = mise a 1 d'une sortie directe
 * o<0-5>-   = mise a 0 d'une sortie directe
 *
 * l<0-127>+ = allumage d'une led
 * l<0-127>- = extinction d'une led
 * c<0-127>+ = mode clignotant
 * c<0-127>- = mode fixe
 * h<0-127>- = phase normale
 * h<0-127>+ = phase inverse
 *
 * p<0-47>=<0-100>  = pwm (sur i2c)
 * s<0-47>=<50-250> = servo(sur i2c) = pulsation en 10us, neutre en 150*10us=1.5ms
 * s<0-47>-         = mettre le servo en position 0
 * s<0-47>+         = mettre le servo en position 1
 * w<0-47>=<0-255>  = vitesse de rotation des servos (0=max, nb de us / 250ms, ex: 1 = 1us/250ms = 4us/sec, 1000->1500us en 125s)
 * Le choix entre pwm et servo se regle au debut du fichier d ela centrale
 * Pour utiliser les sorties en 0/1, utiliser pwm0/100
 * La vitesse de rotation des servos, ainsi que les positions extremes sont reglables
 *
 * t<0-47>- = aiguillage en position direct (t=turnable, aiguillage en Americain)
 * t<0-47>/ = aiguillage en position devie
 * t<0-47>^ = inversion de la position de l'aiguillage
 * l'aiguillage peut etre de n'importe quel type(bobine, moteur, serv) et situe n'importe ou(en sortie de l'esp, sur un module i2c pca9685, sur un decodeur d'accessoire),
 * c'est a l'utilisateur de le definir dans ce programme. Il est egalement possible d'indiquer la duree de l'impulsion et inverser la commande
 * les rapport d et e sont mis a jour un efois la commande effectuee
 *
 * u<0-95>- = mettre a 0 la variable utilisateur
 * u<0-95>+ = mettre a 1 la variable utilisateur
 * u<0-95>^ = inverser la variable utilisateur
 *
 * ? = demande de l'emission de l'etat
 * au[0/1] ana/dcc iXXXXXXXXXXXXXXXXXXXXXXXX dXXXXXXXXXXXX eXXXXXXXXXXXX uXXXXXXXXXXXXXXXXXXXXXXXX cYY
 * 01 2   3456    789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012
 * 0                 1         2         3         4         5         6         7         8         9          10
 * X = codage en hexa 0-F pour transmettre 4 bits par caracter afin de ne pas surcharger le lien
 *     on aurait pu passer en binaire pour encore plus gagner, mais je voulais rester en ASCII
 * i = entree (0-95)
 * u = etat des commandes utilisateur
 * d = aiguillage en position directe (0-47)
 * e = aiguillage en position devie   (0-47)
 * c = convertisseur analogique numerique (souvent utilise pour mesurer le courant
 * YY = 0(0V), 99(1V)
 *
 * ! = information sur la centrale: ie: D17 v20170106a
 *
 */
char tx[30];  // tx debug

void parse_rx(byte cclient, char* rx, byte is_websocket = 0)
{
    byte i, j, v, nb, is_num;
    unsigned int nbi;
    byte slot = 99;
    unsigned int adr = 0;
    byte ch = 0;

    i = 0;

    while(i != strlen(rx))
    {
        // pre-traitement
        is_num = 0; if(rx[i + 1] >= '0' && rx[i + 1] <= '9') is_num = 1;


        // skip

        if(rx[i] == '\n') break;

        else if(rx[i] == ' ') i += 1;

        else if(rx[i] == 'c' && rx[i+1] == 'a') // cabine_net v1
        {
            #if USB_MODE == 1
                Serial.println("cabine_net v1 no more supported !");
            #endif
            while(1) delay(0);
        }

        else if(rx[i] == 's' && rx[i+1] == 'o') break; // souris_d17_net v1


        // locomotives

        else if(rx[i] == 'a' && rx[i + 1] == 'u')
        {
            if(rx[i + 2] == '0')  au = 0;
            else if(rx[i + 2] == '^') au ^= 1;
            else au = 1;
            i += 3;
        }
        
        else if(rx[i] == 'a' && rx[i + 1] == 'a')  // demande pour passer en analogique
        {    
            req_mode = BOOST_ANA;
            i += 2;
        }

        else if(rx[i] == 'a' && rx[i + 1] == 'd')  // demande pour passer en digital
        {    
            req_mode = BOOST_DCC;
            i += 2;
        }

        else if((rx[i] == 'a' || rx[i] == 'b' || rx[i] == 'C' || rx[i] == 'D') && is_num)
        {
            //Serial.println("$");
            if(rx[i] == 'a') ch = 0;
            if(rx[i] == 'b') ch = 1;
            if(rx[i] == 'C') ch = 2;
            if(rx[i] == 'D') ch = 3;

            i++;
            adr = 0;
            while(rx[i] >= '0' && rx[i] <= '9')
            {
                adr = 10 * adr + rx[i] - '0';
                i++;
            }
            if(is_websocket == 0) souris_adr[cclient][ch] = adr; // Serial.println("ADR"); Serial.println(adr);
            if(is_websocket == 1) souris_websocket_adr[cclient][ch] = adr;

            slot = 99;
            if(adr != ADR_ANA)
            if((adr >= 1) && (adr <= 10239))
            {
                slot = get_slot_for_adr(adr); // Serial.println("SLOT"); Serial.println(slot);
                if(slot == 99)
                {
                    slot = get_free_or_oldest_slot();
                    slot_ch[slot].adr = adr;
                    slot_ch[slot].sens = 1; // 1=AV                       //UU: voir si on initialise ou pas
                    slot_ch[slot].vit = 0;                                //UU: voir si on demande a mettre a jours la loco avec les modified=1
                    for(j = 0; j < 29; j++) slot_ch[slot].fct[j] = 0;
                }
                set_first_slot(slot);
            }
        }

        else if(rx[i] == 's' && ( rx[i+1] == '+' || rx[i+1] == '-' ) )
        {
            if(slot != 99)
            {
                if(rx[i + 1] == '+') slot_ch[slot].sens = 1;
                else                 slot_ch[slot].sens = 0;
            }

            if(adr == ADR_ANA)
            {
                if(rx[i + 1] == '+')  pwmsens = 1; else pwmsens = 0;
            }
            i += 2;
        }

        else if(rx[i] == 'v' && is_num)
        {
            i++;
            nbi = 0;
            while(rx[i] >= '0' && rx[i] <= '9')
            {
                nbi = 10 * nbi + (rx[i] - '0');
                i++;
            }
            nb = (byte)nbi;  //0-100%

            if(slot != 99)
            {
            #if USER_USE_126_CRANS == 0
                slot_ch[slot].vit = map(nb, 0, 100, 0, 28);  //0-100% -> 0-28crans
            #endif
            #if USER_USE_126_CRANS == 1
                slot_ch[slot].vit = map(nb, 0, 100, 0, 126);  //0-100% -> 0-126crans
            #endif
            }

            if(adr == ADR_ANA)
            {
                //pwm1024 = nb * 1024 / 100;
                pwm1024 = nb * 1024 * USER_PWM_FACTOR / 100 / 100;
                if(pwm1024 < 40) pwm1024 = 0;                // limitations aux extremites du a un bug de la fonction analogwrite
                if(pwm1024 > (1023 - 40)) pwm1024 = 1024;    // 1024 met la sortie a 1 en permanence
                //Serial.print("pwm="); Serial.print(pwm1024); Serial.print("<\n");
            }
        }

        else if(rx[i] == 'f' && is_num)
        {
            i++;
            nb = 0;
            while(rx[i] >= '0' && rx[i] <= '9')
            {
                nb = 10 * nb + (rx[i] - '0');
                i++;
            }
            while(nb<=28)
            {
                if(     rx[i] == '+') { if(slot != 99) dcc_set_fct(slot, nb, 1); i++; nb++; }
                else if(rx[i] == '-') { if(slot != 99) dcc_set_fct(slot, nb, 0); i++; nb++; }
                else break;
            }
        }

        else if(rx[i] == 'c' && rx[i + 1] == 'v')
        {
            byte cmd = rx[i+2]; 
            i+=3;
            nbi = 0;
            while(rx[i] >= '0' && rx[i] <= '9')
            {
                nbi = 10 * nbi + (rx[i] - '0');
                i++;
            }            
            if(cmd == 'a') cv_adr = nbi;
            if(cmd == 'd') cv_dat = (byte) nbi;
            if(cmd == 'p') if(nbi == CV_PROG_CODE) if(boost == BOOST_DCC)
            {
                led_rgb_sign_of_life(0x00ff00); //LED ON Green
                #if USB_MODE == 1
                    Serial.printf(tx, "prog cv%d=%d\n", cv_adr, cv_dat); // tx globale sinon marche pas
                #endif
                // programing seqeunce
                prog_cv(cv_adr, cv_dat);
                // disable booster during 1s to reboot decoder
                digitalWrite(P_PIN, LOW);
                digitalWrite(S_PIN, LOW);
                led_rgb_sign_of_life(0);
                delay(1000); //UU: possible to check with can
                digitalWrite(P_PIN, HIGH);
                #if USB_MODE == 1
                    Serial.println("prog done");
                #endif
            }
        }

        // entrees/sorties

        // direct out

        else if(rx[i] == 'o' && is_num)
        {
            nb = rx[i + 1] - '0';

            if(rx[i + 2] == '+') user_out(nb, 1); else user_out(nb, 0);
            i += 3;
        }

        // led

        else if(rx[i] == 'l' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < IO_LED_NB)
            {
                if(rx[i] == '+') user_led(nb, 1); else user_led(nb, 0);
            }
            i++;
        }
        else if(rx[i] == 'c' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < IO_LED_NB)
            {
                if(rx[i] == '+') user_led_cli(nb, 1); else user_led_cli(nb, 0);
            }
            i++;
        }
        else if(rx[i] == 'h' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < IO_LED_NB)
            {
                if(rx[i] == '+') user_led_pha(nb, 1); else user_led_pha(nb, 0);
            }
            i++;
        }

        // pwm p<0-47>=<0-100>  = pwm (sur i2c)

        else if(rx[i] == 'p' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            i++; //skip '='
            nbi = 0; while(rx[i] >= '0' && rx[i] <= '9') { nbi = 10 * nbi + rx[i] - '0'; i++; }
            if(nb < IO_PCA9685_OUT_NB) user_pwm_0_100(nb, nbi);
        }

        // servos s<0-47>=<50-250> = servo(sur i2c) = pulsation en 10us, neutre en 150*10us=1.5ms
        //        s<0-47>-         = mettre le servo en position 0
        //        s<0-47>+         = mettre le servo en position 1
        //        s<0-47>/         = mettre le servo en position 1
        //        w<0-47>=<0-255>  = vitesse de rotation des servos (0=max, nb de us / 250ms, ex: 1 = 1us/250ms = 4us/sec, 1000->1500us en 125s)
        else if(rx[i] == 's' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(rx[i] == '=')
            {
                i++;
                nbi = 0; while(rx[i] >= '0' && rx[i] <= '9') { nbi = 10 * nbi + rx[i] - '0'; i++; }
                if(nb < IO_PCA9685_OUT_NB) user_servo_500_2500(nb, 10 * nbi);
            }
            /* desactive
            else
            {
                if(nb < IO_PCA9685_OUT_NB)
                {
                    if(rx[i] == '-') set_srv_50_250(nb, srv_pos_0[nb]);
                    if(rx[i] == '+') set_srv_50_250(nb, srv_pos_1[nb]);
                    if(rx[i] == '/') set_srv_50_250(nb, srv_pos_1[nb]);
                }
                i++;
            }
            */
        }
        else if(rx[i] == 'w' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(rx[i] == '=')
            {
                i++;
                nbi = 0; while(rx[i] >= '0' && rx[i] <= '9') { nbi = 10 * nbi + rx[i] - '0'; i++; }
                if(nb < IO_PCA9685_OUT_NB) user_servo_speed(nb, nbi);
            }
        }

        // decodeur d'accesssoire "basique"
        // x<1-511>.<0-7>+ = activation d'une sortie d'un decodeur d'accessoire
        // x<1-511>.<0-7>- = desactivation d'une sortie d'un decodeur d'accessoire
        //                   (normalement jamais utilise car l'activation de la sortie 0 desactive la 1 et vice versa,
        //                   ou alors si le decodeur est configure en impulsion, les sorties reviennent a 0 toutes seules)
        else if(rx[i] == 'x' && is_num)
        {
            i++; nbi = 0; while(rx[i] >= '0' && rx[i] <= '9') { nbi = 10 * nbi + rx[i] - '0'; i++; }
            i++; //skip '.'
            nb = rx[i] - '0'; i++;
            if(nbi >= 1 && nbi <= 511 && nb<=7)
            {
                if(rx[i] == '-') v = 0; else v = 1;
                //user_notify_bas_acc_dec(nbi, nb, v);
                user_bas_acc_dec_tx(nbi, nb, v);
            }
            i++;
        }

        // decodeur d'accesssoire "extended"
        // y<1-2044>=<0-31> activation d'une sortie d'un decodeur d'accessoire
        else if(rx[i] == 'y' && is_num)
        {
            i++; nbi = 0; while(rx[i] >= '0' && rx[i] <= '9') { nbi = 10 * nbi + rx[i] - '0'; i++; }
            i++; //skip '='
            nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nbi >= 1 && nbi <= 2044 && nb <= 31)
            {
                //user_notify_ext_acc_dec(nbi, nb);
                user_ext_acc_dec_tx(nbi, nb);
            }
            i++;
        }

        // aiguillages
        // t<0-47>- = aiguillage en position direct (t=turnable, aiguillage en Americain)
        // t<0-47>/ = aiguillage en position devie
        // t<0-47>^ = inversion de la position de l'aiguillage
        else if(rx[i] == 't' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < IO_AIG_NB)
            {
                if(rx[i] == '-') io_aig_cmd[nb]  = 0;
                if(rx[i] == '+') io_aig_cmd[nb]  = 1;
                if(rx[i] == '/') io_aig_cmd[nb]  = 1;
                if(rx[i] == '^') io_aig_cmd[nb] ^= 1;

                user_notify_aig(nb, io_aig_cmd[nb]);
            }
            i++;
        }

        // user variables
        // u<0-191>- = mettre a 0 la variable utilisateur
        // u<0-191>+ = mettre a 1 la variable utilisateur
        // u<0-191>^ = inverser la variable utilisateur
        else if(rx[i] == 'u' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < VAR_U_NB)
            {
                if(rx[i] == '-') user_notify_u(nb,0);
                if(rx[i] == '+') user_notify_u(nb,1);
                if(rx[i] == '/') user_notify_u(nb,1);
                if(rx[i] == '^') user_notify_u(nb, get_var_u(nb)^1);
            }
            i++;
        }

        // simule le changement des entrees (uniquement pour debug)
        // i<0-95>- = met a 0 l'entree
        // i<0-95>+ = met a 1 l'entree
        // i<0-95>^ = inverse l'entree
        else if(rx[i] == 'i' && is_num)
        {
            i++; nb = 0; while(rx[i] >= '0' && rx[i] <= '9') { nb = 10 * nb + rx[i] - '0'; i++; }
            if(nb < IO_IN_NB)
            {
                if(rx[i] == '-') simu_set_in(nb,0);
                if(rx[i] == '+') simu_set_in(nb,1);
                if(rx[i] == '^') simu_set_in(nb,2);
            }
            i++;
        }

        // querry
        else if(rx[i] == '?')
        {
            i++;
            if(status_msg[0] == '\0')
            {
                  build_status_message();  // to build only once the message
                  for(byte i = 0; i < MAX_TCP_CLIENTS; i++) status_txed[i] = 0;
                  for(byte i = 0; i < MAX_WEBSOCKET_CLIENTS; i++) status_websocket_txed[i] = 0;
            }
            if(is_websocket == 0) 
            if(status_txed[cclient] == 0)  //to send message only once each 250ms
            {
                update_status_message_with_in_for_tcp(cclient);
                #if USB_MODE == 1
                #if DBG_STATUS_MSG == 1
                    Serial.println(status_msg);
                #endif
                #endif
                status_txed[cclient] = 2;  //2=to transmit  1=transmitted
            }
            if(is_websocket == 1)
            if(status_websocket_txed[cclient] == 0)
            {
                update_status_message_with_in_for_websocket(cclient);
                #if USB_MODE == 1
                #if DBG_STATUS_MSG == 1
                    Serial.println(status_msg);
                #endif
                #endif
                status_websocket_txed[cclient] = 2;              
            }
        }

        else if(rx[i] == '!')
        {
            i++;
            #if USB_MODE == 1
                strcpy(tx,CENTRAL_NAME); // passer par tx
                Serial.println(tx);
            #endif
        }


        else i++;

        yield();
    }
}



//--------------------------------------------------------------------------------
// WIFI part
//----------------------------------------------------------------------------------
#if USE_WIFI == 1

void start_mdns_service()
{
    //initialize mDNS service
    esp_err_t err = mdns_init();
    if (err) {
        #if USB_MODE == 1
            Serial.print("MDNS Init failed\n");
        #endif
        return;
    }

    //set hostname
    mdns_hostname_set(hostname);
    //set default instance
    mdns_instance_name_set(hostname);
    #if USB_MODE == 1
        String msg="MDNS Started with hostname: ";
        msg+=hostname;
        Serial.println(msg);
    #endif

}

void wifi_init(void)
{
#if APMODE
    WiFi.mode(WIFI_AP);
    //WiFi.softAP(AP_name, AP_pass);
    //WiFi.softAP(ssid, password(8-63chars), channel(def=1), hidden(def=false), max_connection(def=4))
    WiFi.softAP(AP_name, AP_pass, 1, false, MAX_WIFI_CLIENTS);
    // MAX_WIFI_CLIENTS est en fait le nombre de stations
    // softAPgetStationNum() retourne le nombre de stations connectees //UU: utiliser
    // nouvelle interface ? WiFi.softAP(ssid, password, channel, hidden)
  
    IPAddress myIP = WiFi.softAPIP();
#else
    IPAddress myIP IPADDRESS;
    IPAddress gateway GATEWAY;
    IPAddress subnet SUBNET;
    WiFi.config(myIP, gateway, subnet);
    WiFi.begin(STA_name, STA_pass);
    WiFi.mode(WIFI_STA);
#endif
    #if USB_MODE == 1
        Serial.print("D17 IP address: "); Serial.println(myIP);
    #endif
    start_mdns_service();
}

#endif



//--------------------------------------------------------------------------------
// TCP/IP server part
//----------------------------------------------------------------------------------

#if USE_WIFI == 1
#if USE_TCP == 1

WiFiServer tcpServer(tcp_port);  //UU: essayer plusieurs serveurs sur des ports differents suivant les protocols
WiFiClient zeclients[MAX_TCP_CLIENTS];
byte wait_id_msg[MAX_TCP_CLIENTS];

void tcpServerInit(void)
{    
    tcpServer.begin();
    
    for(byte i = 0; i < MAX_TCP_CLIENTS; i++) wait_id_msg[i] = 0;
}


void parse_tcp_clients(void)
{
    byte i;
    char rx[120] = "";


    for(i = 0; i < MAX_TCP_CLIENTS; i++)
    {
        // Client comes
        if(!zeclients[i])
        {
            zeclients[i] = tcpServer.available();
            if(zeclients[i])
            {
                #if USB_MODE == 1
                     Serial.print("[T"); Serial.print(i); Serial.println("] Connected");
                     // possibilite d'utiliser zeclients[i].remoteIP pour detecter s'il sagit de la meme souris
                #endif
                zeclients[i].print("d17 v1\n"); wait_id_msg[i] = 1;
            }
        }

        // No Client
        if(!zeclients[i]) continue;

        // Client leaves
        if(!zeclients[i].connected())
        {
            #if USB_MODE == 1
                Serial.print("[T"); Serial.print(i); Serial.println("] Disconnected");
            #endif
            zeclients[i].stop();            
            //zeclients[i] = NULL;  //UU: essayer
            
            for(byte j = 0; j < 4; j++) souris_adr[j][0]=0; // liberartion (surtout pour ANA)
            
            continue;
        }

        // Client sends data
        while(zeclients[i].available())
        {
            #if USB_MODE == 1
                Serial.print("[T"); Serial.print(i); Serial.print("] rx:");
            #endif

            String line = zeclients[i].readStringUntil('\n');
            line.toCharArray(rx, 119); //UU: on peut gagner cette copie avec .c_str
            rx[119] = '\0';
            #if USB_MODE == 1
                /**/ Serial.println(rx);
            #endif

            yield();

            if(wait_id_msg[i]) // remove the welcome message
            {
                wait_id_msg[i] = 0;
            }
            else // parse the command message
            {
                parse_rx(i, rx);
                if(status_txed[i] == 2)
                {
                    // envoie du message de reponse
                    zeclients[i].print(status_msg);
                    status_txed[i] = 1;
                }
            }

            yield();
            #if USB_MODE == 1
                //Serial.print(".");
            #endif
        }
        //client.flush();
    }

    delay(0);
}

#endif
#endif



//--------------------------------------------------------------------------------
// USB DCC++ protocol part
//--------------------------------------------------------------------------------
/*
  DCC++ protocol  (uniquement les cmds, pas les cfg, list, prg ...)

    <0> -> <p0>   PWR OFF
    <1> -> <p1>   PWR ON

    <t slot adr vit sens(1=av)> -> <T slot vit sens>
    <f adr byte1 [byte2]> -> pas de return
        128+1(F1)+2(F2)+4(F3)+8(F4)+16(F0)
        176+1(F5)+2(F6)+4(F7)+8(F8)
        160+1(F9)+2(F10)+4(F11)+8(F12)
        222   byte2:1(F13)....128(F20)
        223   byte2:1(2F1)....128(F28)

    <a adr0_511 sub0_3 on0_1> -> pas de return
    <T ID 0=dir/1=dev> -> <H ID pos>
    <Z ID on> -> pas de return

    <Q> -> toutes les entrees <Q ID> low ou <q ID> high
    en temps normal seulement transmision des changements a chaque bcl
    <c> -> <a0-1023>
*/

#if (USB_MODE == 2) || (USB_MODE == 5)

int acc_id;
int acc_adr;
byte acc_pair;

byte serial_state = 0;
char serial_cmd[20];
byte serial_ind = 0;

void dccpp_in_send_all(void)
{
    byte i, j, m;

    j = 0; m = 1;
    for(i = 0; i < DCCPP_SENSOR_NB; i++)
    {
        if(io_in[j] & m) Serial.print("<Q "); else Serial.print("<q ");
        Serial.print(i); Serial.print(">");

        m = m << 1; if(m == 0) { m = 1; j++; }
    }
}

void dccpp_in_send_diff(void)
{
    byte i, j, m, in;

    j = 0; m = 1; in = io_in[0] ^ io_in_old[0];
    for(i = 0; i < DCCPP_SENSOR_NB; i++)
    {
        if(in & m)
        {
            if(io_in[j] & m) Serial.print("<Q "); else Serial.print("<q ");
            Serial.print(i); Serial.print(">");
        }

        m = m << 1;
        if(m == 0)
        {
            m = 1; j++;
            in = io_in[j] ^ io_in_old[j];
        }
    }

}


byte cdm_nb_mod_s88_8bits = 0;
char cdm_old_s88_reply[(IO_IN_NB / 4) + 5] = "";

// exemple de retour:
//      <y 0000>");    // LSB
//      <y 5963>");    // det0=0 puis 1 0 1   1001 0110 1100
//      <y 5963124884FFFFFFFFFFFFFF>");    // mettre l'hexa en majuscule
void cdm_in_send_diff(void)
{
    if(cdm_nb_mod_s88_8bits == 0) return;

    char reply[(IO_IN_NB / 8) + 5];
    reply[0] = '\0';

    strcat(reply, "<y ");
    byte j = 3;
    for(byte i = 0; i < cdm_nb_mod_s88_8bits; i++)
    {
        byte in8 = io_inmem_usb[i];
        sprintf(&reply[j], "%X", in8 & 0x0f);
        in8 = in8 >> 4;
        j++;
        sprintf(&reply[j], "%X", in8 & 0x0f);
        j++;
        io_inmem_usb[i] = 0;
    }
    strcat(reply, ">");
    if(strcmp(reply, cdm_old_s88_reply) != 0)   // send only if diff
    {
        Serial.print(reply);
        strcpy(cdm_old_s88_reply, reply);
    }
}


byte dccpp_get_slot(unsigned int adr)
{
    byte slot = get_slot_for_adr(adr);
    if(slot == 99)
    {
        slot = get_free_or_oldest_slot();
        slot_ch[slot].adr = adr;
        slot_ch[slot].sens = 1; // 1=AV                       //UU: voir si on initialise ou pas
        slot_ch[slot].vit = 0;                                //UU: voir si on demande a mettre a jours la loco avec les modified=1
        for(byte j = 0; j < 29; j++) slot_ch[slot].fct[j] = 0;
    }
    set_first_slot(slot);

    return slot;
}
void dccpp_set_sens_vit(unsigned int adr, byte sens, byte vit)
{
    byte slot = dccpp_get_slot(adr);
    slot_ch[slot].sens = sens;
    slot_ch[slot].vit = vit;
}


void dccpp_process(void)
{
    byte i, nb, nbi, num, slot;
    unsigned int dat[4];
    unsigned char c;
    unsigned char adr;

    // extraire tous les nombres
    nb = 0; nbi = 0; num = 0;
    for(i = 0; i < serial_ind; i++)
    {
        c = serial_cmd[i];
        if(c >= '0' && c <= '9') { nbi = 10 * nbi + c - '0'; num = 1; }
        else
        {
            if(num) { num=0; if(nb<4) dat[nb]=nbi; nb++; nbi=0; }
        }
    }
    if(num) { num = 0; if(nb<4) dat[nb] = nbi; nb++; nbi = 0; }

    // traiter la commande

    c = serial_cmd[0];

    if(c == '0') { au = 1; Serial.print("<p0>"); }

    else if(c == '1') { au = 0; Serial.print("<p1>"); }

    else if(c == 'c') { Serial.print("<a"); Serial.print(an); Serial.print(">"); }

    #if USB_MODE == 2
        else if(c == 's') { Serial.print("<p"); Serial.print(au^1); Serial.print("><iDCC++ D17 20180222><N0: SERIAL><X><X>"); }
    #endif
    #if USB_MODE == 5
        else if(c == 's') { Serial.print("<p"); Serial.print(au^1); Serial.println("><iDCC++ BASE STATION FOR ARDUINO UNO / ARDUINO MOTOR SHIELD: V-1.2.1+S88.1 / Mar  1 2019 19:28:46><N0: SERIAL><X><X>"); }
    #endif

    else if(c == 'Q') { dccpp_in_send_all();}

    else if(c == 'S') { Serial.print("<O>"); }

    else if(c == 't' && nb == 4)
    {
      // dat[0] = numéro du slot dccpp non utilisé (le numero du slot DCC++ commence a 1, le 0 n'est que pour les actions non bouclees ...)
      // dat[1] = adr
      // dat[2] = vitesse
      // dat[3] = sens
      adr = dat[1];
      byte vit = dat[2];
      byte sens= dat[3];
      if(adr != ADR_ANA)
      if(adr >= 1 && adr <= 10239)
      {
          // on suppose que les commandes sont envoyees en mode 28 crans. Chnager si ce n'est pas le cas
          if(vit > DCCPP_MAX_CRAN) vit = DCCPP_MAX_CRAN;
          #if USER_USE_126_CRANS == 0
              vit = map(vit, 0, DCCPP_MAX_CRAN, 0, 28);
          #endif
          #if USER_USE_126_CRANS == 1
              vit = map(vit, 0, DCCPP_MAX_CRAN, 0, 126);
          #endif

          dccpp_set_sens_vit(adr, sens, vit);   // la vitesse doit etre transmise avec le bon format ex: 0-28 en mode 28 crans ou 0-126 en mode 126 crans
      }
      Serial.print("<T "); Serial.print(dat[0]); Serial.print(" "); Serial.print(dat[2]); Serial.print(" "); Serial.print(dat[3]); Serial.print(">");
    }

    /* en mode DCC++, les fonctions ne sont transmisent qu'une seule fois ici a la reception de la commande*/
    else if(c == 'f')
    {
        adr = dat[0];
        if(adr != ADR_ANA)
        if(adr >= 1 && adr <= 10239)
        {
            if(nb == 2) // 1 byte de cmd pour F0-F4, F5-F8, F9-F12
            {
                if(adr <= 127) dcc_tx2(16, adr, dat[1]);
                else           dcc_tx3(16, 0xC0 + highByte(adr), lowByte(adr), dat[1]);
            }
            if(nb == 3) // 2 bytes de cmd pour F13-F20, F21-F28
            {
                if(adr <= 127) dcc_tx3(16, adr, dat[1], dat[2]);
                else           dcc_tx4(16, 0xC0 + highByte(adr), lowByte(adr), dat[1], dat[2]);
            }
        }
    }

    else if(c == 'T')
    {
        #if USB_MODE == 2
            if(nb == 3)  // configure l'accessoire ID avec adr et pair
            {
                acc_id = dat[0];
                acc_adr = dat[1];
                acc_pair = dat[2];
                Serial.print("<O>");
            }
            if(nb == 2 && acc_id == dat[0]) // commande l'accessoire ID (A chaque fois avec RocRail on a une configuration suivit de la commande)
            {
                user_dccpp_acc(acc_adr, acc_pair, dat[1]);
                Serial.print("<H "); Serial.print(dat[0]); Serial.print(" "); Serial.print(dat[1]); Serial.print(">");
            }
        #endif
        #if USB_MODE == 5
            if(nb == 2)
            {
                unsigned int acc_1_2048 = dat[0];
                byte val0_1 = 0; if(dat[1]) val0_1 = 1;
                if((acc_1_2048 >= 1) && (acc_1_2048 <= 2048)) user_cdm_acc(acc_1_2048, val0_1);
                Serial.print("<H "); Serial.print(dat[0]); Serial.print(" "); Serial.print(dat[1]); Serial.print(">");
            }
        #endif
    }

    // Y est une nouvelle commande ajoutee a DCC++ pour lire les entrees du bus S88
    else if(c == 'Y')   // <Y 2 1> 2x8 1=hexa (only hex mode is supported for CDM-Rail)
    {
        if(nb == 2)
        {
            if(dat[1] == 1)
            {
                cdm_nb_mod_s88_8bits = dat[0];
                if(cdm_nb_mod_s88_8bits > (IO_IN_NB / 8)) cdm_nb_mod_s88_8bits = IO_IN_NB / 8;
                Serial.print("<o "); Serial.print(cdm_nb_mod_s88_8bits); Serial.print("*8 1>");
            }
        }
    }
}


void parse_usb_dccpp(void)
{
    int rcmd;
    unsigned char cmd;

    while(Serial.available())
    {
        rcmd = Serial.read();
        if(rcmd < 0) return;
        cmd = (unsigned char)rcmd;

        if(cmd == '<') { serial_state = 1; serial_ind = 0; continue;}
        if(serial_state == 0) continue;
        if(cmd == '\n' || cmd == '\r') continue;
        if(cmd == '>') { serial_cmd[serial_ind] = '\0'; dccpp_process(); serial_state = 0; continue; }

        serial_cmd[serial_ind] = cmd;
        serial_ind++;
        if(serial_ind == 20) serial_state = 0;
    }
}

#endif



#if USB_MODE == 3

byte ds_get_slot(unsigned int adr)
{
    byte slot = get_slot_for_adr(adr);
    if(slot == 99)
    {
        slot = get_free_or_oldest_slot();
        slot_ch[slot].adr = adr;
        slot_ch[slot].sens = 1; // 1=AV                       //UU: voir si on initialise ou pas
        slot_ch[slot].vit = 0;                                //UU: voir si on demande a mettre a jours la loco avec les modified=1
        for(byte j = 0; j < 29; j++) slot_ch[slot].fct[j] = 0;
    }
    set_first_slot(slot);

    return slot;
}
void ds_set_sens(unsigned int adr, byte sens)
{
    byte slot = ds_get_slot(adr);
    slot_ch[slot].sens = sens;
}
void ds_set_vit(unsigned int adr, byte vit)
{
    byte slot = ds_get_slot(adr);
    slot_ch[slot].vit = vit;
}
// FCT 0-4  : 1111111111 0 0AAAAAAA 0 100-FL-F4-F3-F2-F1  0 EEEEEEEE 1
// FCT 5-8  : 1111111111 0 0AAAAAAA 0 1011-F8-F7-F6-F5    0 EEEEEEEE 1
// FCT 9-12 : 1111111111 0 0AAAAAAA 0 1010-F12-F11-F10-F9 0 EEEEEEEE 1
// FCT 13-20: 1111111111 0 0AAAAAAA 0 11011110            0 F20-F19-F18-F17-F16-F15-F14-F13 0 EEEEEEEE 1
// FCT 11-28: 1111111111 0 0AAAAAAA 0 11011111            0 F28-F27-F26-F25-F24-F23-F22-F21 0 EEEEEEEE 1
void ds_set_fct(unsigned int adr, byte fct, byte val)
{
    byte slot = ds_get_slot(adr);
    dcc_set_fct(slot, fct, val);
}
/*
    if(val) dcc_ch2[i].fct |=  (1 << fct);    //UU: si un jour, on passe en unsigned long
    else    dcc_ch2[i].fct &= ~(1 << fct);

    unsigned long fctl = dcc_ch2[i].fct;
    if(fct>=0 && fct<=4)
    {
        byte tx = 0x80 | ((byte)(fctl>>1)&0x0f);
        if(fctl & (1<<0)) tx |= 16;
        dcc_tx2(16, adr, tx);
        yield();
    }
    if(fct>=5 && fct<=8)
    {
        byte tx = 0xB0 | ((byte)(fctl>>5)&0x0f);
        dcc_tx2(16, adr, tx);
        yield();
    }
    if(fct>=9 && fct<=12)
    {
        byte tx = 0xA0 | ((byte)(fctl>>9)&0x0f);
        dcc_tx2(16, adr, tx);
        yield();
    }
    if(fct>=13 && fct<=20)
    {
        byte tx = ((byte)(fctl>>13)&0xff);
        dcc_tx3(16, adr, 0xDE, tx);
        yield();
    }
    if(fct>=21 && fct<=28)
    {
        byte tx = ((byte)(fctl>>21)&0xff);
        dcc_tx3(16, adr, 0xDF, tx);
        yield();
    }
    */

// --> 100 Ready\n
// setPing()\n --> @200 Ok\n
// setPower(1)\n --> 200 Ok\n
// setLocoSpeed(49174,0,0) --> 200 Ok\n               //0xC000 + DCC ADR, 0-1023, 0=DCC28 2=DCC126
// setLocoDirection(49174,1) --> 200 Ok\n             //1=AV 2=AR
// setLocoFunction(49174,9,1) --> 200 Ok\n            //F9=1  F0-F28 supported
// setTurnout(14336,1) --> 200 Ok\n                   //Pair nb 1=0x3800 2044=0x3ffc
// getS88(2)\n --> @S88,0-65535,0-65535\n200 Ok\n     //max 32 modules de 16
// setLocoConfig(0-49152,0-1023,0-255)\n --> 200 Ok\n //adr

char ds_serial_cmd[100];
byte ds_serial_ind = 0;

byte ds_arg_nb = 0;
unsigned int ds_arg[5] = { 0, 0, 0, 0, 0 };

void desktop_station_decompose(void)
{
     byte i = 0;
     ds_arg_nb = 0;
     unsigned int v = 0;

     while(char c = ds_serial_cmd[i++])
     {
         if(c >= '0' && c <= '9') v = 10 * v + c - '0';
         if(c == ',' || c == ')') if(ds_arg_nb <= 4) { ds_arg[ds_arg_nb] = v; ds_arg_nb++; }
         if(c == '(' || c == ',') v = 0;
     }
}
byte firstPing = 1;
void desktop_station_process(void)
{
    desktop_station_decompose();
    if(!strncmp(ds_serial_cmd, "setPower", 8) && ds_arg_nb == 1)
    {
        if(ds_arg[0]==1) au = 0; else au = 1;
    }
    else if(!strncmp(ds_serial_cmd, "setLocoDirection", 16) && ds_arg_nb == 2)
    {
        unsigned int adr = ds_arg[0] - 0xC000;
        if(adr != ADR_ANA)
        if(adr >= 1 && adr <= 10239)
        {
            if(ds_arg[1] == 1) ds_set_sens(adr, 1); // AV
            if(ds_arg[1] == 2) ds_set_sens(adr, 0); // AR
        }
    }
    else if(!strncmp(ds_serial_cmd, "setLocoSpeed", 12) && ds_arg_nb == 3)
    {
        unsigned int adr = ds_arg[0] - 0xC000;
        if(adr != ADR_ANA)
        if(adr >= 1 && adr <= 10239)
        {
            #if USER_USE_126_CRANS == 0
                ds_set_vit(adr, map(ds_arg[1], 0, 1023, 0, 28));
            #endif
            #if USER_USE_126_CRANS == 1
                ds_set_vit(adr, map(ds_arg[1], 0, 1023, 0, 126));
            #endif
        }
    }
    else if(!strncmp(ds_serial_cmd, "setLocoFunction", 15) && ds_arg_nb == 3)
    {
        unsigned int adr = ds_arg[0] - 0xC000;
        if(adr != ADR_ANA)
        if(adr >= 1 && adr <= 10239)
        {
             ds_set_fct(adr, ds_arg[1], ds_arg[2]);   // adr, fct_0_28, val_0_1
        }
    }
    else if(!strncmp(ds_serial_cmd, "setLocoConfig", 13) && ds_arg_nb == 3)
    {
        // sequence de programmation dcc
        led_rgb_sign_of_life(0xfff000); //LED ON Yellow
        // programing seqeunce
        prog_cv(ds_arg[1], ds_arg[2]);  // cv_adr, cv_dat
        // disable booster during 1s to reboot decoder
        digitalWrite(P_PIN, LOW);
        digitalWrite(S_PIN, LOW);
        led_rgb_sign_of_life(0);
        delay(1000); //UU: possible to check with can
        digitalWrite(P_PIN, HIGH);
    }
    else if(!strncmp(ds_serial_cmd, "setTurnout", 10) && ds_arg_nb == 2)
    {
        unsigned int acc_1_320 = ds_arg[0] - 0x3800;
        byte val0_1 = 0; if(ds_arg[1]) val0_1 = 1;

        if((acc_1_320 >= 1) && (acc_1_320 <= 320)) user_ds_acc(acc_1_320, val0_1);
    }
    else if(!strncmp(ds_serial_cmd, "getS88", 6) && ds_arg_nb == 1)
    {
        byte nb = ds_arg[0];
        Serial.print("@S88,");
        for(byte i = 0; i < nb; i++)
        {
            unsigned int in16;
            in16 = 0;
            if(i < (IO_IN_NB / 16))   //6x16=96 entrees max
            {
                //in16 = (io_in[2*i+1] << 8) | io_in[2*i];          // sans memorisation
                in16 = (io_inmem_usb[2*i+1] << 8) | io_inmem_usb[2*i];  // avec memorisation pour
                io_inmem_usb[2*i+1] = io_inmem_usb[2*i] = 0;            // ne pas louper les impulsions
            }
            Serial.print(in16, HEX); Serial.print(",");
        }
        Serial.println("");
    }
    else if(!strncmp(ds_serial_cmd, "setPing", 7))
    {
        if(firstPing) { firstPing--; Serial.print("300 Command error\n"); return; }
        Serial.println("@DSG,01,00,00,02,00,00,00,00");
    }

    Serial.print("200 Ok\n");
}

void parse_usb_desktop_station(void)
{
    int rcmd;
    unsigned char cmd;

    while(Serial.available())
    {
        rcmd = Serial.read();
        if(rcmd < 0) return;
        cmd = (unsigned char)rcmd;
        if(cmd == '\r') continue;
        if(cmd == '\t') continue;
        if(cmd == ' ') continue;
        if(cmd == '\n') { ds_serial_cmd[ds_serial_ind] = '\0'; desktop_station_process(); ds_serial_ind = 0; continue; }
        else { ds_serial_cmd[ds_serial_ind] = cmd; if(ds_serial_ind < 98) ds_serial_ind++; }
    }
}

#endif



//--------------------------------------------------------------------------------
// HTTP
//----------------------------------------------------------------------------------

#if USE_WIFI == 1
#if USE_HTTP == 1

WebServer httpServer(80); // Create a webserver object that listens for HTTP request on port 80
WebSocketsServer webSocketServer = WebSocketsServer(81); // Create a webSocketServer object that listens for request on port 81
FS* filesystem;   // on peut aussi s'en passer et utiliser FS.

#define DBG Serial

void log(char* str)
{
    #if USB_MODE == 1
    DBG.print(str);
    #endif
}


// A. SPIFFS & SDCARD

// Listing directory. 
void listDir(const char *dirname, uint8_t levels) {

    if (filesystem==&SD)
    {
      DBG.println("File System Type: SD");
    }
    else if (filesystem==&SPIFFS)
    {
      DBG.println("File System Type: SPIFFS");
    }
    else
    {
      DBG.println("File System Type: UNKNOWN");
    }

    DBG.printf("Listing directory: %s\n", dirname);

    File root = filesystem->open(dirname);
    if (!root) {
        DBG.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        DBG.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            DBG.print("  DIR : ");
            DBG.println(file.name());
            if (levels) {
                listDir(file.name(), levels - 1);
            }
        } else {
            DBG.print("  FILE: ");
            DBG.print(file.name());
            DBG.print("  SIZE: ");
            DBG.println(file.size());
        }
        file = root.openNextFile();
    }
}

String formatBytes(size_t bytes)
{
  if (bytes < 1024) return String(bytes) + "B";
  else if (bytes < (1024 * 1024))  return String(bytes / 1024.0) + "KB";
  else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + "MB";
  else return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
}

bool startSDCard()
{
  SPI.begin(23, 33, 19, -1);
  if (!SD.begin(-1, SPI, 10000000)) {
      log("SD Card Mount Failed\n");
      return false;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
      log("No SD card attached\n");
      return false;
  }
  log("SDCard detected.\n");
  filesystem = &SD;

  return true;
}

void startSPIFFS(void) { // Start the SPIFFS and list all contents
  SPIFFS.begin(); // Start the SPI Flash File System (SPIFFS)
  log("SPIFFS started.\n");
  filesystem = &SPIFFS;
}

void startFS()
{
  if (!startSDCard())         // Detect if SDCard is present
  {
    startSPIFFS();
  }

  #if USB_MODE == 1
  log("Files:\n");
  {
    listDir("/", 0);
  }
  #endif  
}

// B. Websocket
#define WStype_t byte
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
    switch(type) 
    {
        case WStype_DISCONNECTED:
            #if USB_MODE == 1
            DBG.printf("[W%u] Disconnected\n", num);
            for(byte i = 0; i < 4; i++) souris_websocket_adr[i][0]=0; // liberartion (surtout pour ANA)
            #endif
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocketServer.remoteIP(num);
                #if USB_MODE == 1
                DBG.printf("[W%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                #endif
                webSocketServer.sendTXT(num, "D17 Welcome !");
            }
            break;
        case WStype_TEXT:
            #if USB_MODE == 1
            DBG.printf("[W%u] rx: %s\n", num, payload);
            #endif
            parse_rx(num, (char *)payload, 1);  //tx[] updated if reply needed
            if(status_websocket_txed[num] == 2)
            {                
                // envoie du message de reponse
                status_msg[STATUS_EXT]=status_msg[STATUS_EXT+1]=' '; // ajout des extensions               
                webSocketServer.sendTXT(num, status_msg);
                status_msg[STATUS_EXT]='\n';status_msg[STATUS_EXT+1]='\0'; // retrait des extensions                
                status_websocket_txed[num] = 1;
            }                        
            break;
        case WStype_BIN:
            #if USB_MODE == 1
            DBG.printf("[W%u] rx bin(%u)\n", num, length);
            #endif
            //hexdump(payload, length);
            // send message to client
            // webSocketServer.sendBIN(num, payload, length);
            break;
        case WStype_ERROR:      
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
            break;
    }
}

// Start the WebSocket server
void startWebSocket(void) 
{ 
  webSocketServer.begin();                          // start the websocket server
  webSocketServer.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  log("WebSocket server started.\n");
}


// C. HTTP

// ex si page HTML en static

const char ok_au1[] PROGMEM = R"rawliteral(
<html><body>PWR is OFF (AU=1)<br><br><a href="index.html">index</a><br><br><a href="admin.html">admin</a><br><br><a href="au0">PWR_ON (AU=0)</a></body></html>
)rawliteral";

const char ok_au0[] PROGMEM = R"rawliteral(
<html><body>PWR is ON (AU=0)<br><br><a href="au1">PWR_OFF (=AU)</a></body></html>
)rawliteral";

const char ask_go_au[] PROGMEM = R"rawliteral(
<html><body>NO HTML IN PWR<br><a href="au1">PWR_OFF (=AU)</a></body></html>
)rawliteral";

// convert the file extension to the MIME type
String getContentType(String filename) 
{ 
    if (filename.endsWith(".html"))     return "text/html";
    else if (filename.endsWith(".htm")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js"))  return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".png")) return"image/png";
    else if (filename.endsWith(".gif")) return "image/gif";
    else if (filename.endsWith(".jpg")) return "image/jpeg";
    else if (filename.endsWith(".xml")) return "text/xml";
    else if (filename.endsWith(".pdf")) return "application/x-pdf";
    else if (filename.endsWith(".zip")) return "application/x-zip";
    else if (filename.endsWith(".gz"))  return "application/x-gzip";
    return "text/plain";
}

void no_html_in_pwr(void)
{
    #if USB_MODE == 1
    DBG.println("no html in AU0");
    #endif
    httpServer.send(200, "text/html", ask_go_au);
}

// send the right file to the client (if it exists)
bool handleFileRead(String path)
{
    if(au == 0) // handle only in AU mode
    {
        no_html_in_pwr();
        return true;
    }
    
    #if USB_MODE == 1
    DBG.println("handleFileRead: " + path);
    #endif

    if (path.endsWith(".del"))   // .del quick workaround to delete a file
    {
          path.replace(".del", "");
          #if USB_MODE == 1
          DBG.println("handleFileDelete: " + path);
          #endif
          if (path == "/") return 0;
          if (!filesystem->exists(path)) return 0;  
          filesystem->remove(path);
          httpServer.send(200, "text/plain", "");
          return 1;
    }
    
    if (path.endsWith("/")) path += "index.html"; // If a folder is requested, send the index file
    String contentType = getContentType(path); // Get the MIME type

    if (filesystem->exists(path)) // If the file exists
    { 
        File file = filesystem->open(path, "r"); // Open it
        size_t sent = httpServer.streamFile(file, contentType); // And send it to the client
        file.close(); // Then close the file again
        return true;
    }
    DBG.println("\tFile Not Found");
    return false; // If the file doesn't exist, return false
}
File fsUploadFile; 
// upload a new file to the SPIFFS/SD
void handleFileUpload() 
{
       // File object to temporarily store the received file  (so no // upload)
    if(au == 0)  // handle only in AU mode
    { 
        #if USB_MODE == 1
        DBG.println("no html in AU0");
        #endif
        httpServer.send(500, "text/plain", "500: couldn't create file");
        return;
    }
    
    HTTPUpload& upload = httpServer.upload();
    if(upload.status == UPLOAD_FILE_START)
    {
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/"+filename;
        #if USB_MODE == 1
        DBG.print("handleFileUpload Name: "); DBG.println(filename);
        #endif
        fsUploadFile = filesystem->open(filename, "w"); // Open the file for writing in FS (create if it doesn't exist)                
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
        #if USB_MODE == 1
        DBG.print("=");
        #endif

    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        #if USB_MODE == 1
        if(fsUploadFile)
        {
          DBG.println("> OK");
        }
        else
        {
          DBG.println("> KO");
        }        
        #endif

        if(fsUploadFile) // If the file was successfully created
        {
            fsUploadFile.close();
            #if USB_MODE == 1
            DBG.print("handleFileUpload Size: "); DBG.println(upload.totalSize);
            #endif
            httpServer.sendHeader("Location","/success.html"); // Redirect the client to the success page
            httpServer.send(303);
        }
        else
        {
            httpServer.send(500, "text/plain", "500: couldn't create file");
        }
    }
}

void handleFileDelete()
{
  //if(au == 0) return; // handle only in AU mode (already tested in handleFileRead)
    
  if (httpServer.args() == 0) return httpServer.send(500, "text/plain", "BAD ARGS");
  
  String path = httpServer.arg(0);
  #if USB_MODE == 1
  DBG.println("handleFileDelete: " + path);
  #endif
  if (path == "/") return httpServer.send(500, "text/plain", "BAD PATH");
  if (!filesystem->exists(path)) return httpServer.send(404, "text/plain", "FileNotFound");  
  filesystem->remove(path);
  httpServer.send(200, "text/plain", "");
  path = String();  //UU: why ? maybe string not deleted from memory, to be checked
}

void replyFileList()  // OLD
{   
    if(au == 0)  // handle only in AU mode
    { 
        no_html_in_pwr();
        return;
    }

    String rsp = "<html><head>Files</head><body><h2>FS Files</h2><br>";

    File dir = filesystem->open("/");
    File file = dir.openNextFile();
    while (file) {
      String fileName = dir.name();
      size_t fileSize = dir.size();      
      rsp += "FS File " + fileName + ", size " + formatBytes(fileSize) + "<BR>";
      fileName = String(); 
      File file = dir.openNextFile();
    }

    rsp += "</body></html>";
    #if USB_MODE == 1
    DBG.println("reply: " + rsp);
    #endif

    httpServer.send(200, "text/html", rsp);
    
    rsp = String(); 
}

void replyFileList2()   // ACTUAL but need to limit string size ...
{
    if(au == 0)  // handle only in AU mode
    { 
        no_html_in_pwr();
        //httpServer.send(404, "text/plain", "404: Not Found");
        return;
    }
    String rsp = "<html><head>Files</head><body><h2>FS Files</h2><br><ul>";

    File dir = filesystem->open("/");
    File file = dir.openNextFile();
    String host = hostname;
    while (file) 
    {
      String fileName = dir.name();
      size_t fileSize = dir.size();      
      rsp += "<li><a href=\"http://" + host + ".local/" + fileName + "\">" + fileName + ", size " + formatBytes(fileSize) + "</a></li>";
      fileName = String(); 
      File file = dir.openNextFile();
    }
    rsp += "</ul>";
    
    /*
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    float fileTotalKB = (float)fs_info.totalBytes / 1024.0; 
    float fileUsedKB = (float)fs_info.usedBytes / 1024.0; 
    rsp += "Total KB: " + String(fileTotalKB,0) + " KB<BR>";
    rsp += " Used KB: " + String(fileUsedKB,0) + " KB<BR>";
    */
    rsp += "</body></html>";
    #if USB_MODE == 1
    DBG.println("reply: " + rsp);
    #endif

    httpServer.send(200, "text/html", rsp);
    
    rsp = String(); 
}

void handleFileList()  // try the JSON ...
{
    if(au == 0)  // handle only in AU mode
    { 
        no_html_in_pwr();
        return;
    }

  if (!httpServer.hasArg("dir")) { httpServer.send(500, "text/plain", "BAD ARGS"); return; }

  String path = httpServer.arg("dir");
  #if USB_MODE == 1
  DBG.println("handleFileList: " + path);
  #endif
  File dir = filesystem->open(path);
  File file = dir.openNextFile();
  path = String();

  String output = "[";
  while (file)
  {
    if (output != "[") output += ',';
  
    output += "{\"type\":\"";
    output += "file";
    output += "\",\"name\":\"";
    output += file.name();
    output += "\"}";

    file = dir.openNextFile();
  }

  output += "]";
  httpServer.send(200, "text/json", output);
  output = String();
}

void startWebServer()
{
    // 1. map default requests
    httpServer.onNotFound([]()  
    {   
        if (!handleFileRead(httpServer.uri())) // send file corresponding to URI if it exists in SPIFFS
        httpServer.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });
    
    // 2. map specific requests (with remaping if needed)
    
    // Icon qui s'affiche dans les onglets du browser internet
    httpServer.on("/favicon.ico", [](){ handleFileRead("/carre_vert.png"); });

    // Racourcit pour la souris simple
    // httpServer.on("/sou1", [](){ handleFileRead("/souris1.html"); });

    // List the SPIFFS content    
    httpServer.on("/list", HTTP_GET, replyFileList2);
    //httpServer.on("/list", HTTP_GET, handleFileList); //essyer avec action dans le html

    // Upload request
    // if the client posts to the upload page
    // Send status 200 (OK) to tell the client we are ready to receive
    // Receive and save the file
    httpServer.on("/upload.html", HTTP_POST, []() { httpServer.send(200, "text/plain", ""); }, handleFileUpload);
    
    // Delete request (not working) --> workaround: add .del to the file    
    // Edit request (on va pas faire) --> workaround: afficher le fichier, le sauver puis le loader
    //httpServer.on("/edit.html", HTTP_DELETE, handleFileDelete);
    
    // Get heap status, analog input value and all GPIO statuses in one json call
    httpServer.on("/stat", HTTP_GET, []() 
    {
        String json = "{";
        json += "\"vers\":" + String(CENTRAL_NAME);
        json += "\"heap\":" + String(ESP.getFreeHeap());
        json += ", \"analog\":" + String(analogRead(ANALOG_SENSOR));
        //json += ", \"gpio\":" + String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
        json += ", \"au\":" + String(au);
        json += "}";
        httpServer.send(200, "text/json", json);
        json = String();
    });
    
    // Direct AU
    httpServer.on("/au0", HTTP_GET, [](){ au = 0; httpServer.send(200, "text/html", ok_au0);});  // on retourne pas trop long pour ne pas perturber le dcc et pour ne pas bloquer le navigateur
    httpServer.on("/au1", HTTP_GET, [](){ au = 1; httpServer.send(200, "text/html", ok_au1);});
    
    httpServer.begin(); // start the server
    #if USB_MODE == 1
    //DBG.print("HTTP server started on Core ");
    //DBG.println(xPortGetCoreID());
    #endif
}



// D. setup / loop

void startWebServerOnCore( void * pvParameters ){
  startFS();
  startWebServer();

  for(;;){
    httpServer.handleClient();
    delay(2);
  } 
}

TaskHandle_t Task1;

void http_setup(void)
{
   //startFS();  
   startWebSocket();
   //startWebServer();
  xTaskCreatePinnedToCore(
                  startWebServerOnCore,     // Task function.
                  "WebServer",              // name of task. 
                  10000,                    // Stack size of task 
                  NULL,                     // parameter of the task 
                  1,                        // priority of the task 
                  &Task1,                   // Task handle to keep track of created task
                  (((int)xPortGetCoreID() + 1) % 2));  // Running on a different Core 
}

void http_loop(void)
{
    //httpServer.handleClient();
    webSocketServer.loop();
}

#endif
#endif




//--------------------------------------------------------------------------------
// MAIN
//----------------------------------------------------------------------------------

void check_au_pin(void)
{
#if USER_USE_AU == 1
    byte state;

    state = digitalRead(AU_PIN);
    pinMode(AU_PIN, INPUT_PULLUP);
    delayMicroseconds(5);
    #if USER_AU_LEVEL == 0
        if(digitalRead(AU_PIN) == LOW) au = 1;
    #else
        if(digitalRead(AU_PIN) == HIGH) au = 1;
    #endif
    pinMode(AU_PIN, OUTPUT);
    digitalWrite(AU_PIN, state);
#endif
}

unsigned long time0;

void setup()
{
    M5.begin(true, false, true);
    delay(50);
    M5.dis.drawpix(0, 0xfff000);    // RGB LED Yellow at startup    
    
    // pins init
    pinMode(P_PIN,   OUTPUT); digitalWrite(P_PIN,   LOW);  // D2
    pinMode(S_PIN,   OUTPUT); digitalWrite(S_PIN,   LOW);  // D3
    // pinMode(LED_PIN, OUTPUT); digitalWrite(LED_PIN,HIGH);  // D4 LED active low, also LLED_PIN
    pinMode(DAT_PIN, OUTPUT); digitalWrite(DAT_PIN, LOW);  // D0
    pinMode(CLK_PIN, OUTPUT); digitalWrite(CLK_PIN, LOW);  // D5
    pinMode(LDIN_PIN,OUTPUT); digitalWrite(LDIN_PIN,LOW);  // D1
    pinMode(RST_PIN, OUTPUT); digitalWrite(RST_PIN, LOW);  // D8
    pinMode(SCK_PIN, OUTPUT); digitalWrite(SCK_PIN, LOW);  // D7
    pinMode(SDA_PIN, OUTPUT); digitalWrite(SDA_PIN, LOW);  // D6

    // some variables init
    slots_init();
    init_souris_adr();    

    // serial init
    Serial.begin(115200);
    #if USB_MODE == 1
        Serial.print("\n\n\nHello from ");
        strcpy(tx,CENTRAL_NAME); // passer par tx
        Serial.println(tx);
        //Serial.print(" on Core ");
        //Serial.println(xPortGetCoreID());
    #endif
    #if USB_MODE == 2
        Serial.print("<iDCC++ "); Serial.print(CENTRAL_NAME); Serial.print(" ***>");
    #endif
    #if USB_MODE == 5
        Serial.println("<p1><iDCC++ BASE STATION FOR ARDUINO UNO / ARDUINO MOTOR SHIELD: V-1.2.1+S88.1 / Mar  1 2019 19:28:46><N0: SERIAL><X><X>");
    #endif
    #if USB_MODE == 3
      Serial.println("--------------------------------------");
      Serial.println("Desktop Station Interface with Arduino");
      Serial.println("--------------------------------------");
      Serial.println("100 Ready");
    #endif


    #if USER_USE_MAX == 1
        led_init();
    #endif

    #if USER_USE_PCA == 1
        i2c_init();
        pca9685_init();
    #endif

    #if USER_USE_S88 == 1
        s88_init();
    #endif

    aig_init();
        
    user_init();

    #if USE_WIFI == 1
        wifi_init();
    
        #if USE_HTTP == 1
            http_setup();    
        #endif
        
        #if USE_TCP == 1
            tcpServerInit();
            #if USB_MODE == 1
            Serial.println("TCP server started. waiting clients ...");
            #endif
        #endif
        
    #endif

    time0 = millis();
}


byte wait_250ms = 0;
byte cpt_250ms = 0;

void loop()
{
    byte is_500ms;
    byte is_1s;
    byte is_2s;
    byte index = 0;
    unsigned long time1;

    // 1. GESTION DES CLIENTS

    // 1.a. GESTION DES CLIENTS TCP
    #if USE_WIFI == 1
    #if USE_TCP == 1
        parse_tcp_clients();
    #endif
    #endif

    // 1.b. GESTION DES CLIENTS HTTP et websockets
    #if USE_WIFI == 1
    #if USE_HTTP == 1
        http_loop();    
    #endif
    #endif

    // 1.c. GESTION DU CLIENT DCC++ USB
    #if (USB_MODE == 2) || (USB_MODE == 5)
        parse_usb_dccpp();
    #endif

    // 1.d. GESTION DU CLIENT DESKTOP STATION
    #if USB_MODE == 3
        parse_usb_desktop_station();
    #endif
    

    // 2. GENERATION DES SIGAUX DU BOOSTER
    check_au_pin(); // to update AU before booster update
    booster_maj();  // update one client (max 2 locos and functions)

    // 3. GESTION DU TEMPS (TOUTES LES 250ms) (uu: essayer de reduire a 125ms)
    time1 = millis();
    if((time1 - time0) < 250) return;
    //#if USB_MODE == 1
    //    if((time1 - time0) > 300) Serial.print("#");
    //    Serial.println(time1 - time0);
    //#endif
    time0=time1;

    // on arrive ici toute les 250ms
    cpt_250ms++;
    if((cpt_250ms & 1)==0) is_500ms = 1; else is_500ms = 0;   // 1x/2
    if((cpt_250ms & 3)==0) is_1s = 1; else is_1s = 0;         // 1x/4
    if((cpt_250ms & 7)==0) is_2s = 1; else is_2s = 0;         // 1x/8

    // 4. CLIGNOTEMENT DE LA LED (Sign of life)
    if(!au) // lent 0.5Hz
    {
        if(cpt_250ms & 4) 
        {
          led_rgb_sign_of_life(0x00ff00);  // GREEN
        }
        else 
        {
          led_rgb_sign_of_life(0);
        }
    }
    else // 2 flashs rapide de 2Hz
    {
        if((cpt_250ms & 1)&&(cpt_250ms & 4)) 
        {
          led_rgb_sign_of_life(0xff0000);     // RED
        }
        else 
        {
          led_rgb_sign_of_life(0);
        }
    }
    delay(0);

    // 5. LECTURE DU COURANT
    #if ANALOG_SENSOR != 0
      an = analogRead(ANALOG_SENSOR);
    #endif

    #if USB_MODE == 1
    #if DBG_1S == 1
        if(is_1s) { Serial.print("can="); Serial.println(an); }
    #endif
    #endif

    // 6. LECTURE DES ENTREES S88
    #if EN_ANALOG_DET_PULSE == 1
        analog_det_pulse();
    #endif
    #if USER_USE_S88
        s88_maj();   //4.8ms
        if(boost == BOOST_DCC) dcc_keepalive();
    #else
        simu_in_update();
    #endif
    delay(0);
    #if USB_MODE == 2
        dccpp_in_send_diff();
    #endif
    #if USB_MODE == 5
        cdm_in_send_diff();
    #endif

    // 7. APPEL DE LA FONCTION DE BOUCLE UTILISATEUR
    tempo_maj();
    user_250ms();
    delay(0);

    // 8. MISE A JOUR DES LEDs
    #if USER_USE_MAX == 1
        if(is_500ms) led_cpt++;
        led_maj();   //2.5ms
        if(boost == BOOST_DCC) dcc_keepalive();
    #endif   //(yield ds les fonctions SPI)

    // 9. MISE A JOUR PWM et SRV du PCA9685
    #if USER_USE_PCA == 1
        pca9685_slow_srv(); // compute positions for servos using slow movements
        index = 0;
        while(1) // total pca9685_maj lasts between 0 and 60ms, so we cut it to call process_packet() between
        {
            if(pca9685_maj(index)) break;  //return 1 when nothing else to update   0.6ms/ch

            index++;
            if((index & 7) == 0)  //each 4.8ms
            {
                if(boost == BOOST_DCC) dcc_keepalive();
            }
        }
    #endif //(yield ds les fonctions I2C)

    // 10. MISE A JOUR DES EXTENSIONS
    iti_update();
    bmvu_update();
    aubinage_update();
    sig_update();

    // 11. DEMANDE DE MISE A JOUR DU MESSAGE DE STATUS (deulement une fois toute les 250ms)
    status_msg[0] = '\0';

    delay(0);
}
