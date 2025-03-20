#!/bin/bash

# Create a directory for tools if it doesn't exist
mkdir -p ~/Bureau/Mario-Bros/tools

# Download PlantUML jar file
echo "Downloading PlantUML..."
wget https://sourceforge.net/projects/plantuml/files/plantuml.jar/download -O ~/Bureau/Mario-Bros/tools/plantuml.jar

# Make the script executable
chmod +x ~/Bureau/Mario-Bros/tools/plantuml.jar

echo "PlantUML has been downloaded to ~/Bureau/Mario-Bros/tools/plantuml.jar"
echo "You can now generate UML diagrams with:"
echo "java -jar ~/Bureau/Mario-Bros/tools/plantuml.jar ~/Bureau/Mario-Bros/uml/*.puml"
