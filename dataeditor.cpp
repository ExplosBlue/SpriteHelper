#include "dataeditor.h"

#include <QLayout>
#include <QSizePolicy>

DataEditor::DataEditor(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mEditorLayout = new QGridLayout();

    // Setup Widgets
    mSpriteID = new QLabel();
    mSpriteID->setText("<b>Editing Sprite (NONE)</b>");
    mEditorLayout->addWidget(mSpriteID, 0, 1, 1, 1, Qt::AlignLeft);

    mEditorLayout->addWidget(new QLabel("Name:"), 1, 0, 1, 1, Qt::AlignRight);
    mSpriteName = new QLineEdit();
    connect(mSpriteName, SIGNAL(textEdited(QString)), this, SLOT(handleNameChanged(QString)));
    mEditorLayout->addWidget(mSpriteName, 1, 1);

    mEditorLayout->addWidget(new QLabel("Notes:"), 2, 0, 1, 1, Qt::AlignRight);
    mSpriteNotes = new QTextEdit();
    mSpriteNotes->setMaximumHeight(150);
    connect(mSpriteNotes, SIGNAL(textChanged()), this, SLOT(handleNotesChanged()));
    mEditorLayout->addWidget(mSpriteNotes, 2, 1);

    mFieldContainer = new FieldContainer();
    connect(mFieldContainer, SIGNAL(removeFieldFromDef(Field*)), this, SLOT(handleRemoveField(Field*)));

    mFieldScrollArea = new QScrollArea();
    mFieldScrollArea->setWidgetResizable(true);
    mFieldScrollArea->setWidget(mFieldContainer);
    mEditorLayout->addWidget(mFieldScrollArea, 3, 0, 1, 2);

    mAddFieldButton = new QPushButton();
    mAddFieldButton->setText("Insert Field");
    connect(mAddFieldButton, SIGNAL(pressed()), this, SLOT(handleAddField()));
    mEditorLayout->addWidget(mAddFieldButton, 4, 0, 1, 2);

    this->setLayout(mEditorLayout);
    this->setMinimumWidth(400);
}

DataEditor::~DataEditor()
{
    delete mSpriteID;
    delete mSpriteName;
    delete mSpriteNotes;
    delete mFieldContainer;
    delete mFieldScrollArea;
    delete mAddFieldButton;
}

void DataEditor::loadData(SpriteDefinition* spriteDef)
{
    if (mFieldContainer->getFieldCount() > 0)
    {
        foreach (DataField* field, mFieldContainer->getFields())
        {
            mFieldContainer->removeField(field);
            delete field;
        }
    }

    mSpriteDefPtr = spriteDef;

    mSpriteID->setText(QString("<b>Editing Sprite (%1)</b>").arg(mSpriteDefPtr->getID()));
    mSpriteName->setText(mSpriteDefPtr->getName());
    mSpriteNotes->setPlainText(mSpriteDefPtr->getNotes());

    for (int i = 0; i < mSpriteDefPtr->getFieldCount(); i++)
    {
        DataField* datafield = new DataField(mSpriteDefPtr->getFieldPtr(i));
        mFieldContainer->addField(datafield);
    }

    // TODO: LOAD + SHOW EXSISTING INGAME SPRITEDATA
}

void DataEditor::handleNameChanged(QString newName)
{
    mSpriteDefPtr->setName(newName);
    emit updateSpriteTreeNames(mSpriteDefPtr->getID(), newName);
}

void DataEditor::handleNotesChanged()
{
    mSpriteDefPtr->setNotes(mSpriteNotes->toPlainText());
}

void DataEditor::handleRemoveField(Field* field)
{
    mSpriteDefPtr->removeField(field);
    mFieldScrollArea->update();
    delete field;
}

void DataEditor::handleAddField()
{
    Field* field = new Field();
    field->type = Field::Value;
    field->isBit = false;
    field->title = "New Field";
    field->nybbles = "0";

    mSpriteDefPtr->addField(field);

    DataField* dataField = new DataField(field);
    mFieldContainer->addField(dataField);
}

DataField::DataField(Field* field, QWidget* parent) : QWidget(parent)
{
    mFieldPtr = field;

    QHBoxLayout* fieldLayout = new QHBoxLayout();

    mFieldName = new QLineEdit();
    mFieldName->setText(field->title);
    mFieldName->setToolTip("Title");
    mFieldName->setCursorPosition(0);
    mFieldName->setMaximumWidth(150);
    connect(mFieldName, SIGNAL(textEdited(QString)), this, SLOT(handleNameChange(QString)));
    fieldLayout->addWidget(mFieldName);

    mNybbles = new QLineEdit();
    mNybbles->setText(field->nybbles);
    if (field->isBit)
        mNybbles->setToolTip("Bit");
    else
        mNybbles->setToolTip("Nybble");
    QRegExpValidator *nybbleValidator = new QRegExpValidator(QRegExp("(\\d*|\\d*[-]\\d*)+"), this);
    mNybbles->setValidator(nybbleValidator);
    mNybbles->setMaximumWidth(60);
    mNybbles->setCursorPosition(0);
    connect(mNybbles, SIGNAL(textEdited(QString)), this, SLOT(handleNybbleChange(QString)));
    fieldLayout->addWidget(mNybbles);

    mBitFlag = new QCheckBox();
    mBitFlag->setChecked(field->isBit);
    mBitFlag->setToolTip("Uses Bits");
    connect(mBitFlag, SIGNAL(toggled(bool)), this, SLOT(handleBitFlagChanged(bool)));
    fieldLayout->addWidget(mBitFlag);

    mStartbit = new QSpinBox();
    mStartbit->setValue(field->startbit);
    mStartbit->setToolTip("StartBit");
    mStartbit->setRange(1, 96);
    mStartbit->setMaximumWidth(60);
    connect(mStartbit, SIGNAL(valueChanged(int)), this, SLOT(handleStartbitChange(int)));
    fieldLayout->addWidget(mStartbit);

    mBitnum = new QSpinBox();
    mBitnum->setValue(field->bitnum);
    mBitnum->setToolTip("BitNum");
    mBitnum->setRange(1, 96);
    mBitnum->setMaximumWidth(60);
    connect(mBitnum, SIGNAL(valueChanged(int)), this, SLOT(handleBitnumChange(int)));
    fieldLayout->addWidget(mBitnum);

    mFieldType = new QComboBox();
    mFieldType->addItem("List");
    mFieldType->addItem("Value");
    mFieldType->addItem("Checkbox");
    mFieldType->addItem("Bitfield");
    mFieldType->setCurrentIndex(field->type);
    mFieldType->setToolTip("Type");
    connect(mFieldType, SIGNAL(currentIndexChanged(int)), this, SLOT(handleTypeChange(int)));
    fieldLayout->addWidget(mFieldType);

    mMask = new QSpinBox();
    mMask->setValue(field->mask);
    mMask->setToolTip("Checkbox Mask");
    mMask->setRange(1, 255);
    mMask->setMaximumWidth(60);
    connect(mStartbit, SIGNAL(valueChanged(int)), this, SLOT(handleMaskChange(int)));
    fieldLayout->addWidget(mMask);

    mFieldValue = new QLineEdit();
    mFieldValue->setText(field->data);
    mFieldValue->setToolTip("List Entries");
    mFieldValue->setCursorPosition(0);
    connect(mFieldValue, SIGNAL(textEdited(QString)), this, SLOT(handleValueChange(QString)));
    fieldLayout->addWidget(mFieldValue);

    mFieldComment = new QLineEdit();
    mFieldComment->setText(field->comment);
    mFieldComment->setToolTip("Comment");
    mFieldComment->setCursorPosition(0);
    connect(mFieldComment, SIGNAL(textEdited(QString)), this, SLOT(handleCommentChange(QString)));
    fieldLayout->addWidget(mFieldComment);

    mDeleteButton = new QPushButton();
    mDeleteButton->setText("");
    mDeleteButton->setToolTip("Delete Field");
    mDeleteButton->setIcon(QIcon(QCoreApplication::applicationDirPath() + "/data/icons/delete.png"));
    connect(mDeleteButton, SIGNAL(pressed()), this, SLOT(handleDeletePressed()));
    fieldLayout->addWidget(mDeleteButton);

    this->setLayout(fieldLayout);

    updateLayout();
}

DataField::~DataField()
{
    delete mFieldName;
    delete mNybbles;
    delete mBitFlag;
    delete mStartbit;
    delete mBitnum;
    delete mFieldType;
    delete mMask;
    delete mFieldValue;
    delete mFieldComment;
    delete mDeleteButton;
}

void DataField::handleNameChange(QString newName)
{
    mFieldName->setToolTip(newName);
    mFieldPtr->title = newName;
}

void DataField::handleNybbleChange(QString newNybble)
{
    mFieldPtr->nybbles = newNybble;
}

void DataField::handleTypeChange(int typeID)
{
    mFieldPtr->type = Field::fieldType(typeID);

    updateLayout();
}

void DataField::handleValueChange(QString newVal)
{
    mFieldValue->setToolTip(newVal);
    mFieldPtr->data = newVal;
}

void DataField::handleCommentChange(QString newComment)
{
    mFieldComment->setToolTip(newComment);
    mFieldPtr->comment = newComment;
}

void DataField::handleBitFlagChanged(bool state)
{
    if (state)
        mNybbles->setToolTip("Bit");
    else
        mNybbles->setToolTip("Nybble");

    mFieldPtr->isBit = state;
}

void DataField::handleMaskChange(int newMask)
{
    mFieldPtr->mask = newMask;
}

void DataField::handleBitnumChange(int newBitnum)
{
    mFieldPtr->bitnum = newBitnum;
}

void DataField::handleStartbitChange(int newStartBit)
{
    mFieldPtr->startbit = newStartBit;
}

void DataField::handleDeletePressed()
{
    emit deleteField(this);
}

void DataField::updateLayout()
{
    switch (mFieldType->currentIndex())
    {
    case 0: // Lists
        mNybbles->setVisible(true);
        mBitFlag->setVisible(true);
        mStartbit->setVisible(false);
        mBitnum->setVisible(false);
        mMask->setVisible(false);
        mFieldValue->setVisible(true);
        break;

    case 1: // Value
        mNybbles->setVisible(true);
        mBitFlag->setVisible(true);
        mStartbit->setVisible(false);
        mBitnum->setVisible(false);
        mMask->setVisible(false);
        mFieldValue->setVisible(false);
        break;

    case 2: // Checkbox
        mNybbles->setVisible(true);
        mBitFlag->setVisible(true);
        mStartbit->setVisible(false);
        mBitnum->setVisible(false);
        mMask->setVisible(true);
        mFieldValue->setVisible(false);
        break;

    case 3: // Bitfield
        mNybbles->setVisible(false);
        mBitFlag->setVisible(false);
        mStartbit->setVisible(true);
        mBitnum->setVisible(true);
        mMask->setVisible(false);
        mFieldValue->setVisible(false);
        break;
    }
}

FieldContainer::FieldContainer(QWidget* parent) : QWidget(parent)
{
    mFieldLayout = new QVBoxLayout();
    this->setLayout(mFieldLayout);
    // TODO: ADD A HEADER ROW
}

FieldContainer::~FieldContainer()
{
    delete mFieldLayout;
}

void FieldContainer::addField(DataField *dataField)
{
    mFields.append(dataField);
    mFieldLayout->addWidget(dataField);
    mFieldLayout->setAlignment(dataField, Qt::AlignTop);
    connect(dataField, SIGNAL(deleteField(DataField*)), this, SLOT(handleDeleteField(DataField*)));
}

void FieldContainer::removeField(DataField *dataField)
{
    mFieldLayout->removeWidget(dataField);
    mFields.removeOne(dataField);
}

void FieldContainer::handleDeleteField(DataField* dataField)
{
    removeField(dataField);
    emit removeFieldFromDef(dataField->getFieldPtr());
    delete dataField;
}
