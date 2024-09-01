#include "filemode.h"
#include <fstream>
#include <iostream>
#include <QString>
#include <QDirIterator>
#include <QtQml>

#include<thread>
#include<mutex>


NameFM::NameFM(QObject *parent) : QObject (parent){}
//---------------------------------------------------------------------------------------------------
NameFM::~NameFM(){}
//---------------------------------------------------------------------------------------------------
IFileMode::IFileMode(QObject *parent) : NameFM (parent){}
IFileMode::~IFileMode(){}
//---------------------------------------------------------------------------------------------------
FileMode::FileMode(QObject *parent) : IFileMode (parent),
    mask(".txt"), key(8,0)
{
    updateParameters();
    connect(&modTimer, &QTimer::timeout, this, &FileMode::searchAndMod);
    // connect(this, &FileMode::parametersChanged, this, [](){qDebug()<<"ASSS--------------------------";});
}
//---------------------------------------------------------------------------------------------------
FileMode::~FileMode(){}
//---------------------------------------------------------------------------------------------------
void FileMode::searchAndMod()
{
    status = true;
    updateParameters();

    //std::mutex mtx;
    //mtx.lock();
    //std::thread myThread1(&FileMode::updateParameters, this);
    // myThread1.join();
    //mtx.unlock();


    searchFiles();

    int count = hitList.size();
    for (int i = 0; i < count; i++)
        modFile(i);

    status = false;
    updateParameters();
}
//---------------------------------------------------------------------------------------------------
void FileMode::updateParameters()
{
    //qDebug() << "updateParameters" <<  status;
    QVariantMap statusMap;

    QString str = "0x";
    int size = key.size();
    for (int i = 0; i < size; i++) {
        QString tmpStr = QString::number(key[i],16);
        str.append((tmpStr.size() == 1) ? ("0" + tmpStr) : ( tmpStr));
    }

    statusMap["key"] = str ;
    statusMap["searchPath"] =  searchPath;
    statusMap["savePath"] = savePath ;
    statusMap["mask"] = mask ;
    statusMap["status"] = status ;
    statusMap["timeMode"] = timeMode ;

    this->parameters = statusMap;
    emit parametersChanged();
}
//---------------------------------------------------------------------------------------------------
void FileMode::stopModReplay()
{
    std::cout  << "stopModReplay" << std::endl;
    modTimer.stop();
    status = false;
    updateParameters();
}
//---------------------------------------------------------------------------------------------------
void FileMode::startModification(bool replay)
{
    std::cout  << "startModification " << replay << std::endl;
    //status = true;
    //updateParameters();

    if( ! replay)
        modTimer.singleShot(0, this, &FileMode::searchAndMod);
    else
        modTimer.start(static_cast<int>(timeMode) * 1000);
}
//---------------------------------------------------------------------------------------------------
void FileMode::modFile(int indexFile)
{
    using namespace std;

    ifstream in(hitList[indexFile].absoluteFilePath().toStdString(),
                ios_base::in | ios_base::binary);

    if(!in.is_open())
    {
        cout << hitList[indexFile].absoluteFilePath().toStdString()
             << "\t not open in ifstream!" << endl;
        status = false;
        updateParameters();
        return;
    }

    QByteArray dataFile;
    int keyArrSize = key.size();

    for (int i = 0; in.peek() != EOF; i++)
    {
        if(i >= keyArrSize)
            i = 0;
        dataFile.append(static_cast<char>(in.get()) ^ key.at(i));
    }

    in.close();
    if(delOriginFlag)
        std::remove(hitList[indexFile].
                    absoluteFilePath().toStdString().c_str());

    saveFile(dataFile, indexFile);
}
//---------------------------------------------------------------------------------------------------
QString FileMode::modRepit(QString fileName, const char* shell)
{
    auto find = [&](QString fileN, const char* shellN)
    {
        QDirIterator it(savePath, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QFileInfo file(it.next());
            if (file.isDir())
                continue;

            if (file.fileName().contains(fileN+shellN, Qt::CaseInsensitive)){
                return true;
            }
        }
        return false;
    };

    if(!find(fileName, shell))
        return fileName.append(shell);

    int counter = 0;
    while(1){
        counter ++;
        if(!find(fileName + "(" + QString::number(counter) + ")", shell))
            break;
    }

    fileName += ("(" + QString::number(counter) + ")" + shell);

    return fileName;
}
//---------------------------------------------------------------------------------------------------
void FileMode::saveFile(QByteArray& data, int indexFile)
{
    using namespace std;
    QString tmpName;

    if(nameModeFlag){
        char ext[30];
        _splitpath(hitList[indexFile].filePath().toStdString().c_str(),
                   nullptr, nullptr, nullptr,  ext);

        tmpName = modRepit((hitList[indexFile].fileName().replace(ext,"")),ext);
    }
    else {
        tmpName = hitList[indexFile].fileName();
    }

    ofstream out((savePath + tmpName).toStdString(),
                 ios_base::out | ios_base::binary);
    if(!out.is_open()){
        cout << hitList[indexFile].fileName().toStdString()
             << "\t open in ofstream !" << endl;
        status = false;
        updateParameters();
        return;
    }
    int sizeData = data.size();
    for (int i = 0; i < sizeData; i++)
        out.put(data.at(i));

    out.close();

}
//---------------------------------------------------------------------------------------------------
void FileMode::searchFiles()
{
    hitList.clear();
    QDirIterator it(searchPath, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QFileInfo file(it.next());
        if (file.isDir())
            continue;

        if (file.fileName().contains(mask, Qt::CaseInsensitive)){
            qDebug() << file.absoluteFilePath();
            hitList.append(file);
        }
    }
}
//---------------------------------------------------------------------------------------------------
void FileMode::setCheckState(NameFM::TypeCheckBox index, bool value)
{

    switch (index) {
    case NameFM::DEL_ORIGIN_FLAG : {
        delOriginFlag = value;
        break;
    }
    case NameFM::MOD_NAME_FLAG : {
        nameModeFlag = value;
        break;
    }
    case NameFM::TIMER_FLAG : {
        timerFlag = value;
        break;
    }
    }
}
//---------------------------------------------------------------------------------------------------
void FileMode::setParameters(NameFM::TypeParameters param, QString value)
{
    switch (param) {
    case NameFM::SEARCH_PATH : {
        searchPath = value;
        break;
    }
    case NameFM::SAVE_PATH : {
        savePath = value;
        break;
    }
    case NameFM::MASK : {
        mask = value;
        break;
    }
    case NameFM::KEY : {
        if(value == "")
            value = "00";
        strToQBArr(value);
        break;
    }
    case NameFM::TIME_MODE : {
        bool ok;
        timeMode = value.toUInt(&ok);
        if( !ok )
            timeMode = 0;
        break;
    }
    default: return;
    }
    updateParameters();
}
//---------------------------------------------------------------------------------------------------
void FileMode::strToQBArr(QString code)
{
    key.clear();
    QString codeEl;
    int size = code.size() - 1;
    for (int i = 0; i < size; i += 2)
    {
        codeEl = code.at(i);
        codeEl.append(code.at(i + 1));
        key.append(QByteArray::fromHex(codeEl.toUtf8()));
    }
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------
