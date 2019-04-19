#include "editorwidget.h"

#include <QMessageBox>

EditorWidget::EditorWidget(QWidget *parent) : QWidget(parent)
{
    mSpriteDefs = new QList<SpriteDefinition*>();

    // Initialise Editor Widgets
    QGridLayout* editorLayout = new QGridLayout();

    mSpriteTree = new SpriteTree();
    connect(mSpriteTree, SIGNAL(currentSpriteChanged(int)), this, SLOT(changeSprite(int)));
    mSpriteTree->setDisabled(true);
    editorLayout->addWidget(mSpriteTree, 0, 0);
    editorLayout->setColumnStretch(0, 2);

    mDataEditor = new DataEditor();
    mDataEditor->setDisabled(true);
    connect(mDataEditor, SIGNAL(updateSpriteTreeNames(int, QString)), this, SLOT(handleUpdateSpriteNames(int, QString)));
    editorLayout->addWidget(mDataEditor, 0, 1);
    editorLayout->setColumnStretch(1, 4);

    mDataViewer = new DataViewer();
    mDataViewer->setDisabled(true);
    editorLayout->addWidget(mDataViewer, 0, 2);
    editorLayout->setColumnStretch(2, 3);

    this->setLayout(editorLayout);
}

EditorWidget::~EditorWidget()
{
    delete mSpriteTree;
    delete mDataEditor;
    delete mDataViewer;
    delete mSpriteDefs;
}

void EditorWidget::loadXML(QDomDocument xml)
{
    QDomElement spriteElement = xml.documentElement().firstChild().toElement();
    while (!spriteElement.isNull())
    {
        mSpriteDefs->append(new SpriteDefinition(spriteElement));
        spriteElement = spriteElement.nextSiblingElement();
    }

    mSpriteTree->populateTree(mSpriteDefs);
    mSpriteTree->setEnabled(true);

}

void EditorWidget::saveXML(QString path)
{
    if (!path.endsWith(".xml"))
        path.append(".xml");

    QFile outFile(path);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug( "Failed to open file for writing." );
        return;
    }
    QXmlStreamWriter stream(&outFile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    // Write xml
    stream.writeStartElement("sprites");

    // create entry for each sprite
    foreach (SpriteDefinition* def, *mSpriteDefs)
    {
        if (def->getName().isEmpty())
        {
            QMessageBox::information(this, "Invalid Name!", QString("Sprite %1 has an invalid name. Saving will now be aborted!").arg(def->getID()), QMessageBox::Ok);
            outFile.close();
            outFile.remove();
            return;
        }

        stream.writeStartElement("sprite");

        stream.writeAttribute("id", QString::number(def->getID()));
        stream.writeAttribute("name", QString(def->getName()));

        if(!def->getNotes().isEmpty())
            stream.writeAttribute("notes", QString(def->getNotes()));

        // write Fields
        foreach (Field* field, def->getFields())
        {
            switch (field->type)
            {
            case Field::List:
            {
                if ((field->title.isEmpty()) || (field->nybbles.isEmpty()))
                {
                    QMessageBox::information(this, "Invalid Field!", QString("Sprite %1 contains an invalid field, This field will be skipped.").arg(def->getID()), QMessageBox::Ok);
                    continue;
                }

                stream.writeStartElement("list");

                if (field->isBit)
                    stream.writeAttribute("bit", field->nybbles);
                else
                    stream.writeAttribute("nybble", field->nybbles);

                stream.writeAttribute("title", field->title);

                if (!field->comment.isEmpty())
                    stream.writeAttribute("comment", field->comment);

                QStringList entries = field->data.split((QRegExp("[,](?= \\d*=)")));

                // Handle List Entries
                foreach (QString entryString, entries)
                {
                    if (entryString.startsWith(" "))
                        entryString.remove(0, 1);

                    QStringList val = entryString.split("=");

                    stream.writeStartElement("entry");

                    stream.writeAttribute("value", val[0]);

                    if (!val[1].isEmpty())
                        stream.writeCharacters(val[1]);

                    stream.writeEndElement(); // entry
                }

                stream.writeEndElement(); // list
            }
                break;
            case Field::Value:
            {
                if ((field->title.isEmpty()) || (field->nybbles.isEmpty()))
                {
                    QMessageBox::information(this, "Invalid Field!", QString("Sprite %1 contains an invalid field, This field will be skipped.").arg(def->getID()), QMessageBox::Ok);
                    continue;
                }

                stream.writeStartElement("value");

                if (field->isBit)
                    stream.writeAttribute("bit", field->nybbles);
                else
                    stream.writeAttribute("nybble", field->nybbles);

                stream.writeAttribute("title", field->title);

                if (!field->comment.isEmpty())
                    stream.writeAttribute("comment", field->comment);

                stream.writeEndElement(); // value
            }
                break;
            case Field::Checkbox:
            {
                if ((field->title.isEmpty()) || (field->nybbles.isEmpty()))
                {
                    QMessageBox::information(this, "Invalid Field!", QString("Sprite %1 contains an invalid field, This field will be skipped.").arg(def->getID()), QMessageBox::Ok);
                    continue;
                }

                stream.writeStartElement("checkbox");

                if (field->isBit)
                    stream.writeAttribute("bit", field->nybbles);
                else
                    stream.writeAttribute("nybble", field->nybbles);

                stream.writeAttribute("title", field->title);

                if (!(field->mask <= 1))
                    stream.writeAttribute("mask", QString::number(field->mask));

                if (!field->comment.isEmpty())
                    stream.writeAttribute("comment", field->comment);

                stream.writeEndElement(); // checkbox
            }
                break;
            case Field::bitfield:
            {
                if ((field->title.isEmpty()))
                {
                    QMessageBox::information(this, "Invalid Field!", QString("Sprite %1 contains an invalid field, This field will be skipped.").arg(def->getID()), QMessageBox::Ok);
                    continue;
                }

                stream.writeStartElement("bitfield");

                stream.writeAttribute("startbit", QString::number(field->startbit));
                stream.writeAttribute("bitnum", QString::number(field->bitnum));
                stream.writeAttribute("title", field->title);

                if (!field->comment.isEmpty())
                    stream.writeAttribute("comment", field->comment);

                stream.writeEndElement(); // checkbox
            }
                break;
            }
        }

        stream.writeEndElement(); // sprite

    }
    stream.writeEndElement(); // sprites

    outFile.close();
}

void EditorWidget::changeSprite(int id)
{
    mDataEditor->setEnabled(false);
    mDataViewer->setEnabled(false);
    mSpriteTree->setEnabled(false);

    mDataEditor->loadData(mSpriteDefs->at(id));
    mDataViewer->setSprite(id);

    mDataEditor->setEnabled(true);
    mDataViewer->setEnabled(true);
    mSpriteTree->setEnabled(true);
}

void EditorWidget::handleUpdateSpriteNames(int id, QString name)
{
    mSpriteTree->changeSpriteName(id, name);
}

void EditorWidget::toggleExistingData(bool toggle)
{
    if (toggle)
        dynamic_cast<QGridLayout*>(this->layout())->setColumnStretch(2, 3);
    else
        dynamic_cast<QGridLayout*>(this->layout())->setColumnStretch(2, 0);

    mDataViewer->setVisible(toggle);
}

void EditorWidget::toggleSpriteTree(bool toggle)
{
    if (toggle)
        dynamic_cast<QGridLayout*>(this->layout())->setColumnStretch(0, 2);
    else
        dynamic_cast<QGridLayout*>(this->layout())->setColumnStretch(0, 0);

    mSpriteTree->setVisible(toggle);
}
