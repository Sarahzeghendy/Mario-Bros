#ifndef FRIENDLY_MUSHROOM_HPP
#define FRIENDLY_MUSHROOM_HPP

#include "enemy.hpp"
#include "player.hpp"
#include <string>

class FriendlyMushroom : public Enemy {
public:
    FriendlyMushroom(float x, float y);  // Updated constructor without limits

    void update() override;
    void interactWithPlayer(Player& player) override;
    void onJumpedOn() override; 
    void reverseDirection();
};

#endif