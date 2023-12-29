#include "CustomButton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QUuid>

CustomButton::CustomButton(QWidget* parent) : QSFMLCanvas(parent) {
    // Set up the SFML render window
    renderWindow = new sf::RenderWindow(reinterpret_cast<sf::WindowHandle>(this->winId()));
    QFile resourceFile(":/Toggle_button_sprite.png");
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open font resource!";
        return;
    }

    // Generate a unique temporary filename
    QString tempFileName = "Sprite_sheet" + QUuid::createUuid().toString(QUuid::WithoutBraces)
                           + ".png";
    QString tempFilePath = QDir::temp().absoluteFilePath(tempFileName);
    qDebug() << "Temporary file path:" << tempFilePath;

    QFile tempFile(tempFilePath);
    if (tempFile.exists()) {
        qDebug() << "Temporary file already exists. Deleting...";
        if (!tempFile.remove()) {
            qDebug() << "Failed to remove existing temporary file.";
            return;
        }
    }

    if (!resourceFile.copy(tempFilePath)) {
        qDebug() << "Failed to copy font to temporary file!";
        qDebug() << "Error:" << resourceFile.errorString();
        return;
    }
    resourceFile.close();

    if (!buttonTexture.loadFromFile(tempFilePath.toStdString())) {
        qDebug() << "Failed to load font from file!";
        QFile::remove(tempFilePath);
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
    buttonSprite.setTexture(buttonTexture);
    buttonSprite.setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight)); // Adjust the rectangle as needed
}


CustomButton::~CustomButton() {
    delete renderWindow;
}

void CustomButton::paintEvent(QPaintEvent* event) {
    // Activate the SFML render window
    renderWindow->setActive(true);

    // Clear, draw your sprite, and display
    renderWindow->clear(sf::Color::Transparent);
    renderWindow->draw(buttonSprite);
    renderWindow->display();

    // Call base class implementation
    QWidget::paintEvent(event);
}

void CustomButton::mousePressEvent(QMouseEvent* event) {
    // Check if the sprite is clicked
    sf::Vector2i sfmlPos(event->position().x(), event->position().y());
    if (buttonSprite.getGlobalBounds().contains(sfmlPos.x, sfmlPos.y)) {
        emit clicked();
    }
}
