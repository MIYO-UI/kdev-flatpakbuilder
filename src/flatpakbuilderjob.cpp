/**
 * @file flatpakbuilderjob.cpp
 * @brief Implementacja klasy zadania budowania pakietu Flatpak
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#include "flatpakbuilderjob.h"
#include "flatpakbuilderplugin.h"
#include "flatpakbuilderconfig.h"
#include "flatpakmanifestmanager.h"
#include "flatpakbuildoutputparser.h"

#include <interfaces/icore.h>
#include <interfaces/iproject.h>
#include <interfaces/iruncontroller.h>
#include <outputview/outputmodel.h>

#include <KLocalizedString>
#include <KMessageBox>

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

FlatpakBuilderJob::FlatpakBuilderJob(FlatpakBuilderPlugin* parent, KDevelop::IProject* project, OperationType type)
    : KDevelop::OutputExecuteJob(parent)
    , m_operationType(type)
    , m_plugin(parent)
    , m_project(project)
    , m_buildDir("")
{
    // Ustaw tytuł zadania w zależności od operacji
    switch (m_operationType) {
        case BuildOperation:
            setJobName(i18n("Flatpak Build: %1", project->name()));
            break;
        case InstallOperation:
            setJobName(i18n("Flatpak Install: %1", project->name()));
            break;
        case ExportOperation:
            setJobName(i18n("Flatpak Export: %1", project->name()));
            break;
    }
    
    // Ustaw domyślny katalog roboczy
    setWorkingDirectory(project->path().toLocalFile());
    
    // Ustaw domyślny katalog wyjściowy (jeśli nie zostanie nadpisany)
    m_buildDir = QDir::tempPath() + "/flatpak-build-" + project->name();
    
    // Ustaw parser wyjścia
    setToolViewFactory(new FlatpakBuildOutputParser(this));
    
    // Umożliw zatrzymanie zadania przez użytkownika
    setProperties(KDevelop::OutputExecuteJob::JobProperty::Killable);
    
    // Rejestruj zadanie w kontrolerze uruchamiania
    KDevelop::ICore::self()->runController()->registerJob(this);
}

FlatpakBuilderJob::~FlatpakBuilderJob()
{
}

void FlatpakBuilderJob::setManifestPath(const QString& path)
{
    m_manifestPath = path;
}

void FlatpakBuilderJob::setBuildDir(const QString& path)
{
    m_buildDir = path;
}

void FlatpakBuilderJob::setAdditionalOptions(const QStringList& options)
{
    m_additionalOptions = options;
}

int FlatpakBuilderJob::prepare()
{
    // Sprawdź, czy ścieżka do manifestu jest poprawna
    if (m_manifestPath.isEmpty() || !QFile::exists(m_manifestPath)) {
        setError(1);
        setErrorText(i18n("Invalid manifest path: %1", m_manifestPath));
        return 1;
    }
    
    // Upewnij się, że katalog wyjściowy istnieje
    QDir buildDir(m_buildDir);
    if (!buildDir.exists()) {
        if (!buildDir.mkpath(".")) {
            setError(2);
            setErrorText(i18n("Could not create build directory: %1", m_buildDir));
            return 2;
        }
    }
    
    // Upewnij się, że flatpak-builder jest zainstalowane
    FlatpakBuilderConfig* config = m_plugin->config();
    QString flatpakBuilderPath = config->flatpakBuilderPath();
    QString flatpakPath = config->flatpakPath();
    
    if (m_operationType == BuildOperation && flatpakBuilderPath.isEmpty()) {
        setError(3);
        setErrorText(i18n("flatpak-builder not found. Please install flatpak-builder package."));
        return 3;
    }
    
    if ((m_operationType == InstallOperation || m_operationType == ExportOperation) && 
        flatpakPath.isEmpty()) {
        setError(4);
        setErrorText(i18n("flatpak not found. Please install flatpak package."));
        return 4;
    }
    
    return 0;
}

QProcess* FlatpakBuilderJob::createProcess()
{
    // Utwórz nowy proces
    QProcess* process = new QProcess();
    
    // Ustaw katalog roboczy
    process->setWorkingDirectory(workingDirectory());
    
    // Wybierz program i argumenty w zależności od operacji
    FlatpakBuilderConfig* config = m_plugin->config();
    
    switch (m_operationType) {
        case BuildOperation:
            process->setProgram(config->flatpakBuilderPath());
            process->setArguments(prepareArguments());
            break;
            
        case InstallOperation:
        case ExportOperation:
            process->setProgram(config->flatpakPath());
            process->setArguments(prepareFlatpakArguments());
            break;
    }
    
    return process;
}

void FlatpakBuilderJob::childProcessExited(int exitCode)
{
    // Obsługa zakończenia procesu
    if (exitCode != 0) {
        KDevelop::OutputModel* model = qobject_cast<KDevelop::OutputModel*>(outputModel());
        if (model) {
            model->appendLine(i18n("Process exited with code %1", exitCode));
        }
    } else {
        switch (m_operationType) {
            case BuildOperation:
                outputModel()->appendLine(i18n("Flatpak successfully built."));
                break;
                
            case InstallOperation:
                outputModel()->appendLine(i18n("Flatpak successfully installed."));
                break;
                
            case ExportOperation:
                outputModel()->appendLine(i18n("Flatpak successfully exported to bundle."));
                break;
        }
    }
    
    // Wywołanie metody bazowej
    KDevelop::OutputExecuteJob::childProcessExited(exitCode);
}

QStringList FlatpakBuilderJob::prepareArguments() const
{
    QStringList args;
    
    // Dodaj domyślne opcje
    args << "--force-clean";
    
    // Dodaj ścieżkę do katalogu wyjściowego
    args << m_buildDir;
    
    // Dodaj ścieżkę do manifestu
    args << m_manifestPath;
    
    // Dodaj dodatkowe opcje
    args << m_additionalOptions;
    
    return args;
}

QStringList FlatpakBuilderJob::prepareFlatpakArguments() const
{
    QStringList args;
    
    switch (m_operationType) {
        case InstallOperation:
            args << "install";
            args << "--user";  // Instaluj dla bieżącego użytkownika
            args << "--bundle";
            
            // Znajdź plik bundle w katalogu budowania
            QString appId = m_plugin->config()->getAppId(QUrl::fromLocalFile(m_manifestPath));
            args << QString("%1/%2.flatpak").arg(m_buildDir, appId);
            break;
            
        case ExportOperation:
            args << "build-export";
            args << m_buildDir + "-repo";  // Repozytorium
            args << m_buildDir;           // Katalog budowania
            break;
            
        default:
            // Domyślnie puste argumenty
            break;
    }
    
    return args;
}