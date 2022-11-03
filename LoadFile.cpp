#include "LoadFile.h"

LoadFile::LoadFile(QObject *parent)
    : QObject(parent)
{

}

void LoadFile::loadFile(QString fileUrl)
{
    std::string filePath = fileUrl.toStdString();
    std::string pattern = "file://";
    filePath.erase(0, 7);
    std::cout << "fileUrl = " << filePath << "\n";
    std::string cmd = "cp " + filePath + " ~/.";
    system(cmd.c_str());
}
