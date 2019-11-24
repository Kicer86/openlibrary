
#ifndef CONFIGURATION_JSON_STORAGE_HPP
#define CONFIGURATION_JSON_STORAGE_HPP


#include <QString>
#include <OpenLibrary/utils_qt/configuration.hpp>
#include <OpenLibrary/utils_qt/utils_qt_export.h>


class UTILS_QT_EXPORT ConfigJsonStorage: public IConfigStorage
{
public:
    explicit ConfigJsonStorage(const QString& configFile);

    IConfigStorage::Content load() override;
    void save(const IConfigStorage::Content& configuration) override;

private:
    QString m_configFile;
};

#endif
