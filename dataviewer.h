#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QObject>
#include <QWidget>

class dataviewer : public QObject
{
    Q_OBJECT
public:
    explicit dataviewer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DATAVIEWER_H