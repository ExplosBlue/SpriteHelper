#include "spritedef.h"

SpriteDefinition::SpriteDefinition(QDomElement spriteElement)
{
    id = spriteElement.attribute("id", "999").toInt();
    name = spriteElement.attribute("name", "Invalid Name");
    notes = spriteElement.attribute("notes", "");

    QDomElement fieldElement = spriteElement.firstChild().toElement();
    while (!fieldElement.isNull())
    {
        Field* field = new Field();
        field->title = fieldElement.attribute("title", "INVALID TITLE");
        field->comment = fieldElement.attribute("comment", "");

        if (fieldElement.attribute("nybble") == "")
        {
            field->isBit = true;
            field->nybbles = fieldElement.attribute("bit");
        }
        else
        {
            field->isBit = false;
            field->nybbles = fieldElement.attribute("nybble");
        }

        if (fieldElement.tagName() == "value")
        {
            field->type = Field::Value;
        }

        else if (fieldElement.tagName() == "checkbox")
        {
            field->type = Field::Checkbox;
            field->mask = fieldElement.attribute("mask").toUInt();
        }

        else if (fieldElement.tagName() == "list")
        {
            field->type = Field::List;
            QDomNodeList entries = fieldElement.elementsByTagName("entry");
            for (int i = 0; i < entries.size(); i++)
            {
                QDomElement entryElement = entries.at(i).toElement();
                field->data.append(QString("%1=%2, ").arg(entryElement.attribute("value").toInt()).arg(entryElement.text()));
            }
            if (field->data.length() > 0)
                field->data.chop(2);
        }

        else if (fieldElement.tagName() == "bitfield")
        {
            field->type = Field::bitfield;
            field->startbit = fieldElement.attribute("startbit").toUInt();
            field->bitnum = fieldElement.attribute("bitnum").toUInt();
        }

        fields.append(field);

        fieldElement = fieldElement.nextSibling().toElement();
    }
}
