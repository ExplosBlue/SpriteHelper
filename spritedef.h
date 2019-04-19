#ifndef SPRITEDEF_H
#define SPRITEDEF_H

#include <QString>
#include <QMap>
#include <QtXml>

class Field
{
public:

    enum fieldType
    {
        List,
        Value,
        Checkbox,
        bitfield
    };

    Field() {}
    QString title;
    QString comment;
    QString data;
    fieldType type;
    QString nybbles;

    int mask = 1;
    int startbit = 1;
    int bitnum = 1;

    bool isBit = false;
};

class SpriteDefinition
{
public:
    SpriteDefinition(QDomElement spriteElement);

    QString getName() { return name; }
    int getID() { return id; }
    QString getNotes() { return notes; }
    Field* getFieldPtr(int id) { return fields[id]; }
    int getFieldCount() { return fields.size(); }
    QList<Field*> getFields() { return fields; }

    void setName(QString newName) { name = newName; }
    void setNotes(QString newNotes) { notes = newNotes; }

    void addField(Field* field) { fields.append(field); }
    void removeField(Field* field) { fields.removeOne(field); }
    void moveFieldUp(Field* field) { fields.move(fields.indexOf(field), fields.indexOf(field)-1); }
    void moveFieldDown(Field* field) { fields.move(fields.indexOf(field), fields.indexOf(field)+1); }
private:
    QString name;
    int id;
    QString notes;
    QList<Field*> fields;
};

#endif // SPRITEDEF_H
