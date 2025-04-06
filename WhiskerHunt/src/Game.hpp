#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"  // Die Spieler-Klasse einbinden

class Game {
public:
    Game(); // Konstruktor
    void run(); // Hauptspiel-Loop

private:
    sf::RenderWindow window;  // Das Fenster
    Player player;  // Das Player-Objekt
    sf::Clock clock;  // Die Zeitmessung für die FPS
};
