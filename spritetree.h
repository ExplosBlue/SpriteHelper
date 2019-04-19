#ifndef SPRITETREE_H
#define SPRITETREE_H

#include "spritedef.h"

#include <QWidget>
#include <QTreeWidget>
#include <QLineEdit>

class SpriteTree : public QWidget
{
    Q_OBJECT
public:
    explicit SpriteTree(QWidget *parent = nullptr);
    ~SpriteTree();

    void populateTree(QList<SpriteDefinition*>* spriteDefs);

    void changeSpriteName(int id, QString name);

signals:
    void currentSpriteChanged(int);

private slots:
    void handleItemChanged(QTreeWidgetItem*, int);
    void searchTree(QString);

private:
    QTreeWidget* mTreeWidget;
    QLineEdit* mSearchBox;
};

#endif // SPRITETREE_H
