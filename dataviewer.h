#ifndef DATAVIEWER_H
#define DATAVIEWER_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>

#include <QJsonDocument>

class DataViewer : public QWidget
{
    Q_OBJECT
public:
    explicit DataViewer(QWidget *parent = nullptr);
    ~DataViewer();

    void setSprite(int id);

private:
    QJsonDocument mDoc;

    QTableWidget* mTable;
};

#endif // DATAVIEWER_H
