#include "Follower.h"
#include <iostream>
#include <cmath>

/// @brief Konstruktor: Lädt die Textur und initialisiert das Sprite mit richtiger Skalierung und Animation.
Follower::Follower(const std::string& texturePath) {
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Fehler beim Laden von " << texturePath << std::endl;
    }

    sprite.setTexture(texture);

    // Animation Setup: Setze die Größe eines Frames (3 Spalten × 4 Zeilen)
    frameWidth = texture.getSize().x / 3;   // 3 Animations-Frames pro Richtung
    frameHeight = texture.getSize().y / 4;  // 4 Richtungen (oben, links, rechts, unten)

    // Initialer Ausschnitt der Textur (Frame 0 in Start-Richtung)
    sprite.setTextureRect(sf::IntRect(0, direction * frameHeight, frameWidth, frameHeight));

    // Skalierung: Vergrößere das Sprite visuell
    sprite.setScale(1.5f, 1.5f);
}

/// @brief Bewegt den Follower sanft zur Zielposition und aktualisiert die Animation.
void Follower::update(float deltaTime, const sf::Vector2f& targetPosition) {
    sf::Vector2f currentPos = sprite.getPosition();
    sf::Vector2f directionVec = targetPosition - currentPos;

    float distance = std::sqrt(directionVec.x * directionVec.x + directionVec.y * directionVec.y);

    // Nur bewegen, wenn genügend Abstand besteht
    if (distance > 0.5f) {
        sf::Vector2f normalized = directionVec / distance;
        sprite.move(normalized * speed * deltaTime);

        // Bewegungsrichtung für Animation berechnen
        if (std::abs(normalized.x) > std::abs(normalized.y)) {
            direction = (normalized.x > 0) ? 2 : 1; // rechts oder links
        }
        else {
            direction = (normalized.y > 0) ? 0 : 3; // unten oder oben
        }

        updateAnimation(deltaTime); // Animation nur bei Bewegung aktualisieren
    }
}

/// @brief Wechselt zwischen den Animationsframes, basierend auf der Zeit.
void Follower::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= animationDelay) {
        frame = (frame + 1) % 3; // Zyklisch durch 3 Frames
        animationTimer = 0.f;
    }

    sprite.setTextureRect(sf::IntRect(
        frame * frameWidth,
        direction * frameHeight,
        frameWidth,
        frameHeight
    ));
}

/// @brief Zeichnet den Follower ins Fenster.
void Follower::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

/// @brief Gibt die aktuelle Position des Followers zurück.
sf::Vector2f Follower::getPosition() const {
    return sprite.getPosition();
}

/// @brief Setzt die exakte Position des Followers mit Y-Offset für bessere Darstellung.
void Follower::setPosition(const sf::Vector2f& pos) {
    const float yOffset = 2.0f; // Kleine optische Korrektur nach unten
    sprite.setPosition(pos.x, pos.y + yOffset);
}

/// @brief Gibt die Kollisionsgrenze des Sprites zurück (z. B. für Snake-Kollision).
sf::FloatRect Follower::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
