💧 WildWater-TECH

WildWater-TECH est un programme en C conçu pour modéliser et analyser la performance des réseaux de distribution d'eau potable.

🚀 Comment l'exécuter

Pour démarrer le programme, ouvrez un terminal de commande dans le dossier du projet :

1. Compilation : Tapez `make` pour générer l'exécutable `wildwater`.
2. Lancement** : Le programme est orchestré par un script Shell pour faciliter les filtres. Utilisez la commande :
`./myScript.sh <fichier_csv> <commande> [options]`.

🧩 Fonctionnalités du programme

Tout ce dont vous avez besoin pour analyser le rendement d'un réseau :

1. **Analyse Capacitaire (Histo Max)** : Calcule la capacité maximale de production de chaque usine.
2. **Suivi de Captation (Histo Src)** : Mesure le volume total d'eau prélevé aux sources pour chaque unité de traitement.
3. **Volume Réel (Histo Real)** : Évalue la quantité d'eau réellement traitée après les pertes initiales.
4. **Détection de Fuites (Leaks)** : Analyse le rendement d'une usine spécifique en calculant les pertes par évaporation ou fuite dans tout son réseau aval.
5. **Visualisation Graphique** : Génération automatique de graphiques via Gnuplot pour visualiser les résultats.


