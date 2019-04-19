#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include "spritedef.h"
#include "spritetree.h"
#include "dataeditor.h"
#include "dataviewer.h"

#include <QWidget>
#include <QLayout>
#include <QList>
#include <QtXml>

class EditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditorWidget(QWidget *parent = nullptr);
    ~EditorWidget();

    void loadXML(QDomDocument xml);
    void saveXML(QString path);

    void toggleExistingData(bool toggle);
    void toggleSpriteTree(bool toggle);

private slots:
    void changeSprite(int);
    void handleUpdateSpriteNames(int, QString);

private:
    SpriteTree* mSpriteTree;
    DataEditor* mDataEditor;
    DataViewer* mDataViewer;

    QList<SpriteDefinition*>* mSpriteDefs;
};

#endif // EDITORWIDGET_H
