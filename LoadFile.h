#ifndef LOADFILE_H
#define LOADFILE_H
#include <QObject>
#include <iostream>
class LoadFile : public QObject
{
    Q_OBJECT
public:
    LoadFile(QObject *parent = nullptr);
    Q_INVOKABLE void loadFile(QString fileUrl);
};

#endif // LOADFILE_H
