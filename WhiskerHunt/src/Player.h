#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <memory>
#include "Follower.h"

extern bool isGameOver;

/// @brief Repräsentiert den Spieler (Miels) im Spiel inklusive Bewegung, Animation, Follower-Logik und Kollision.
class Player {
public:
    Player();  // Konstruktor

    /// @brief Verarbeitet Tasteneingaben und setzt Bewegung.
    void handleInput();

    /// @brief Aktualisiert Position, Animation und Trail.
    void update(float deltaTime, const sf::RenderWindow& window);

    /// @brief Zeichnet Spieler und seine Follower auf das Fenster.
    void draw(sf::RenderWindow& window);

    /// @brief Setzt den Spieler auf Startzustand zurück (z. B. nach Game Over).
    void reset();

    /// @brief Gibt die Kollisionsgrenzen des Spielers zurück.
    sf::FloatRect getBounds() const;

    /// @brief Fügt einen neuen Follower (Katze) hinzu.
    void addFollower();

    /// @brief Entfernt den letzten Follower (z. B. bei Bestrafung).
    void loseFollower();

private:
    // --- Bewegung & Darstellung ---
    sf::Texture texture;           ///< Textur des Spielers
    sf::Sprite sprite;             ///< Sprite für die Darstellung
    sf::Vector2f velocity;         ///< Bewegungsrichtung
    float speed = 100.f;           ///< Geschwindigkeit

    int lastDirection = 3;         ///< Letzte Bewegung (wichtig für Richtungswechsel-Lock)

    /// @brief Prüft, ob der Spieler mit einem Follower kollidiert ist.
    bool checkSelfCollision() const;

    // --- Animation ---
    int frame = 0;                 ///< Aktueller Frame der Animation
    float animationTimer = 0.f;    ///< Zeit seit letztem Frame-Wechsel
    const float animationDelay = 0.15f;  ///< Zeitabstand zwischen Animationsframes
    int direction = 3;             ///< Bewegungsrichtung (0=oben, 1=links, 2=rechts, 3=unten)
    bool hasStartedMoving = false;///< Wird aktiviert, sobald sich der Spieler das erste Mal bewegt

    int frameWidth = 32;          ///< Breite eines einzelnen Frames
    int frameHeight = 32;         ///< Höhe eines einzelnen Frames

    /// @brief Aktualisiert die Animation des Spielers.
    void updateAnimation(float deltaTime);

    // --- Follower ---
    std::vector<std::shared_ptr<Follower>> followers; ///< Liste aller Katzen, die dem Spieler folgen

    /// @brief Bewegt die Follower entlang des gespeicherten Trails.
    void updateFollowers(float deltaTime);

    // --- Trail-System (Snake-Logik) ---
    std::deque<sf::Vector2f> trail;  ///< Pfad, den der Spieler hinterlässt
    const int trailSpacing = 15;     ///< Abstand zwischen zwei Followern (mehr = weiter auseinander)
    const int maxTrailLength = 300;  ///< Maximale Länge des Trails (begrenzt Speicherverbrauch)
};
