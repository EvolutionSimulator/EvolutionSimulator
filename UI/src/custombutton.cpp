#include "CustomButton.h"
#include <QMouseEvent>
#include <QPainter>

CustomButton::CustomButton(QWidget* parent) : QWidget(parent) {
    // Set up the SFML render window
    renderWindow = new sf::RenderWindow(reinterpret_cast<sf::WindowHandle>(this->winId()));

    // Load the texture from the resource
    if (!buttonTexture.loadFromFile(":/Toggle_button_sprite.png")) {
        qDebug() << "Failed to load sprite sheet from resource!";
        return;
    }

    // Define the size of each sprite
    int spriteWidth = 256;
    int spriteHeight = 256;

    // Define the texture rectangles for each sprite
    sf::IntRect sprite1Rect(0, 0, spriteWidth, spriteHeight);         // Top-left sprite
    sf::IntRect sprite2Rect(spriteWidth, 0, spriteWidth, spriteHeight); // Top-right sprite
    sf::IntRect sprite3Rect(0, spriteHeight, spriteWidth, spriteHeight); // Bottom-left sprite
    sf::IntRect sprite4Rect(spriteWidth, spriteHeight, spriteWidth, spriteHeight); // Bottom-right sprite

    // Set the texture rect for the sprite you want to display
    buttonSprite.setTextureRect(sprite1Rect); // if you want to display the first sprite
}


CustomButton::~CustomButton() {
    delete renderWindow;
}

void CustomButton::paintEvent(QPaintEvent* event) {
    // Make SFML compatible with QWidget painting
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(rect(), Qt::transparent);
    painter.end();

    // Activate the SFML render window
    renderWindow->setActive(true);

    // Clear, draw your sprite, and display
    renderWindow->clear(sf::Color::Transparent);
    renderWindow->draw(buttonSprite);
    renderWindow->display();
}

void CustomButton::mousePressEvent(QMouseEvent* event) {
    // Check if the sprite is clicked
    sf::Vector2i sfmlPos(event->position().x(), event->position().y());
    if (buttonSprite.getGlobalBounds().contains(sfmlPos.x, sfmlPos.y)) {
        emit clicked();
    }
}
