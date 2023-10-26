
#pragma once
#ifndef _QDATATOOLS_H_
#define _QDATATOOLS_H_

#include <string>
#include <vector>

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

namespace rfbase
{
class DataUtils
{
public:
    static std::string generateUuid();

    [[nodiscard]] static bool makeDir(const QString &folder_path);

    [[nodiscard]] static int rmDir(const QString &path);

    [[nodiscard]] static bool makeFile(const std::string &path);

    [[nodiscard]] static bool makeFile(const QString &path);

    [[nodiscard]] static bool loadJson(const std::string &path, QJsonObject &json);

    [[nodiscard]] static bool saveJson(const QJsonObject &obj, const std::string &path);

    [[nodiscard]] static std::string getFileExtension(const std::string &filepath);

    [[nodiscard]] static bool getAllFilesByDir(const std::string &_path, const std::string &suffix,
                                               std::vector<std::string> &files);

    [[nodiscard]] static QString loadQString(const std::string &path);

    [[nodiscard]] static QJsonObject strToJson(const QString &json_string);

    [[nodiscard]] static QString jsonToStr(const QJsonObject &json_object);
};
} // namespace rfbase

#endif