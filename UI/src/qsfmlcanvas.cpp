#include "qsfmlcanvas.h"
#include "config.h"

QSFMLCanvas::QSFMLCanvas(QWidget* Parent) : QWidget(Parent) {
  float scaleFactor = this->devicePixelRatioF(); // Get the device pixel ratio

  // Scale the map width and height according to the pixel ratio
  int scaledWidth = static_cast<int>(settings::environment::kMapWidth/scaleFactor);
  int scaledHeight = static_cast<int>(settings::environment::kMapHeight/scaleFactor);

  // Set the fixed size with the scaled dimensions
  setFixedSize(scaledWidth, scaledHeight);


  // Setup some states to allow direct rendering into the widget
  setAttribute(Qt::WA_PaintOnScreen);
  setAttribute(Qt::WA_OpaquePaintEvent);
  setAttribute(Qt::WA_NoSystemBackground);

  // Set strong focus to enable keyboard events to be received
  setFocusPolicy(Qt::StrongFocus);

  // Setup the timer with a specified refresh interval (0 means it updates as
  // fast as possible)
  timer_.setInterval(0);
}

void QSFMLCanvas::SetRefreshInterval(int milliseconds) {
  timer_.setInterval(milliseconds);
}

#ifdef Q_WS_X11
#include <Qt/qx11info_x11.h>
#include <X11/Xlib.h>
#endif

void QSFMLCanvas::showEvent(QShowEvent*) {
  if (!isOpen()) {
// Under X11, we need to flush the commands sent to the server to ensure that
// SFML will get an updated view of the windows
#ifdef Q_WS_X11
    XFlush(QX11Info::display());
#endif

    sf::RenderWindow::create((sf::WindowHandle)(winId()));

    OnInit();

    // Setup the timer to trigger a refresh at specified framerate
    connect(&timer_, SIGNAL(timeout()), this, SLOT(repaint()));
    timer_.start();
  }
}

// I have no clue what this does exactly but if its not present then the widget
// flickers like crazy
QPaintEngine* QSFMLCanvas::paintEngine() const { return nullptr; }

// called when the widget is repainted
void QSFMLCanvas::paintEvent(QPaintEvent*) {
  OnUpdate();

  display();
}
