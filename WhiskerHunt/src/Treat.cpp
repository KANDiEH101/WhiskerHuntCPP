#include "Treat.h"
#include <iostream>
#include <random>

/// @brief Konstruktor: Initialisiert das Treat mit einer zufälligen Textur und setzt die Animation auf den ersten Frame
Treat::Treat() {
    randomizeTexture(); // Lade zufällige Treat-Textur

    // Setze Maße der einzelnen Animationsframes (4 horizontale Frames pro Datei)
    frameWidth = texture.getSize().x / 4;
    frameHeight = texture.getSize().y;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(1.0f, 1.0f); // Standard-Skalierung
}

/// @brief Lädt eine zufällige Treat-Textur (treat1.png bis treat9.png) und markiert sie als Belohnung
void Treat::randomizeTexture() {
    static std::random_device rd;
    static std::mt19937 gen(rd()); // Zufallsgenerator (einmal initialisiert)
    std::uniform_int_distribution<> distr(1, 9); // Treat 1 bis 9 (ausschließlich keine Strafen)

    int treatNumber = distr(gen);
    std::string path = "assets/treats/treat" + std::to_string(treatNumber) + ".png";

    if (!texture.loadFromFile(path)) {
        std::cerr << "Fehler beim Laden von " << path << std::endl;
    }

    punishment = false; // Standardmäßig kein Strafe-Treat
}

/// @brief Aktualisiert die Animationslogik: wechselt alle 0.2 Sekunden auf den nächsten Frame
/// @param deltaTime Zeit seit letztem Frame
void Treat::update(float deltaTime) {
    animationTimer += deltaTime;

    if (animationTimer >= animationDelay) {
        frame = (frame + 1) % 4; // 4 Frames pro Zeile
        animationTimer = 0.f;

        // Setze den neuen Bildausschnitt (Frame) des Treats
        sprite.setTextureRect(sf::IntRect(frame * frameWidth, 0, frameWidth, frameHeight));
    }
}

/// @brief Zeichnet das Treat-Sprite auf das übergebene SFML-Window
/// @param window Ziel-Fenster
void Treat::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

/// @brief Setzt die Welt-Position des Treats
/// @param position neue Position
void Treat::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

/// @brief Gibt die aktuelle Kollisionsbox (GlobalBounds) zurück
sf::FloatRect Treat::getBounds() const {
    return sprite.getGlobalBounds();
}

/// @brief Gibt zurück, ob dieses Treat ein Bestrafungstyp ist
bool Treat::isPunishment() const {
    return punishment;
}
