/**
 * @file flatpakbuilderconfig.cpp
 * @brief Implementacja klasy konfiguracji wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#include "flatpakbuilderconfig.h"

#include <KSharedConfig>
#include <QStandardPaths>
#include <QDir>

FlatpakBuilderConfig::FlatpakBuilderConfig(QObject* parent)
    : QObject(parent)
    , m_flatpakBuilderPath(QStandardPaths::findExecutable("flatpak-builder"))
    , m_flatpakPath(QStandardPaths::findExecutable("flatpak"))
    , m_defaultBuildDir(QDir::homePath() + "/.cache/flatpak-builder")
    , m_config(KSharedConfig::openConfig()->group("FlatpakBuilder"))
{
    load();
}

FlatpakBuilderConfig::~FlatpakBuilderConfig()
{
    save();
}

QString FlatpakBuilderConfig::flatpakBuilderPath() const
{
    return m_flatpakBuilderPath;
}

void FlatpakBuilderConfig::setFlatpakBuilderPath(const QString& path)
{
    m_flatpakBuilderPath = path;
}

QString FlatpakBuilderConfig::flatpakPath() const
{
    return m_flatpakPath;
}

void FlatpakBuilderConfig::setFlatpakPath(const QString& path)
{
    m_flatpakPath = path;
}

QString FlatpakBuilderConfig::defaultBuildDir() const
{
    return m_defaultBuildDir;
}

void FlatpakBuilderConfig::setDefaultBuildDir(const QString& dir)
{
    m_defaultBuildDir = dir;
}

void FlatpakBuilderConfig::load()
{
    m_flatpakBuilderPath = m_config.readEntry("FlatpakBuilderPath", m_flatpakBuilderPath);
    m_flatpakPath = m_config.readEntry("FlatpakPath", m_flatpakPath);
    m_defaultBuildDir = m_config.readEntry("DefaultBuildDir", m_defaultBuildDir);
}

void FlatpakBuilderConfig::save()
{
    m_config.writeEntry("FlatpakBuilderPath", m_flatpakBuilderPath);
    m_config.writeEntry("FlatpakPath", m_flatpakPath);
    m_config.writeEntry("DefaultBuildDir", m_defaultBuildDir);
    m_config.sync();
}

QStringList FlatpakBuilderConfig::defaultRuntimes() const
{
    return {
        "org.freedesktop.Platform",
        "org.kde.Platform",
        "org.gnome.Platform"
    };
}