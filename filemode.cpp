#include "filemode.h"

#include <QString>
#include <QDirIterator>
#include <QtQml>
#include <fstream>
#include <iostream>
#include<thread>


NameFM::NameFM(QObject *parent) : QObject (parent){}
//---------------------------------------------------------------------------------------------------
NameFM::~NameFM(){}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
IFileMode::IFileMode(QObject *parent) : NameFM (parent){}
//---------------------------------------------------------------------------------------------------
IFileMode::~IFileMode(){}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
FileMode::FileMode(QObject *parent) : IFileMode (parent),
    mask(".txt"), key(8,0)
{
    updateParameters();
    connect(&modTimer, &QTimer::timeout, this, &FileMode::onTimerTimeout);
}
//---------------------------------------------------------------------------------------------------
FileMode::~FileMode(){}
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
void FileMode::updateParameters()
{
    // запихиваем нужные для отображения параметры в мап и передаем в qml
    QVariantMap statusMap;

    QString str = "0x";
    int size = key.size();
    for (int i = 0; i < size; i++) {
        QString tmpStr = QString::number(key[i], 16);
        str.append((tmpStr.size() == 1) ? ("0" + tmpStr) : (tmpStr));
    }

    statusMap["key"] = str ;
    statusMap["searchPath"] =  searchPath;
    statusMap["savePath"] = savePath ;
    statusMap["mask"] = mask ;
    statusMap["status"] = status ;
    statusMap["timeMode"] = timeMode ;

    parameters = statusMap;
    emit parametersChanged();
}
//---------------------------------------------------------------------------------------------------
void FileMode::stopModReplay()
{
    modTimer.stop();
}
//---------------------------------------------------------------------------------------------------
void FileMode::startModification(bool replay)
{
    modTimer.singleShot(1, this, &FileMode::onTimerTimeout);
    if(replay)
        modTimer.start(static_cast<int>(timeMode) * 1000);
}
//---------------------------------------------------------------------------------------------------
void FileMode::onTimerTimeout()
{
    //Чтобы не стопить отображение qml, модификацию пихнем в отдельный поток
    std::thread t1(&FileMode::searchAndMod, this);
    t1.detach();
}
//---------------------------------------------------------------------------------------------------
void FileMode::searchAndMod()
{
    mtx.lock();
    hitList.clear();
    status = true;
    updateParameters();

    QDirIterator it(searchPath, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QFileInfo file(it.next());
        if (file.isDir())
            continue;
        if (file.fileName().contains(mask, Qt::CaseInsensitive)){
           // qDebug() << file;
            hitList.append(file);
        }
    }

    int count = hitList.size();
    for (int i = 0; i < count; i++)
        modFile(i);

    status = false;
    updateParameters();
    mtx.unlock();
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
        remove(hitList[indexFile]. absoluteFilePath().toStdString().c_str());
    saveFile(dataFile, indexFile);
}
//---------------------------------------------------------------------------------------------------
void FileMode::saveFile(QByteArray& data, int indexFile)
{
    using namespace std;
    QString tmpName;

    if(nameModeFlag)
    {
        string name_f = hitList[indexFile].fileName().toStdString();
        string ext;
        size_t index = name_f.find_last_of(".");
        if(index == string::npos)
            ext = "";
        else{
            ext = name_f.substr(index);
            name_f = name_f.replace(index, ext.size(),"");
        }
        tmpName = modRepit(QString::fromStdString(name_f), ext.data());
    }

    else {
        tmpName = hitList[indexFile].fileName();
    }

    ofstream out((savePath + tmpName).toStdString(),
                 ios_base::out | ios_base::binary);
    if(!out.is_open()){
        cout << hitList[indexFile].fileName().toStdString()
             << "\t open in ofstream !" << endl;
        return;
    }
    int sizeData = data.size();
    for (int i = 0; i < sizeData; i++)
        out.put(data.at(i));

    out.close();

}
//---------------------------------------------------------------------------------------------------
QString FileMode::modRepit(QString fileName, const char* shell)
{
    auto find = [&](QString nameF)
    {
        QDirIterator it(savePath, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QFileInfo file(it.next());
            if (file.isDir())
                continue;
            if (file.fileName().contains(nameF, Qt::CaseInsensitive)){
                return true;
            }
        }
        return false;
    };

    if(!find(fileName+ shell))
        return fileName.append(shell);

    int counter = 0;
    while(1){
        counter ++;
        if(!find(fileName + "(" + QString::number(counter) + ")" + shell))
            break;
    }

    fileName += ("(" + QString::number(counter) + ")" + shell);

    return fileName;
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
        if( !ok || timeMode < 1)
            timeMode = 1;
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
//void search(const QString& path, const QString& name)
//{
//    auto find = [&](QString nameF)
//    {
//        QDirIterator it(path, QDirIterator::Subdirectories);
//        while (it.hasNext())
//        {
//            QFileInfo file(it.next());
//            if (file.isDir())
//                continue;
//            if (file.fileName().contains(name, Qt::CaseInsensitive)){
//                return true;
//            }
//        }
//        return false;
//    };


//    QDirIterator it(path, QDirIterator::Subdirectories);
//    while (it.hasNext())
//    {
//        QFileInfo file(it.next());
//        if (file.isDir())
//            continue;
//        if (file.fileName().contains(name, Qt::CaseInsensitive)){
//            //  hitList.append(file);
//        }
//    }

//}
