#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMouseEvent>
#include <QPainter>
#include <QUuid>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include "creaturetracker.h"
#include <utility> // For std::pair
#include <vector>
#include <deque>
#include <string>
#include <algorithm>

CreatureTracker::CreatureTracker(const Creature& creature)
    : trackedCreature_(creature), window_(sf::VideoMode(400, 300), "Creature Tracker") {
    QFile resourceFile(":/font.ttf");
    if (!resourceFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open font resource!";
        return;
    }

    // Generate a unique temporary filename
    QString tempFileName = "temp_font_" +
                           QUuid::createUuid().toString(QUuid::WithoutBraces) +
                           ".ttf";
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

    if (!font_.loadFromFile(tempFilePath.toStdString())) {
        qDebug() << "Failed to load font from file!";
        QFile::remove(tempFilePath);
        return;
    }



    // Clean up the temporary file after use
    QFile::remove(tempFilePath);
}

void CreatureTracker::UpdateData(float newValue) {
    historicalData.push_back({newValue});
    std::cout << "Data updated: " << newValue << std::endl;

    if (historicalData.size() > 100) { // Limit the size to last 100 steps
        historicalData.pop_front();
    }
}

void CreatureTracker::DrawGraph() {
    if (historicalData.empty()) return;

    const float graphWidth = 300.0f;
    const float graphHeight = 100.0f;
    const float offsetX = 50.0f;
    const float offsetY = 150.0f;

    float minValue = std::min_element(historicalData.begin(), historicalData.end(),
                                      [](const DataPoint& a, const DataPoint& b) { return a.value < b.value; })->value;
    float maxValue = std::max_element(historicalData.begin(), historicalData.end(),
                                      [](const DataPoint& a, const DataPoint& b) { return a.value < b.value; })->value;

    sf::VertexArray lines(sf::LinesStrip, historicalData.size());
    for (size_t i = 0; i < historicalData.size(); ++i) {
        float x = offsetX + (i / (float)historicalData.size()) * graphWidth;
        float y = offsetY + ((historicalData[i].value - minValue) / (maxValue - minValue)) * graphHeight;
        lines[i] = sf::Vertex(sf::Vector2f(x, graphHeight - y + offsetY), sf::Color::Red);
    }

    window_.draw(lines);
    // Optionally, add more graphical elements like axes, labels, etc.
}

void CreatureTracker::DrawInfo() {
    sf::Text text;
    text.setFont(font_);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::White);
    text.setString("ID: " + std::to_string(trackedCreature_.GetID()));
    text.setPosition(10, 10);
    window_.draw(text);

    DrawGraph();
}

void CreatureTracker::Show() {
    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
        }

        window_.clear();
        DrawInfo();
        window_.display();
    }
}
