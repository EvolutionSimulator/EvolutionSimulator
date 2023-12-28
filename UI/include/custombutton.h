#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QWidget>
#include <SFML/Graphics.hpp>

class CustomButton : public QWidget
{
    Q_OBJECT
public:
    explicit CustomButton(QWidget* parent = nullptr);
    ~CustomButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    sf::RenderWindow* renderWindow;
    sf::Sprite buttonSprite;
    sf::Texture buttonTexture;

signals:
    void clicked();
};

#endif // CUSTOMBUTTON_H
