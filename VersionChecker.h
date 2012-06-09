#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class VersionChecker: public QObject
{
    Q_OBJECT
private:
    int mMajorVer;
    int mMinorVer;
    int mRevisVer;

    QString mVersionFileLink;

    QNetworkAccessManager *mNetworkAccessManager;
    bool mCheckLaunched;

public:
    VersionChecker(int xMajorVer, int xMinorVer, int xRevisVer, const QString &xVersionFileLink, QObject *xParent = 0);
    void checkVersion();

private slots:
    void requestFinished(QNetworkReply *xNetworkReply);

signals:
    void newVersionAvailable(int xMajorVer, int xMinorVer, int xRevisVer, const QString &xVersionFileLink);
    void newVersionNotAvailable();
    void errorHappened(QString xError);
};

#endif // VERSIONCHECKER_H
