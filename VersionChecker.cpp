#include "VersionChecker.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>

VersionChecker::VersionChecker(int xMajorVer, int xMinorVer, int xRevisVer, const QString &xVersionFileLink, QObject *xParent): QObject(xParent)
{
    mMajorVer = xMajorVer;
    mMinorVer = xMinorVer;
    mRevisVer = xRevisVer;

    mVersionFileLink = xVersionFileLink;

    mNetworkAccessManager = new QNetworkAccessManager(this);
    connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));

    mCheckLaunched = false;
}

void VersionChecker::checkVersion()
{
    if(mCheckLaunched == false)
    {
        mCheckLaunched = true;
        QUrl xCheckUrl(mVersionFileLink);
        QNetworkRequest xNetworkRequest(xCheckUrl);
        xNetworkRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
        xNetworkRequest.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
        mNetworkAccessManager->get(xNetworkRequest);
    }
}

void VersionChecker::requestFinished(QNetworkReply *xNetworkReply)
{
    if(xNetworkReply->error() == QNetworkReply::NoError)
    {
        if(xNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl() != QUrl(""))
        {
            QNetworkRequest xNetworkRequest(xNetworkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl());
            xNetworkRequest.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
            xNetworkRequest.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);
            mNetworkAccessManager->get(xNetworkRequest);
        }
        else
        {
            QString xFullString;
            xFullString = xNetworkReply->readAll();

            QString xVersion = xFullString.left(xFullString.indexOf('='));
            QString xDownloadLink = xFullString.mid(xFullString.indexOf('=')+1);

            QStringList xValuesList;
            xValuesList = xVersion.split('.');

            int xMajorVer, xMinorVer, xRevisVer;
            xMajorVer = xValuesList.at(0).toInt();
            xMinorVer = xValuesList.at(1).toInt();
            xRevisVer = xValuesList.at(2).toInt();

            if((mMajorVer < xMajorVer) ||
               (mMajorVer == xMajorVer && mMinorVer < xMinorVer) ||
               (mMajorVer == xMajorVer && mMinorVer == xMinorVer && mRevisVer < xRevisVer))
                emit this->newVersionAvailable(xMajorVer, xMinorVer, xRevisVer, xDownloadLink);
            else
                emit this->newVersionNotAvailable();

            mCheckLaunched = false;
        }
    }
    else
    {
        mCheckLaunched = false;
        emit this->errorHappened(xNetworkReply->errorString());
    }

    xNetworkReply->deleteLater();
}
