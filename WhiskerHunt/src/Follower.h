#pragma once

#include <SFML/Graphics.hpp>
#include <string>

/// @brief Repräsentiert einen einzelnen Katzen-Follower, der der Spielfigur folgt.
class Follower {
public:
    /// @brief Konstruktor: Initialisiert den Follower mit der gegebenen Textur.
    /// @param texturePath Pfad zur Bilddatei des Follower-Sprites.
    explicit Follower(const std::string& texturePath);

    /// @brief Aktualisiert die Position und Animation des Followers.
    /// @param deltaTime Zeit seit letztem Frame.
    /// @param targetPosition Zielposition, die der Follower ansteuert.
    void update(float deltaTime, const sf::Vector2f& targetPosition);

    /// @brief Zeichnet den Follower auf das übergebene Renderfenster.
    void draw(sf::RenderWindow& window);

    /// @brief Gibt die aktuelle Position des Followers zurück.
    sf::Vector2f getPosition() const;

    /// @brief Setzt die Position des Followers (z. B. beim Spawn).
    void setPosition(const sf::Vector2f& pos);

    /// @brief Gibt die Kollisionsgrenzen des Sprites zurück (für z. B. Snake-Kollision).
    sf::FloatRect getGlobalBounds() const;

private:
    // Grafikkomponenten
    sf::Texture texture;     // Textur des Follower-Sprites
    sf::Sprite sprite;       // Visueller Sprite

    // Bewegung
    float speed = 110.f;     // Geschwindigkeit in Pixel pro Sekunde

    // Animation
    int frame = 0;                     // Aktueller Animationsframe
    float animationTimer = 0.f;       // Timer für Animationstaktung
    const float animationDelay = 0.2f;// Dauer zwischen Frames
    int direction = 3;                // Bewegungsrichtung (0=oben, 1=links, 2=rechts, 3=unten)
    int frameWidth = 32;              // Breite eines einzelnen Frames
    int frameHeight = 32;             // Höhe eines einzelnen Frames

    /// @brief Interne Methode zur Aktualisierung der Animation (Sprite-Schnitt).
    void updateAnimation(float deltaTime);
};
