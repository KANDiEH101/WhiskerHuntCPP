#pragma once
#include <SFML/Graphics.hpp>

/// @brief Repräsentiert ein "Treat" (Belohnung oder Bestrafung), das eingesammelt werden kann.
class Treat {
public:
    /// @brief Konstruktor: Lädt zufällige Textur (Belohnung oder Bestrafung) und initialisiert Animation
    Treat();

    /// @brief Aktualisiert die Animation des Treats (Framewechsel)
    /// @param deltaTime Zeit seit letztem Frame
    void update(float deltaTime);

    /// @brief Zeichnet das Treat auf das übergebene RenderWindow
    /// @param window Referenz auf das SFML-Fenster
    void draw(sf::RenderWindow& window);

    /// @brief Setzt die Position des Treats
    /// @param position Weltkoordinaten des Treats
    void setPosition(const sf::Vector2f& position);

    /// @brief Gibt die Kollisionsgrenze (Hitbox) des Treats zurück
    /// @return sf::FloatRect mit Grenzen des Treat-Sprites
    sf::FloatRect getBounds() const;

    /// @brief Prüft, ob das Treat ein Bestrafungselement ist
    /// @return true, wenn es eine Bestrafung ist, sonst false
    bool isPunishment() const;

private:
    sf::Texture texture;     ///< Textur des Treats
    sf::Sprite sprite;       ///< Sprite zur Darstellung des Treats

    int frame = 0;           ///< Aktueller Frame für die Animation
    float animationTimer = 0.f; ///< Timer für Animation
    float animationDelay = 0.2f; ///< Zeitverzögerung zwischen Frames

    int frameWidth;          ///< Einzelne Frame-Breite
    int frameHeight;         ///< Einzelne Frame-Höhe
    bool punishment;         ///< Gibt an, ob Treat bestrafend ist

    /// @brief Wählt zufällig ein Treat aus 11 vorhandenen (Belohnung oder Bestrafung)
    void randomizeTexture();
};
