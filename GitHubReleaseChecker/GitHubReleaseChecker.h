#ifndef GITHUBRELEASECHECKER_H
#define GITHUBRELEASECHECKERR_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

class GitHubReleaseChecker: public QObject
{
    Q_OBJECT
private:
    QString m_author;
    QString m_repoName;

    qint32 m_currentMajorVer;
    qint32 m_currentMinorVer;
    qint32 m_currentRevisVer;

    QString m_updateFileName;

    qint32 m_newMajorVer;
    qint32 m_newMinorVer;
    qint32 m_newRevisVer;
    QString m_downloadLink;

    QNetworkAccessManager *m_networkAccessManager;
    bool m_checkLaunched;
    bool m_checked;

public:
    GitHubReleaseChecker(const QString &par_author, const QString &par_repoName, const qint32 &par_majorVer, const qint32 &par_minorVer, const qint32 &par_revisVer, const QString &par_updateFileName, QObject *par_parent = 0);
    ~GitHubReleaseChecker();

    void checkForNewVersion();
    bool isCheckComplete();
    bool isNewVersionAvailable();

    qint32 getNewMajorVer();
    qint32 getNewMinorVer();
    qint32 getNewRevisVer();
    QString getDownloadLink();


private slots:
    void requestFinished(QNetworkReply *par_networkReply);

private:
    void resetNewVersionCheck();
    void readNetworkReply(const QByteArray &par_data);

signals:
    void checkComplete();
    void newVersionAvailable(const qint32 &par_majorVer, const qint32 &par_minorVer, const qint32 &par_revisVer, const QString &par_downloadLink);
    void newVersionNotAvailable();

    void errorHappens(QString par_error);
};

#endif
