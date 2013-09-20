QT += core network
QT -= gui

TARGET = GitHubReleaseCheckerTest
CONFIG += console c++11
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += main.cpp

# GitHubReleaseChecker
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../GitHubReleaseChecker/release/ -lGitHubReleaseChecker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../GitHubReleaseChecker/debug/ -lGitHubReleaseChecker
else:unix: LIBS += -L$$OUT_PWD/../GitHubReleaseChecker/ -lGitHubReleaseChecker

INCLUDEPATH += $$PWD/../GitHubReleaseChecker
DEPENDPATH += $$PWD/../GitHubReleaseChecker

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GitHubReleaseChecker/release/libGitHubReleaseChecker.a
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../GitHubReleaseChecker/debug/libGitHubReleaseChecker.a
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../GitHubReleaseChecker/libGitHubReleaseChecker.a
