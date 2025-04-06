#include <SFML/Graphics.hpp>
#include "Game.h"
#include <ctime>
#include <cstdlib>
#include <memory>
#include <iostream>

// Globale Spielstatus-Variablen
sf::Texture gameOverTexture;         // Textur für den Game-Over-Bildschirm
sf::Sprite gameOverSprite;          // Sprite für den Game-Over-Bildschirm
bool isGameOver = false;            // Spielstatus: true, wenn Spieler verloren hat

// Konstruktor: Initialisiert Fenster, lädt Ressourcen, skaliert Grafiken
Game::Game() : window(sf::VideoMode(800, 600), "Whisker Hunt") {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Startbildschirm laden und setzen
    if (!startTexture.loadFromFile("assets/screens/startscreen.png")) {
        std::cerr << "Fehler beim Laden des Startbilds!" << std::endl;
    }
    startSprite.setTexture(startTexture);

    // Game-Over-Bildschirm laden und setzen
    if (!gameOverTexture.loadFromFile("assets/screens/gameover.png")) {
        std::cerr << "Game-Over-Image konnte nicht geladen werden!\n";
    }
    else {
        std::cout << "Game-Over-Image erfolgreich geladen!\n";
        gameOverSprite.setTexture(gameOverTexture);
    }

    // Startbildschirm skalieren auf Fenstergröße
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u texSize = startTexture.getSize();
    float startScaleX = static_cast<float>(winSize.x) / texSize.x;
    float startScaleY = static_cast<float>(winSize.y) / texSize.y;
    startSprite.setScale(startScaleX, startScaleY);

    // Game-Over-Bildschirm ebenfalls skalieren
    sf::Vector2u overTexSize = gameOverTexture.getSize();
    float scaleOX = static_cast<float>(winSize.x) / overTexSize.x;
    float scaleOY = static_cast<float>(winSize.y) / overTexSize.y;
    gameOverSprite.setScale(scaleOX, scaleOY);

    // Hintergrundbild laden und leicht transparent machen
    if (!backgroundTexture.loadFromFile("assets/background/bg_main.png")) {
        std::cerr << "Fehler beim Laden des Hintergrundbildes!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 220)); // Alpha = 220 für Transparenz

    // Hintergrund skalieren auf Fenstergröße
    sf::Vector2u textureSize = backgroundTexture.getSize();
    float bgScaleX = static_cast<float>(winSize.x) / textureSize.x;
    float bgScaleY = static_cast<float>(winSize.y) / textureSize.y;
    backgroundSprite.setScale(bgScaleX, bgScaleY);

    spawnTreat(); // Erstes Treat platzieren
}

// Spawn-Funktion für ein neues Treat an zufälliger Position
void Game::spawnTreat() {
    auto treat = std::make_shared<Treat>();
    float x = static_cast<float>(rand() % (window.getSize().x - 32));
    float y = static_cast<float>(rand() % (window.getSize().y - 32));
    treat->setPosition({ x, y });
    treats.push_back(treat);
}

// Hauptspielschleife
void Game::run() {
    window.create(sf::VideoMode(800, 600), "WhiskerHunt", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        // Ereignisse abfragen
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Startbildschirm verlassen
            if (showStartScreen && event.type == sf::Event::KeyPressed) {
                showStartScreen = false;
            }

            // Game Over → Enter = Spiel zurücksetzen
            if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                isGameOver = false;
                player.reset();
                treats.clear();
                spawnTreat();
            }
        }

        // STARTSCREEN anzeigen
        if (showStartScreen) {
            window.clear();
            window.draw(startSprite);
            window.display();
            continue; // Spiel-Update überspringen
        }

        // GAME OVER SCREEN anzeigen
        if (isGameOver) {
            window.clear();
            window.draw(gameOverSprite);
            window.display();
            continue; // Spiel pausiert
        }

        // GAME LOOP
        float deltaTime = clock.restart().asSeconds();

        // Spielerbewegung und Spiellogik
        player.handleInput();
        player.update(deltaTime, window);

        // Treats aktualisieren + Kollisionen prüfen
        for (size_t i = 0; i < treats.size(); i++) {
            treats[i]->update(deltaTime);

            // Kollision mit Treat erkannt
            if (player.getBounds().intersects(treats[i]->getBounds())) {
                if (treats[i]->isPunishment()) {
                    player.loseFollower(); // Bestrafung
                }
                else {
                    player.addFollower(); // Belohnung
                }

                treats.erase(treats.begin() + i);
                spawnTreat();
                break;
            }
        }

        // RENDERING
        window.clear();
        window.draw(backgroundSprite);         // Hintergrund zeichnen
        for (const auto& treat : treats) {     // Treats zeichnen
            treat->draw(window);
        }
        player.draw(window);                   // Spieler und Follower zeichnen
        window.display();                      // Alles anzeigen
    }
}
