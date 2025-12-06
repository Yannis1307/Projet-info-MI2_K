\# Documentation Technique - Partie A (Scripting \& Automatisation)



Ce module gère l'automatisation des tests et le lancement du programme principal `wildwater`.



\## 1. Pré-requis

\* Environnement Linux ou Git Bash (Windows).

\* Compilateur GCC et Make installés.



\## 2. Compilation

Pour générer l'exécutable `wildwater` (actuellement un stub de test) :

make



Pour nettoyer les fichiers compilés (.o et exécutable) :

make clean



\## 3. Utilisation du script shell

Le script `shell/myScript.sh` est le point d'entrée unique. Il vérifie les arguments avant d'appeler le programme C.



\### Commandes disponibles :



\*\*A) Mode Histogramme (Analyse de hauteur)\*\*

./shell/myScript.sh histo <mode> <fichier\_csv>



\* `mode` : Doit être `max`, `src`, ou `real`.

\* `fichier\_csv` : Chemin vers le fichier de données.



\*\*B) Mode Fuites (Analyse de consommation)\*\*

./shell/myScript.sh leaks "<id\_usine>" <fichier\_csv>



\* `id\_usine` : L'identifiant de l'usine (entre guillemets si contient des espaces).

\* `fichier\_csv` : Chemin vers le fichier de données.



\## 4. À faire (Roadmap)

\* \[ ] Remplacer le `main.c` de test par le vrai programme C (Équipe C).

\* \[ ] Le programme C devra générer les fichiers `.dat` en sortie.

\* \[ ] Le script shell appellera ensuite Gnuplot pour créer les graphiques.

