#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "Treat.h"

// Externe Zustände, die auch in Player verwendet werden
extern sf::Texture gameOverTexture;
extern sf::Sprite gameOverSprite;
extern bool isGameOver;

/// @brief Die Hauptklasse für das gesamte Spiel: Initialisierung, Loop-Logik, Rendering
class Game {
public:
    /// @brief Konstruktor: Initialisiert Fenster, lädt Assets und setzt den Startzustand
    Game();

    /// @brief Startet die Hauptspielschleife
    void run();

private:
    // Fenster und Zeitsteuerung
    sf::RenderWindow window;  ///< Das Hauptanzeigefenster des Spiels
    sf::Clock clock;          ///< Dient zur Berechnung von deltaTime

    // Spielzustände
    bool showStartScreen = true; ///< Wird true gesetzt, wenn Startbildschirm angezeigt werden soll

    // Spielfigur
    Player player; ///< Die Spielfigur „Miels“

    // Treats zum Einsammeln
    std::vector<std::shared_ptr<Treat>> treats; ///< Liste aller Treats, die aktuell aktiv sind
    void spawnTreat(); ///< Spawnt ein neues Treat an zufälliger Position

    // Hintergrundgrafik
    sf::Texture backgroundTexture; ///< Textur für den Hintergrund
    sf::Sprite backgroundSprite;   ///< Sprite für die Darstellung des Hintergrunds

    // Startbildschirmgrafik
    sf::Texture startTexture; ///< Textur für den Startscreen
    sf::Sprite startSprite;   ///< Sprite für den Startscreen
};
