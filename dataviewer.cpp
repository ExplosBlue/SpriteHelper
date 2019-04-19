#include "dataviewer.h"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QFile>
#include <QHeaderView>

DataViewer::DataViewer(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout;

    mTable = new QTableWidget();
    mTable->setColumnCount(2);
    mTable->setHorizontalHeaderLabels(QStringList() << "Data" << "Used In");
    mTable->horizontalHeader()->setStretchLastSection(true);
    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTable->verticalHeader()->hide();
    mTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    mTable->setTextElideMode(Qt::ElideNone);
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mTable->resizeRowToContents(true);
    layout->addWidget(mTable);

    this->setLayout(layout);

    QFile file;
    file.setFileName("data/exsistingdata.json");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    mDoc = QJsonDocument::fromJson(file.readAll());
    file.close();
}

DataViewer::~DataViewer()
{
    delete mTable;
}

void DataViewer::setSprite(int id)
{
    // Clear the table
    mTable->clearContents();
    mTable->setRowCount(0);

    // Get sprite
    QJsonObject sprite = mDoc.object().value(QString("Sprite %1").arg(id)).toObject();

    // Get every data entry for sprite
    foreach(QString key, sprite.keys())
    {
        int row = mTable->rowCount();
        mTable->insertRow(row);

        mTable->setItem(row, 0, new QTableWidgetItem(tr("%1").arg(key)));

        QJsonArray levels = sprite.value(key).toArray();

        QString levelString;
        foreach (QJsonValue level, levels)
        {
            levelString.append(level.toString() + ", ");
        }
        levelString.chop(2);

        mTable->setItem(row, 1, new QTableWidgetItem(tr("%1").arg(levelString)));
    }
}
