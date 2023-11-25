#include "qsfmlcanvas.h"

QSFMLCanvas::QSFMLCanvas(QWidget* Parent) :
    QWidget       (Parent)
{
    // Setup some states to allow direct rendering into the widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Set strong focus to enable keyboard events to be received
    setFocusPolicy(Qt::StrongFocus);

    // Setup the timer
    timer_.setInterval(0);
}

void QSFMLCanvas::SetRefreshInterval(int milliseconds)
{
    timer_.setInterval(milliseconds);
}

#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif


void QSFMLCanvas::showEvent(QShowEvent*)
{
    if (!isOpen())
    {
        // Under X11, we need to flush the commands sent to the server to ensure that
        // SFML will get an updated view of the windows
        #ifdef Q_WS_X11
            XFlush(QX11Info::display());
        #endif

        sf::RenderWindow::create(reinterpret_cast<sf::WindowHandle>(winId()));

        OnInit();

        // Setup the timer to trigger a refresh at specified framerate
        connect(&timer_, SIGNAL(timeout()), this, SLOT(repaint()));
        timer_.start();
    }
}

QPaintEngine* QSFMLCanvas::paintEngine() const
{
    return nullptr;
}

void QSFMLCanvas::paintEvent(QPaintEvent*)
{
    OnUpdate();

    display();
}
