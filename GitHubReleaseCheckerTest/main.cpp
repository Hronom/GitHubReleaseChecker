#include <QCoreApplication>

#include "GitHubReleaseChecker.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    GitHubReleaseChecker *gitHubReleaseChecker;
    gitHubReleaseChecker = new GitHubReleaseChecker("Hronom", "GitHubReleaseChecker", 0, 0, 1, "GitHubReleaseCheckerTest.zip");

    gitHubReleaseChecker->checkForNewVersion();

    while(!gitHubReleaseChecker->isCheckComplete())
    {
        QCoreApplication::processEvents();
        qDebug()<<"Check in progress...";
    }

    if(gitHubReleaseChecker->isNewVersionAvailable())
    {
        qDebug()<<"New version available:"<<gitHubReleaseChecker->getNewMajorVer()<<gitHubReleaseChecker->getNewMinorVer()<<gitHubReleaseChecker->getNewRevisVer();
        qDebug()<<"New version available at link:"<<gitHubReleaseChecker->getDownloadLink();
    }
    else
    {
        qDebug()<<"New version not available";
    }

    return a.exec();
}
