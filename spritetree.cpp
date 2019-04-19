#include "spritetree.h"

#include <QLayout>
#include <QLabel>
#include <QHeaderView>

SpriteTree::SpriteTree(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* searchBoxLayout = new QHBoxLayout();

    QLabel* searchLabel = new QLabel("Search:");
    searchLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    searchBoxLayout->addWidget(searchLabel);

    mSearchBox = new QLineEdit();
    connect(mSearchBox, SIGNAL(textEdited(QString)), this, SLOT(searchTree(QString)));
    searchBoxLayout->addWidget(mSearchBox);

    layout->addLayout(searchBoxLayout);

    mTreeWidget = new QTreeWidget();
    mTreeWidget->setColumnCount(1);
    mTreeWidget->setHeaderHidden(true);
    mTreeWidget->setTextElideMode(Qt::TextElideMode::ElideNone);
    mTreeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    connect(mTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(handleItemChanged(QTreeWidgetItem*, int)));
    layout->addWidget(mTreeWidget);

    this->setLayout(layout);
    this->setMaximumWidth(400);
}

SpriteTree::~SpriteTree()
{
    delete mTreeWidget;
    delete mSearchBox;
}

void SpriteTree::populateTree(QList<SpriteDefinition*> *spriteDefs)
{
    for (int i = 0; i < spriteDefs->size(); i++)
    {
        QTreeWidgetItem* spriteDef = new QTreeWidgetItem();
        spriteDef->setText(0, QString("%1: %2").arg(i).arg(spriteDefs->at(i)->getName()));
        spriteDef->setData(0, Qt::UserRole, i);
        mTreeWidget->addTopLevelItem(spriteDef);
    }
    mTreeWidget->resizeColumnToContents(0);
}

void SpriteTree::handleItemChanged(QTreeWidgetItem* item, int)
{
    emit currentSpriteChanged(item->data(0, Qt::UserRole).toInt());
}

void SpriteTree::searchTree(QString query)
{
    foreach (QTreeWidgetItem* spriteDef, mTreeWidget->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard | Qt::MatchRecursive))
    {
        if (spriteDef->data(0, Qt::UserRole).toInt() != -1)
            spriteDef->setHidden(!spriteDef->text(0).contains(query, Qt::CaseInsensitive));
    }
}

void SpriteTree::changeSpriteName(int id, QString name)
{
    if (mTreeWidget->currentItem()->data(0, Qt::UserRole).toInt() != id)
        return;

    mTreeWidget->currentItem()->setText(0, QString("%1: %2").arg(id).arg(name));
    mTreeWidget->resizeColumnToContents(0);
}
