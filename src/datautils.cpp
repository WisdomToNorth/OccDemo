
#include "datautils.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>

#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QStandardPaths>
#include <QString>
#include <QUuid>

#include <filesystem>
#include <iostream>
#include <string>

namespace rfbase
{
std::string DataUtils::generateUuid()
{
    // boost::uuids::random_generator gen;
    // boost::uuids::uuid uuid = gen();
    // return boost::uuids::to_string(uuid);
    return QUuid::createUuid().toString().toStdString();
}

bool DataUtils::makeDir(const QString &folder_path)
{
    std::filesystem::path dir(folder_path.toStdString());
    return std::filesystem::create_directories(dir);
}

int DataUtils::rmDir(const QString &path)
{
    return std::filesystem::remove_all(path.toStdString());
}

bool DataUtils::makeFile(const std::string &path)
{
    std::filesystem::path filePath(path);
    if (std::filesystem::exists(filePath))
    {
        return true;
    }

    std::filesystem::path parentDir = filePath.parent_path();
    if (!std::filesystem::exists(parentDir))
    {
        bool created = std::filesystem::create_directories(parentDir);
        if (!created)
        {
            return false;
        }
    }

    std::ofstream fileStream(filePath);
    if (fileStream.is_open())
    {
        fileStream.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool DataUtils::makeFile(const QString &path)
{
    return makeFile(path.toStdString());
}

bool DataUtils::loadJson(const std::string &path, QJsonObject &json)
{
    QFile file(QString::fromStdString(path));
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray l_data = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument json_doc(QJsonDocument::fromJson(l_data, &json_error));

    if (json_error.error == QJsonParseError::NoError)
    {
        json = json_doc.object();
        return true;
    }
    else
    {
        std::cout << "JSON ERROR: " << json_error.errorString().toStdString();
        return false;
    }
}

QString DataUtils::loadQString(const std::string &path)
{
    QFile file(QString::fromStdString(path));
    if (!file.exists() || !file.open(QIODevice::ReadOnly))
    {
        return QString();
    }

    QByteArray l_data = file.readAll();
    file.close();
    return QString(l_data);
}

bool DataUtils::getAllFilesByDir(const std::string &_path, const std::string &suffix,
                                 std::vector<std::string> &files)
{
    std::filesystem::path l_dir_path(_path);
    // is exists
    if (!std::filesystem::exists(l_dir_path))
    {
        std::cout << _path << ", not exists..." << std::endl;
        return false;
    }
    if (!std::filesystem::is_directory(l_dir_path))
    {
        std::cout << _path << ", not dir...." << std::endl;
        return false;
    }

    std::cout << "Files found in " << _path << ":\n";

    std::filesystem::directory_entry l_dir(l_dir_path);
    for (std::filesystem::directory_iterator end, begin(l_dir); begin != end; ++begin)
    {
        if (!std::filesystem::is_directory(begin->path()))
        {
            if (begin->path().extension().string() == suffix)
            {
                files.push_back(begin->path().string());
                std::cout << begin->path().string() << std::endl;
            }
        }
    }
    return true;
};

bool DataUtils::saveJson(const QJsonObject &obj, const std::string &path)
{
    QFile file(QString::fromStdString(path));

    if (!file.exists() && (!makeFile(QString::fromStdString(path))))
    {
        std::cout << "File create error";
        return false;
    }

    if (!file.open(QIODevice::WriteOnly | QFile::Truncate))
    {
        std::cout << "File open error";
        return false;
    }

    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}

QJsonObject DataUtils::strToJson(const QString &json_string)
{
    return QJsonDocument::fromJson(json_string.toStdString().data()).object();
}

QString DataUtils::jsonToStr(const QJsonObject &json_object)
{
    return QString(QJsonDocument(json_object).toJson());
}

std::string DataUtils::getFileExtension(const std::string &filepath)
{
    size_t pos = filepath.find_last_of('.');
    if (pos != std::string::npos)
    {
        return filepath.substr(pos + 1);
    }
    return "";
}
} // namespace rfbase
