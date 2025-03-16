# Nom de l'exécutable
EXEC = mario

# Répertoires
SRC_DIR = .
HEADERS_DIR = $(SRC_DIR)/Headers
IMAGES_DIR = $(SRC_DIR)/images

# Compiler et options
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(HEADERS_DIR)

# Bibliothèques SFML
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Liste des fichiers source à compiler
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/background.cpp \
       $(SRC_DIR)/player.cpp \
       $(SRC_DIR)/mouvement.cpp \
       $(SRC_DIR)/camera.cpp \
       $(SRC_DIR)/coin.cpp \
       $(SRC_DIR)/enemy.cpp \
       $(SRC_DIR)/friendlymushroom.cpp \
       $(SRC_DIR)/menu.cpp \
       $(SRC_DIR)/game.cpp \
       $(SRC_DIR)/goomba.cpp \
       $(SRC_DIR)/koopa.cpp \
       $(SRC_DIR)/fireflower.cpp \
       $(SRC_DIR)/fireball.cpp \


# Générer les fichiers objets correspondants
OBJS = $(SRCS:.cpp=.o)

# Règle principale
all: $(EXEC)

# Compilation de l'exécutable
$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(SFML_LIBS)

# Compilation des fichiers .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -f $(OBJS) $(EXEC)

# Nettoyage complet
mrproper: clean
	rm -f $(EXEC)
