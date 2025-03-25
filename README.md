# Mario Bros (SFML, C++, Makefile)

## **Auteurs du Projet :**

1. Sarah EL ZEGHENDY
2. Evinia ANASTASOPOULOU
3. Julie MAUGUIT

## **Description du Projet**

Ce projet est une version personnalisée de Mario Bros développée en **C++** avec l'utilisation de la bibliothèque SFML . Il inclut un menu de démarrage avec plusieurs modes de jeu :

* **1 vs 1** : Mario contre Luigi (chaque personnage est contrôlé par un joueur).
* **Joueur vs IA** : Mario est contrôlé par le joueur tandis que Luigi est contrôlé par une IA.

## **Dépendances**

* **SFML** : Gestion des graphiques, audio et événements.
* **C++** : Langage de programmation principal.
* **Makefile** : Pour automatiser la compilation.
* **Java** : Pour les diagramme UML.

## **Installation**

### 1. **Installer les Dépendances**

Assurez-vous d’avoir installé  **SFML** , **g++** et  **Make** .

    1.1 - Installation G++ et make sur Linux :

```bash
sudo apt update
sudo apt install build-essential
```

    1.2 - Installation SFML sur Ubuntu :

```bash
sudo apt update
sudo apt install libsfml-dev build-essential
```

### 2. **Cloner le Dépôt**

Pour récupérer le code source, ouvrez un terminal et entrez la commande suivante :

```bash
git clone https://github.com/Sarahzeghendy/Mario-Bros.git
cd Mario-Bros
```

### 3. **Compilation et Lancement du Jeu**

Pour compiler le projet, utilisez le Makefile inclus :

```bash
cd Mario-Bros/src
make
```

Puis lancez le jeu :

```bash
./mario
```

Si vous voulez nettoyer les fichiers générés :

```bash
make clean
```

### **4. Création diagrammes UML**

Les images des diagrammes UML existent déjà dans le dossier  "uml" mais si vous souhaitez réexecuter suivez es étapes suivantes :

Pour la création des diagrammes UML il faut tout d'abord, modifier le fichier setup_plantuml.sh situé dans le dossier Marios-Bros. Il faudra modifier les 2 chemins suivants :

```bash
UML_DIR="$HOME/Téléchargements/Mario-Bros/uml"
PLANTUML_JAR="$HOME/Téléchargements/Mario-Bros/tools/plantuml.jar"
```

Le premier chemin correspond à l'emplacement du dossier "uml". Le deuxième chemin correspond à l'emplacement du fichier "plantuml.jar".

Une fois que vous avez précisé les bons chemins, vous pouver lancer la génération de tous vos diagramme UML avet la commande suivante :

```bash
cd Mario-Bros/
./setup_plantuml.sh
```

## **But du Jeu**

Le but du jeu est  d'arriver en premier au drapeau tout en surmontant les obstacleq ! Pour avoir le plus de vies possible pour arriver à la fin il faut collecter le plus de pièces et tuer les ennemis.

Attention : Mario et Luigi ne peuvent pas se traverser !

### **Objectifs Principaux** :

* Collecter le plus de pièces pour augmenter votre score.
* Éviter les trous pour ne pas perdre de vies.
* Éviter les obstacles comme les bricks et pipes pour continuer à avancer.
* Échapper aux ennemis ou les éliminer si possible soit en sautant dessus soit tirer des boules de feux si vous avez le pouvoir de la fleur.
* Récupérer des pouvoirs (champignons, étoiles, fleur) pour gagner des avantages temporaires (devenir plus grand, super vitesse, lancer des boules de feu).
* Atteindre le drapeau en premier.

Le gagnant est celui qui atteint le drapeau en premier  !

## **Gestion des Vies et du Score**

* Le joueur commence avec 3 vies .
* Le score augmente en :
  * Collectant des pièces (+5 points)
  * Sautant sur les ennemis (+20 points)
  * Tirant une boule de feu sur les ennemis (+30 points)
* À chaque 100 points , le joueur  gagne une vie supplémentaire.
* Si le joueur meurt mais a encore une vie disponible, il revient à l'endroit où il est mort sans recommencer le niveau depuis le début.
* S’il n’a plus de vies, c’est Game Over pour lui, l'autre joueur gagne. Vous avez l'option de revenir au Menu pour recommencer une nouvelle partie.

## **Comment jouer?**

### **Menu Principal**

Lorsque le jeu démarre, vous accédez à un menu avec plusieurs options :

* Jouer.
* Jouer contre IA.
* Règles du jeu.
* Quitter.

Pour se déplacer dans le menu vous appuyez sur les fleches up et down. Une fois que vous avez choisi votre option appuyer sur entrée pour y acceder et sur échap pour sortir.

### **Contrôles**

***Mode Joueur vs Joueur***

| Action                         | Mario (Joueur 1) | Luigi (Joueur 2) |
| ------------------------------ | ---------------- | ---------------- |
| **Avancer**              | Flèche →       | **D**      |
| **Reculer**              | Flèche ←       | **Q**      |
| **Sauter**               | Flèche ↑       | **Z**      |
| **Tirer (Boule de feu)** | **Espace** | **F**      |

***Mode Joueur vs IA***

* Mario est contrôlé par le joueur :

  Avancer → Flèche →

  Reculer → Flèche ←

  Sauter → Flèche ↑

  Tirer (Boule de feu) → Espace
* Luigi est contrôlé par une IA qui se déplace automatiquement.

### Les Défis

#### Goomba

Le joueur peut tuer le Goomba en lui sautant dessus.

Dans le cas contraire, au contact du Goomba avec le joueur :

* Si le Joueur est grand il devient petit.
* Si le Joueur est petit il meurt.
* Si le Joueur a le pouvoir du feu il le perd.

#### Koopa Troopa

Le joueur fait rentrer le Koopa Troopa dans sa carapasse en lui sautant dessus. Le Koopa dans sa carapasse continue à bouger sans effet sur le joueur.

D'autre part, au contact du Koopa Troopa avec le joueur :

* Si le Joueur est grand il devient petit.
* Si le Joueur est petit il meurt.
* Si le Joueur a le pouvoir du feu il le perd.

### Les Aides

#### Friendly Mushroom

Le joueur petit en récupérant le Friendly Mushroom devient grand. S'il est déjà grand celle-ci n'a pas d'effet sur lui.

#### Fire Flower

Lorsque le joueur collecte une fleur il a désormais le pouvoir de lancer des boules de feu. En lançant ces boules de feu il peut éliminer les ennemis.

#### Etloile

Lorsque le joueur collecte une étoile sa vitesse augmente pour un temps limité.

## Description des Classes

Pour construire le jeu nous avons créé les classes suivantes:

* Classe Player
* Classe Mouvement
* Classe Enemy
* Classe FriendlyMushroom
* Classe FireFlower
* Classe Fireball
* Classe Etoile
* Classe Coin
* Classe Camera
* Classe Background
* Classe AIPlayer
* Classe KoopaTroopa
* Classe Goomba
* Classe Menu
* Classe Score
* Classe Game

Pour consulter les détails de chaque classe et comment elles intéragissent entre elles, vous pouvez aller voir les diagrammes UML disponibles dans le dossier UML.

## Lien GitHub

https://github.com/Sarahzeghendy/Mario-Bros.git
