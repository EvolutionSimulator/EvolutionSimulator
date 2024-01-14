#ifndef GRAPH_MANAGER_H
#define GRAPH_MANAGER_H

#include <QObject>
#include <QWidget>
#include "engine.h"

class GraphManager : public QObject {
  Q_OBJECT  // Enable signal and slot mechanism

public:
  explicit GraphManager(QWidget* parent, Engine* engine);

public slots:
  void DrawCreaturesOverTimeGraph();
  void DrawCreaturesSizeOverTimeGraph();
  void handleDropdownSelection(int index);

signals:
  void someEventHappened(); // Example signal

private:
  QWidget* parent_;
  Engine* engine_;
};



#endif // GRAPH_MANAGER_H
