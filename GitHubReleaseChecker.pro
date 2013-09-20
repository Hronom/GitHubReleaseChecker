TEMPLATE = subdirs

SUBDIRS += \
    GitHubReleaseChecker \
    GitHubReleaseCheckerTest

GitHubReleaseCheckerTest.depends = GitHubReleaseChecker
