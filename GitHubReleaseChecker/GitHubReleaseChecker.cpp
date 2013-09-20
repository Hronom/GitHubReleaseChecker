#include "GitHubReleaseChecker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

GitHubReleaseChecker::GitHubReleaseChecker(const QString &par_author, const QString &par_repoName, const qint32 &par_majorVer, const qint32 &par_minorVer, const qint32 &par_revisVer, const QString &par_updateFileName, QObject *par_parent): QObject(par_parent)
{
    m_author = par_author;
    m_repoName = par_repoName;

    m_currentMajorVer = par_majorVer;
    m_currentMinorVer = par_minorVer;
    m_currentRevisVer = par_revisVer;

    m_updateFileName = par_updateFileName;

    m_newMajorVer = m_currentMajorVer;
    m_newMinorVer = m_currentMinorVer;
    m_newRevisVer = m_currentRevisVer;
    m_downloadLink = "";

    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    m_checkLaunched = false;
    m_checked = false;
}

GitHubReleaseChecker::~GitHubReleaseChecker()
{

}

void GitHubReleaseChecker::checkForNewVersion()
{
    if(m_checkLaunched == false)
    {
        m_checkLaunched = true;
        QUrl checkUrl("https://api.github.com/repos/" + m_author + "/" + m_repoName + "/tags");
        QNetworkRequest networkRequest(checkUrl);
        networkRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        networkRequest.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
        m_networkAccessManager->get(networkRequest);
    }
}

bool GitHubReleaseChecker::isCheckComplete()
{
    return m_checked;
}

bool GitHubReleaseChecker::isNewVersionAvailable()
{
    if(m_currentMajorVer != m_newMajorVer ||
            m_currentMinorVer != m_newMinorVer ||
            m_currentRevisVer != m_newRevisVer)
        return true;
    else
        return false;
}

qint32 GitHubReleaseChecker::getNewMajorVer()
{
    return m_newMajorVer;
}

qint32 GitHubReleaseChecker::getNewMinorVer()
{
    return m_newMinorVer;
}

qint32 GitHubReleaseChecker::getNewRevisVer()
{
    return m_newRevisVer;
}

QString GitHubReleaseChecker::getDownloadLink()
{
    return m_downloadLink;
}

void GitHubReleaseChecker::requestFinished(QNetworkReply *par_networkReply)
{
    if(par_networkReply->error() == QNetworkReply::NoError)
    {
        if(par_networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl() != QUrl(""))
        {
            QNetworkRequest networkRequest(par_networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
            networkRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
            networkRequest.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
            m_networkAccessManager->get(networkRequest);
        }
        else
        {
            readNetworkReply(par_networkReply->readAll());

            m_checkLaunched = false;
            m_checked = true;

            emit this->checkComplete();
        }
    }
    else
    {
        m_checkLaunched = false;
        emit this->errorHappens(par_networkReply->errorString());
    }

    par_networkReply->deleteLater();
}

void GitHubReleaseChecker::readNetworkReply(const QByteArray &par_data)
{
    QJsonDocument doc;
    doc = QJsonDocument::fromJson(par_data);

    QJsonArray replyArray;
    replyArray = doc.array();

    QJsonArray::Iterator iter;
    iter = replyArray.begin();

    while(iter != replyArray.end())
    {
        QJsonObject object;
        object = (*iter).toObject();

        QString tagName;
        tagName = object.value("name").toString();

        QRegExp regExp;
        //exp.setPattern("v[0-9]{1,}.[0-9]{1,}.[0-9]{1,}"); // Old
        regExp.setPattern("^[v]{0,1}(\\d+\\.)?(\\d+\\.)?(\\*|\\d+)$"); // New, source: http://stackoverflow.com/a/82205

        if(regExp.exactMatch(tagName))
        {
            if(regExp.matchedLength() != -1 && regExp.matchedLength() == tagName.size())
            {
                qint32 newMajorVer;
                newMajorVer = regExp.cap(1).toInt();

                qint32 newMinorVer;
                newMinorVer = regExp.cap(2).toInt();

                qint32 newRevisVer;
                newRevisVer = regExp.cap(3).toInt();

                if( ( m_newMajorVer < newMajorVer ) ||
                    ( m_newMajorVer == newMajorVer && m_newMinorVer < newMinorVer ) ||
                    ( m_newMajorVer == newMajorVer && m_newMinorVer == newMinorVer && m_newRevisVer < newRevisVer ) )
                {
                    m_newMajorVer = newMajorVer;
                    m_newMinorVer = newMinorVer;
                    m_newRevisVer = newRevisVer;
                    m_downloadLink = "https://github.com/" + m_author + "/" + m_repoName + "/releases/download/" + tagName + "/" + m_updateFileName;   
                }
            }
        }

        ++iter;
    }
}
