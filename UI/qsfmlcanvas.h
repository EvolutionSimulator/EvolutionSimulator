#pragma once

#include "qwidget.h"
#include "qtimer.h"
#include <SFML/Graphics.hpp>

class QSFMLCanvas : public QWidget, public sf::RenderWindow
{
public :

    QSFMLCanvas(QWidget* Parent);
    virtual ~QSFMLCanvas() { };

    void SetRefreshInterval(int milliseconds);

private :
    virtual void OnInit() { };
    virtual void OnUpdate() { };

    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);
    virtual void paintEvent(QPaintEvent*);

    QTimer timer_;

};
