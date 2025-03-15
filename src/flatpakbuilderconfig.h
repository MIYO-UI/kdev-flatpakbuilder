/**
 * @file flatpakbuilderconfig.h
 * @brief Klasa konfiguracji wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#ifndef FLATPAKBUILDERCONFIG_H
#define FLATPAKBUILDERCONFIG_H

#include <QObject>
#include <QString>
#include <KConfigGroup>

/**
 * @class FlatpakBuilderConfig
 * @brief Klasa przechowująca i zarządzająca konfiguracją wtyczki
 *
 * Ta klasa odpowiada za odczyt i zapis konfiguracji wtyczki Flatpak Builder,
 * przechowuje ścieżki do narzędzi, domyślne opcje budowania itp.
 */
class FlatpakBuilderConfig : public QObject
{
    Q_OBJECT

public:
    /**
     * Konstruktor
     * 
     * @param parent Obiekt rodzica
     */
    explicit FlatpakBuilderConfig(QObject* parent = nullptr);
    
    /**
     * Destruktor
     */
    ~FlatpakBuilderConfig() override;

    /**
     * @brief Zwraca ścieżkę do narzędzia flatpak-builder
     * @return Ścieżka do flatpak-builder
     */
    QString flatpakBuilderPath() const;
    
    /**
     * @brief Ustawia ścieżkę do narzędzia flatpak-builder
     * @param path Nowa ścieżka
     */
    void setFlatpakBuilderPath(const QString& path);
    
    /**
     * @brief Zwraca ścieżkę do narzędzia flatpak
     * @return Ścieżka do flatpak
     */
    QString flatpakPath() const;
    
    /**
     * @brief Ustawia ścieżkę do narzędzia flatpak
     * @param path Nowa ścieżka
     */
    void setFlatpakPath(const QString& path);
    
    /**
     * @brief Zwraca domyślną lokalizację katalogu wyjściowego
     * @return Ścieżka do katalogu wyjściowego
     */
    QString defaultBuildDir() const;
    
    /**
     * @brief Ustawia domyślną lokalizację katalogu wyjściowego
     * @param dir Ścieżka do katalogu wyjściowego
     */
    void setDefaultBuildDir(const QString& dir);
    
    /**
     * @brief Odczytuje konfigurację z pliku
     */
    void load();
    
    /**
     * @brief Zapisuje konfigurację do pliku
     */
    void save();
    
    /**
     * @brief Zwraca listę domyślnych runtime'ów Flatpak
     * @return Lista domyślnych runtime'ów
     */
    QStringList defaultRuntimes() const;

private:
    QString m_flatpakBuilderPath;
    QString m_flatpakPath;
    QString m_defaultBuildDir;
    KConfigGroup m_config;
};

#endif // FLATPAKBUILDERCONFIG_H