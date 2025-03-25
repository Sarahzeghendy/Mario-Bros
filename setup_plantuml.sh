#!/bin/bash

# Définir le chemin du dossier UML
UML_DIR="$HOME/Téléchargements/Mario-Bros/uml"
PLANTUML_JAR="$HOME/Téléchargements/Mario-Bros/tools/plantuml.jar"

# Vérifie si le fichier plantuml.jar existe
if [ ! -f "$PLANTUML_JAR" ]; then
    echo "Erreur : plantuml.jar introuvable dans $PLANTUML_JAR"
    exit 1
fi

# Trouve et exécute tous les fichiers .puml
find "$UML_DIR" -name '*.puml' -exec java -jar "$PLANTUML_JAR" {} \;

echo "Tous les fichiers .puml ont été traités avec succès !"
