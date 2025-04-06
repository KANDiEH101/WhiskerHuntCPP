#include "Player.h"
#include "Follower.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

/// @brief Konstruktor: lädt Textur, setzt Startposition, skaliert Sprite und initialisiert Trail
Player::Player() {
    if (!texture.loadFromFile("assets/player/player_walk.png")) {
        std::cerr << "Fehler beim Laden von assets/player/player_walk.png" << std::endl;
    }

    sprite.setTexture(texture);
    sprite.setPosition(400.f, 300.f);
    sprite.setScale(2.0f, 2.0f);

    frameWidth = texture.getSize().x / 9;
    frameHeight = texture.getSize().y / 4;

    sprite.setTextureRect(sf::IntRect(
        0,
        direction * frameHeight,
        frameWidth,
        frameHeight
    ));

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Trail mit Startposition füllen
    for (int i = 0; i < maxTrailLength; ++i) {
        trail.push_back(sprite.getPosition());
    }
}

/// @brief Bewegt den Spieler in eine Richtung, wenn Taste gedrückt wird und kein Game Over
void Player::handleInput() {
    if (isGameOver) return;

    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && lastDirection != 2) {
        velocity = sf::Vector2f(0.f, -speed);
        direction = 0;
        lastDirection = 0;
        hasStartedMoving = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && lastDirection != 0) {
        velocity = sf::Vector2f(0.f, speed);
        direction = 2;
        lastDirection = 2;
        hasStartedMoving = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && lastDirection != 3) {
        velocity = sf::Vector2f(-speed, 0.f);
        direction = 1;
        lastDirection = 1;
        hasStartedMoving = true;
    }
    else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && lastDirection != 1) {
        velocity = sf::Vector2f(speed, 0.f);
        direction = 3;
        lastDirection = 3;
        hasStartedMoving = true;
    }
}

/// @brief Haupt-Update-Methode des Spielers: Bewegung, Trail, Animation, Kollision, Follower
void Player::update(float deltaTime, const sf::RenderWindow& window) {
    sprite.move(velocity * deltaTime);
    updateAnimation(deltaTime);

    // Bildschirm-Wraparound
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2u windowSize = window.getSize();

    float spriteWidth = frameWidth * sprite.getScale().x;
    float spriteHeight = frameHeight * sprite.getScale().y;

    if (pos.x + spriteWidth < 0) pos.x = static_cast<float>(windowSize.x);
    else if (pos.x > windowSize.x) pos.x = -spriteWidth + 1.f;

    if (pos.y + spriteHeight < 0) pos.y = static_cast<float>(windowSize.y);
    else if (pos.y > windowSize.y) pos.y = -spriteHeight + 1.f;

    sprite.setPosition(pos);

    // Trail speichern
    trail.push_front(sprite.getPosition());
    if (trail.size() > followers.size() * trailSpacing + 1) {
        trail.pop_back();
    }

    // Kollision mit eigenem Follower (Snake-Mechanik)
    for (size_t i = 2; i < followers.size(); ++i) {
        sf::Vector2f followerPos = followers[i]->getPosition();
        sf::Vector2f playerPos = sprite.getPosition();

        float dx = followerPos.x - playerPos.x;
        float dy = followerPos.y - playerPos.y;

        const float threshold = 12.f;
        bool collision = false;

        switch (direction) {
        case 0: collision = (dy < 0 && std::abs(dy) < threshold && std::abs(dx) < threshold); break;
        case 1: collision = (dx < 0 && std::abs(dx) < threshold && std::abs(dy) < threshold); break;
        case 2: collision = (dy > 0 && std::abs(dy) < threshold && std::abs(dx) < threshold); break;
        case 3: collision = (dx > 0 && std::abs(dx) < threshold && std::abs(dy) < threshold); break;
        }

        if (collision) {
            std::cout << " Miels ist mit sich selbst kollidiert! Game Over!" << std::endl;
            isGameOver = true;
            velocity = sf::Vector2f(0.f, 0.f);
            return;
        }
    }

    updateFollowers(deltaTime);
}

/// @brief Zeichnet Spieler und Follower auf das Fenster
void Player::draw(sf::RenderWindow& window) {
    for (size_t i = 0; i < followers.size(); ++i) {
        followers[i]->draw(window);
    }
    window.draw(sprite);
}

/// @brief Aktualisiert Sprite-Animation auf Basis der Bewegungsrichtung
void Player::updateAnimation(float deltaTime) {
    if (hasStartedMoving) {
        animationTimer += deltaTime;

        if (animationTimer >= animationDelay) {
            frame = (frame + 1) % 9;
            animationTimer = 0.f;
        }

        sprite.setTextureRect(sf::IntRect(
            frame * frameWidth,
            direction * frameHeight,
            frameWidth,
            frameHeight
        ));
    }
}

/// @brief Gibt die Kollisionsbox des Spielers zurück
sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

/// @brief Fügt eine neue Katze zum Follower-Kreis hinzu
void Player::addFollower() {
    int random = rand() % 8 + 1;
    std::string path = "assets/cat/cat" + std::to_string(random) + ".png";

    auto follower = std::make_shared<Follower>(path);

    // Position aus dem Trail entnehmen
    sf::Vector2f spawnPos = sprite.getPosition();
    if (trail.size() >= trailSpacing * (followers.size() + 1)) {
        spawnPos = trail[(followers.size() + 1) * trailSpacing];
    }

    if (direction == 1) {  // Wenn Spieler nach links läuft: versetzen
        spawnPos.x += 6.f;
    }

    follower->setPosition(spawnPos);
    followers.push_back(follower);

    std::cout << "Follower gesammelt!\n";
}

/// @brief Entfernt den letzten Follower (z. B. bei Bestrafung)
void Player::loseFollower() {
    if (!followers.empty()) {
        followers.pop_back();
        std::cout << "Follower entfernt!\n";
    }
}

/// @brief Aktualisiert die Bewegung der Follower entlang des Trails
void Player::updateFollowers(float deltaTime) {
    for (size_t i = 0; i < followers.size(); ++i) {
        size_t index = (i + 1) * trailSpacing;
        if (index < trail.size()) {
            sf::Vector2f target = trail.at(index);
            target.y += 6.f; // Leichter Versatz zur optischen Korrektur
            followers[i]->update(deltaTime, target);
        }
    }
}

/// @brief Prüft auf grobe Kollision mit Followern (nicht verwendet)
bool Player::checkSelfCollision() const {
    sf::FloatRect playerBounds = sprite.getGlobalBounds();

    for (const auto& follower : followers) {
        if (playerBounds.intersects(follower->getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

/// @brief Setzt den Spieler nach Game Over auf Ausgangszustand zurück
void Player::reset() {
    sprite.setPosition(400.f, 300.f);
    velocity = sf::Vector2f(0.f, 0.f);
    direction = 2;
    frame = 0;
    animationTimer = 0.f;
    hasStartedMoving = false;

    followers.clear();   // Katzen entfernen
    trail.clear();       // Trail leeren

    for (int i = 0; i < maxTrailLength; ++i) {
        trail.push_back(sprite.getPosition());
    }
}
