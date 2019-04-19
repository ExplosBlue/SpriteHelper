#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include "spritedef.h"

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>

class DataField : public QWidget
{
    Q_OBJECT
public:
    explicit DataField(Field* field, QWidget *parent = nullptr);
    ~DataField();

    Field* getFieldPtr() { return mFieldPtr; }

signals:
    void deleteField(DataField*);

    void moveUp(DataField*);
    void moveDown(DataField*);

private slots:
    void handleNameChange(QString);
    void handleNybbleChange(QString);
    void handleBitFlagChanged(bool);
    void handleTypeChange(int);
    void handleValueChange(QString);
    void handleCommentChange(QString);
    void handleDeletePressed();
    void handleMaskChange(int);
    void handleBitnumChange(int);
    void handleStartbitChange(int);
    void handleMoveUpPressed();
    void handleMoveDownPressed();

private:

    void updateLayout();

    QLineEdit* mFieldName;
    QLineEdit* mNybbles;
    QCheckBox* mBitFlag;
    QSpinBox* mStartbit;
    QSpinBox* mBitnum;
    QComboBox* mFieldType;
    QSpinBox* mMask;
    QLineEdit* mFieldValue;
    QLineEdit* mFieldComment;
    QPushButton* mDeleteButton;

    QPushButton* mMoveUpButton;
    QPushButton* mMoveDownButton;

    Field* mFieldPtr;
};

class FieldContainer : public QWidget
{
    Q_OBJECT
public:
    explicit FieldContainer(QWidget *parent = nullptr);
    ~FieldContainer();

    void addField(DataField* dataField);
    void removeField(DataField* dataField);

    QList<DataField*> getFields() { return mFields; }
    int getFieldCount() { return mFields.count(); }

signals:
    void removeFieldFromDef(Field*);
    void moveFieldUp(Field*);
    void moveFieldDown(Field*);

private slots:
    void handleDeleteField(DataField* dataField);
    void handleMoveFieldUp(DataField* dataField);
    void handleMoveFieldDown(DataField* dataField);

private:
    QVBoxLayout* mFieldLayout;

    QList<DataField*> mFields;
};

class DataEditor : public QWidget
{
    Q_OBJECT
public:
    explicit DataEditor(QWidget *parent = nullptr);
    ~DataEditor();

    void loadData(SpriteDefinition* spriteDef);

signals:
    void updateSpriteTreeNames(int, QString);

private slots:
    void handleNameChanged(QString);
    void handleNotesChanged();
    void handleRemoveField(Field*);
    void handleMoveFieldUp(Field*);
    void handleMoveFieldDown(Field*);
    void handleAddField();

private:
    SpriteDefinition* mSpriteDefPtr;

    FieldContainer* mFieldContainer;

    QLabel* mSpriteID;
    QLineEdit* mSpriteName;
    QTextEdit* mSpriteNotes;
    QScrollArea* mFieldScrollArea;
    QPushButton* mAddFieldButton;
};

#endif // DATAEDITOR_H
