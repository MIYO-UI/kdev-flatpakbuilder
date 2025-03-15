/**
 * @file flatpakbuilderjob.h
 * @brief Klasa zadania budowania pakietu Flatpak
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#ifndef FLATPAKBUILDERJOB_H
#define FLATPAKBUILDERJOB_H

#include <outputview/outputexecutejob.h>
#include <QProcess>

class FlatpakBuilderPlugin;

namespace KDevelop {
    class IProject;
}

/**
 * @class FlatpakBuilderJob
 * @brief Klasa reprezentująca zadanie budowania pakietu Flatpak
 *
 * Ta klasa odpowiada za uruchamianie procesu flatpak-builder z odpowiednimi
 * parametrami, przetwarzanie wyjścia i zarządzanie stanem zadania.
 */
class FlatpakBuilderJob : public KDevelop::OutputExecuteJob
{
    Q_OBJECT

public:
    /**
     * Typ operacji wykonywanych przez zadanie
     */
    enum OperationType {
        BuildOperation,     ///< Budowanie pakietu
        InstallOperation,   ///< Instalowanie pakietu
        ExportOperation     ///< Eksportowanie pakietu jako bundle
    };

    /**
     * Konstruktor
     * 
     * @param parent Obiekt rodzica (plugin)
     * @param project Projekt dla którego wykonywane jest zadanie
     * @param type Typ operacji
     */
    FlatpakBuilderJob(FlatpakBuilderPlugin* parent, KDevelop::IProject* project, OperationType type);
    
    /**
     * Destruktor
     */
    ~FlatpakBuilderJob() override;

    /**
     * @brief Ustawia ścieżkę do pliku manifestu Flatpak
     * @param path Ścieżka do manifestu
     */
    void setManifestPath(const QString& path);
    
    /**
     * @brief Ustawia katalog wyjściowy
     * @param path Ścieżka do katalogu wyjściowego
     */
    void setBuildDir(const QString& path);
    
    /**
     * @brief Ustawia dodatkowe opcje dla flatpak-builder
     * @param options Lista dodatkowych opcji
     */
    void setAdditionalOptions(const QStringList& options);

protected:
    /**
     * @brief Przygotowuje zadanie przed uruchomieniem
     * @return Kod błędu (0 jeśli wszystko OK)
     */
    int prepare() override;
    
    /**
     * @brief Tworzy proces i konfiguruje go przed uruchomieniem
     * @return Skonfigurowany proces
     */
    QProcess* createProcess() override;
    
    /**
     * @brief Obsługuje zakończenie procesu
     * @param exitCode Kod wyjścia procesu
     */
    void childProcessExited(int exitCode) override;

private:
    OperationType m_operationType;
    FlatpakBuilderPlugin* m_plugin;
    KDevelop::IProject* m_project;
    QString m_manifestPath;
    QString m_buildDir;
    QStringList m_additionalOptions;
    
    /**
     * @brief Przygotowuje argumenty dla procesu flatpak-builder
     * @return Lista argumentów
     */
    QStringList prepareArguments() const;
    
    /**
     * @brief Przygotowuje argumenty dla procesu flatpak (install/export)
     * @return Lista argumentów
     */
    QStringList prepareFlatpakArguments() const;
};

#endif // FLATPAKBUILDERJOB_H