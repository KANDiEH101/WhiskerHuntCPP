#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "Treat.h"

// Externe Zust�nde, die auch in Player verwendet werden
extern sf::Texture gameOverTexture;
extern sf::Sprite gameOverSprite;
extern bool isGameOver;

/// @brief Die Hauptklasse f�r das gesamte Spiel: Initialisierung, Loop-Logik, Rendering
class Game {
public:
    /// @brief Konstruktor: Initialisiert Fenster, l�dt Assets und setzt den Startzustand
    Game();

    /// @brief Startet die Hauptspielschleife
    void run();

private:
    // Fenster und Zeitsteuerung
    sf::RenderWindow window;  ///< Das Hauptanzeigefenster des Spiels
    sf::Clock clock;          ///< Dient zur Berechnung von deltaTime

    // Spielzust�nde
    bool showStartScreen = true; ///< Wird true gesetzt, wenn Startbildschirm angezeigt werden soll

    // Spielfigur
    Player player; ///< Die Spielfigur �Miels�

    // Treats zum Einsammeln
    std::vector<std::shared_ptr<Treat>> treats; ///< Liste aller Treats, die aktuell aktiv sind
    void spawnTreat(); ///< Spawnt ein neues Treat an zuf�lliger Position

    // Hintergrundgrafik
    sf::Texture backgroundTexture; ///< Textur f�r den Hintergrund
    sf::Sprite backgroundSprite;   ///< Sprite f�r die Darstellung des Hintergrunds

    // Startbildschirmgrafik
    sf::Texture startTexture; ///< Textur f�r den Startscreen
    sf::Sprite startSprite;   ///< Sprite f�r den Startscreen
};
