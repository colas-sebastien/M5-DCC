
| Date | Auteur | Description |
| ---- | ------ | ----------- |
| 2024/12/06 | Sébastien Colas | Portage vers M5atom + TF-Card-Reader, ajout du support de mDNS, Serveur Web sur Core 0, Correction bug Upload de fichier 
| 2021/12/04 | Ulysse Delmas-Begue |  Si une page web est demandee hors AU, propose de passer en AU    
| 2020/10/19 | Ulysse Delmas-Begue |  Mise en ligne, Activation par defaut des souris html
| 2020/04/27 | Ulysse Delmas-Begue | Possibilité de choisir le mode analogique/dcc independament de l'utilisation de l'adresse analogique par une souris
| 2020/04/20 | Ulysse Delmas-Begue | Ajout d'une impulsion bidirectionelle en analogique pour detecter les locos avec les detecteurs de courant  dcc
| 2020/04/10 | Ulysse Delmas-Begue | Ajout d'une constante pour regler la repetition des commandes accessoires
| 2020/04/03 |  Ulysse Delmas-Begue | Correction d'un bug sur la commande des accessoires DCC (pendant un court instant) an analogique  
| 2020/03/28 |  Ulysse Delmas-Begue | Augmentation du nombre des variables u de 96 a 192
| 2020/03/28 |  Ulysse Delmas-Begue | Ajout des extensions BMVU, ITI, aubinage, SIG (experimental)
| 2020/03/20 |  Ulysse Delmas-Begue | Mise en ligne de la version HTML
| 2020/03/12 |  Ulysse Delmas-Begue | Liberation du mode analogique si une souris se deconnecte
| 2020/03/12 |  Ulysse Delmas-Begue | Possibilite de regler le nombre de trames envoyees a chaque changement de fonction auxiliaire
| 2020/03/11 |  Ulysse Delmas-Begue | Ajout de 5 canaux separes pour les websockets
| 2020/03/10 |  Ulysse Delmas-Begue | Ajout du serveur Web HTTP et Websockets (5 clients websocket max en // des clients TCP et pages web)
| 2020/03/09 |  Ulysse Delmas-Begue | Corection d'un bug sur le mask des variables u  Inversion du sens pour DS
| 2019/04/08 |  Ulysse Delmas-Begue | Correction d'un bug dans le protocole D17 wifi qui affecté les fonctions auxiliaires >= 10
| 2019/03/05 |  Ulysse Delmas-Begue | Support de CDM Rail avec le protocole DCC++
| 2019/02/25 |  Ulysse Delmas-Begue | Correction de bugs (envoie des adresses longues,  protocole desktop station, analogwrite)
| 2019/02/22 |  Ulysse Delmas-Begue | Ajout des adresses longues 0-10239
| 2019/02/21 |  Ulysse Delmas-Begue | Possibilité de connecter 8 clients wifi (qui peuvent etre sur des appareils differents) contre 4 auparavant (ce nombre est parametrable et mis a 4 par defaut)
| 2019/02/21 |  Ulysse Delmas-Begue | Passage de 2 locos max a 4 locos max par souris (pour etre compatible avec JM)
| 2019/02/21 |  Ulysse Delmas-Begue | Passage aux slots
| 2019/01/30 |  Ulysse Delmas-Begue | Ajout du protocol Desktop Station
| 2019/01/24 |  Ulysse Delmas-Begue | Correction d'un bug qui n'envoyait que la valeur 0 aux decodeurs DCC
| 2018/03/23 |  Ulysse Delmas-Begue | Passage des preambules à 16 "1" sur les conseils de JM afin de restpecter la norme NEM671. Il semblerait que ca fasse du bien a certains decodeurs recents
| 2018/03/08 |  Ulysse Delmas-Begue | Merge des changements de JM
| 2018/03/07 |  Jean-Michel Dubois | Proposition pour les changements suivants: <ul> <li> Ajout du support du mode station (en plus du mode point d'accès) </li>         <li> Generation des trames DCC plus claire </li> <li> Ajout d'un mode pour gérer différement les fonctions auxiliaires (car les mises a jour permanentes des fonctions posent des problemes aux decodeurs ESU pour les sons) </li> <li> Ajout des pointeurs pour simplifier la lisibilite du code --> TBD </li> </ul>
| 2018/02/22 |  Ulysse Delmas-Begue | Ajout du protocole DCC++ (light) pour connecter la centrale a RocRail par exemple
| 2018/02/13 |  Ulysse Delmas-Begue | <ul> <li>Ajout du choix de la polarite du bouton d'arret d'urgence</li><li> Memorisation des entrees pour chaque souris afin de ne pas manquer les contacts fugitifs comme les ILS tres utile pour le mode script) </li> <li> Limitation a une seule reponse de demande de status par souris et par boucle (afin de ne pas surcharger le wifi) </li> <li> Choix de la commande des decodeurs d'accessoires en analogique (interdir, autoriser mais avec booster inactif, autoriser avec booster actif) </li> </ul>
| 2018/02/08 | Ulysse Delmas-Begue | Ajout de la rotation lente des servos
| 2018/02/07 | Ulysse Delmas-Begue | <ul> <li>Ajout du mode 126 crans en DCC. Vous pouvez choisir entre le mode 28 crans ou 126 crans (28 crans par defaut)</li> <li> Ajout d'un facteur pour diminuer la plage PWM en analogique si vous utilisez une alim > 12V </li></ul>
| 2018/01/31 | Ulysse Delmas-Begue | <ul> <li>Passage de la resolution des tempos de 20ms a 250ms car on ne pouvait pas tenir cette resolution et les pca/led ne sont mis a jour que toute les 250ms </li><li> La mise a jour des pca9685 est maintenant faite uniquement toute les 250ms (et seulement sur les canaux necessaires) </li><li> Ajout d'un bit 0 au signal dcc lorsque la boucle peut prendre trop de temps afin de rester dans les tolerances de la norme DCC (bit0 = 10ms max par état, 12ms au total) </li></ul>
| 2018/01/29 | Ulysse Delmas-Begue | <ul> <li>Ajout du bouton d'arrêt d'urgence sur D5 </li><li> Retrait temporaire de la variation lente des servos </li></ul>
| 2018/01/22 | Ulysse Delmas-Begue | <ul><li>Ajout de la commande u </li><li> Ajout a l'etat des variables u </li><li> Ajout de l'environement user de D18 </li><li> Optimisation de la transmission des fonctions auxiliaires f1+f2-f3+... -> f1+-+ </li></ul>
| 2018/01/21 | Ulysse Delmas-Begue | Rajout de la commande des decodeurs d'accessoires etendus
| 2018/01/18 | Ulysse Delmas-Begue | Test avec le decodeur d'accessories D18
| 2018/01/07 | Ulysse Delmas-Begue | Test avec TCO sur tablette pour in et aig ok
| 2018/01/06 | Ulysse Delmas-Begue | <ul><li> La LED "sign of life" peut etre dispo maintenant sur le max </li><li> Rajout de la commande des decodeurs d'accessoirs basiques </li><li> Ajout d'une commande pour identifier la centrale </li></ul>
| 2018/01/05 | Ulysse Delmas-Begue | <ul><li>Passage a 96entrees, 128leds, 48srv/pwm </li><li> Rajout de la commande des leds/srv/pwm par wifi </li><li> Rajout variation lente des servos </li></ul>
| 2017/12/29 | Ulysse Delmas-Begue |  <ul><li>Ajout des fonctions F13-F28 </li><li> Ajout des packets de 3bytes DCC (en plus du preambule et checksum) </li><li> Ajout de la commande des decodeurs d'accesoires DCC</li></ul>
| 2017/12/27 | Ulysse Delmas-Begue |  <ul><li>Passage de 0-28 crans a 0-100% pour les souris </li><li> Ajout des fonctions F5-F12 (elle ne sont transmises que lorsque necessaire pour ne pas perdre de la bande passante) </li><li> Ajout d'un second canal dcc pour chaque souris pr piloter 2 locos simultanees par souris</li></ul>
| 2017/12/26 | Ulysse Delmas-Begue |  <ul><li>Ajout de l'I2C afin de proposer 16 PWM ou SRV par PCA9685 (16) </li><li> Ajout du S88 (64)</li></ul>
| 2017/12/24 | Ulysse Delmas-Begue | Ajout des leds (64)
| 2017/12/23 | Ulysse Delmas-Begue | Ajout programation des CV
| 2017/12/22 | Ulysse Delmas-Begue | Ajout (temporaire) de 6 sorties controlees par le wifi
| 2017/12/21 | Ulysse Delmas-Begue |  <ul><li>Correctif la centrale ne plante plus en DCC </li><li> Gere les passages entre DCC et ANA. </li><li> Ajout de la mesure du courant</li></ul>
| 2017/12/16 | Ulysse Delmas-Begue |  <ul><li>Genere un signal PWM pour une locomotive ANAlogique en fonction d'une souris wifi </li><li> Passage de 1kHz a 20KHz afin de rendre la PWM inaudible</li></ul>
| 2017/06/27 | Ulysse Delmas-Begue | Creation: Genere un signal DCC en fonction de 4 souris wifi max
