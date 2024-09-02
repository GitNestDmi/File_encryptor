#ifndef FILEMODE_H
#define FILEMODE_H

#include <QFileInfoList>
#include <QVariant>
#include <QTimer>
#include<mutex>

class NameFM: public QObject
{
    Q_OBJECT
public:
    NameFM(){}
    NameFM(QObject *parent);
    virtual ~NameFM();

    enum TypeCheckBox{
        DEL_ORIGIN_FLAG,
        MOD_NAME_FLAG,
        TIMER_FLAG
    };
    enum TypeParameters{
        SEARCH_PATH,
        SAVE_PATH,
        MASK,
        KEY,
        TIME_MODE,
        STATUS_MOD
    };
    Q_ENUM(TypeCheckBox)
    Q_ENUM(TypeParameters)

};




class IFileMode: public NameFM
{
    Q_OBJECT
    Q_PROPERTY(QVariant parameters  MEMBER parameters  NOTIFY parametersChanged)        //данные для вывода в qml

public:
    explicit IFileMode(QObject *parent);
     ~IFileMode();

    Q_INVOKABLE virtual void setCheckState(NameFM::TypeCheckBox index, bool value) = 0;
    Q_INVOKABLE virtual void setParameters(NameFM::TypeParameters param, QString value) = 0;
    Q_INVOKABLE virtual void startModification(bool replay) = 0;
    Q_INVOKABLE virtual void stopModReplay() = 0;
    QVariant parameters {};

signals:
    void parametersChanged();
};





class FileMode: public IFileMode
{
    Q_OBJECT

public:
    explicit FileMode(QObject *parent = nullptr);
    ~FileMode() override;

    void startModification(bool replay) override;
    void stopModReplay() override;
    void setCheckState(NameFM::TypeCheckBox index, bool value) override;
    void setParameters(NameFM::TypeParameters param, QString value) override;

private:
    QString modRepit(QString fileName,const QString shell);
    void updateParameters();
    void modFile(int indexFile);
    void searchAndMod();
    void saveFile(QByteArray& code,int indexFile);
    void strToQBArr(QString code);

    bool status = false;
    bool delOriginFlag = false;
    bool nameModeFlag = false;
    bool timerFlag = false;
    uint timeMode {30};
    QString searchPath {};
    QString savePath {};
    QString mask {};
    QByteArray key {};
    QFileInfoList hitList;
    QTimer modTimer;
    std::mutex mtx;

private slots:
    void onTimerTimeout();

};

#endif // FILEMODE_H
