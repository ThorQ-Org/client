#include "pathutils.h"

#include <QStandardPaths>

QDir ThorQ::PathUtils::GetConfigDirectory(QString subDirectory)
{
    QDir appdataDirectory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    appdataDirectory.mkdir(subDirectory);
    if (!appdataDirectory.cd(subDirectory)) {
        return QDir();
    }

    return appdataDirectory;
}
