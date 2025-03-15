/**
 * @file flatpakbuilderplugin.h
 * @brief Główna klasa wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#ifndef FLATPAKBUILDERPLUGIN_H
#define FLATPAKBUILDERPLUGIN_H

#include <interfaces/iplugin.h>
#include <project/interfaces/iprojectbuilder.h>
#include <QVariantList>

class FlatpakBuilderConfig;
class FlatpakManifestManager;

/**
 * @class FlatpakBuilderPlugin
 * @brief Główna klasa wtyczki do budowania pakietów Flatpak w środowisku KDevelop
 *
 * Ta wtyczka integruje narzędzie flatpak-builder z KDevelop, umożliwiając
 * budowanie aplikacji jako pakietów Flatpak bezpośrednio z IDE.
 */
class FlatpakBuilderPlugin : public KDevelop::IPlugin, public KDevelop::IProjectBuilder
{
    Q_OBJECT
    Q_INTERFACES(KDevelop::IProjectBuilder)

public:
    /**
     * Konstruktor
     * 
     * @param parent Obiekt rodzica
     * @param args Lista argumentów dla wtyczki
     */
    FlatpakBuilderPlugin(QObject* parent, const QVariantList& args);
    
    /**
     * Destruktor
     */
    ~FlatpakBuilderPlugin() override;

    /**
     * @brief Zwraca nazwę wtyczki
     * @return Nazwa wtyczki
     */
    QString name() const override;

    /**
     * @brief Sprawdza, czy projekt ma skonfigurowany manifest Flatpak
     * @param project Projekt do sprawdzenia
     * @return true jeśli projekt ma manifest Flatpak
     */
    bool hasManifest(KDevelop::IProject* project) const;

    /**
     * @brief Tworzy nowy manifest Flatpak dla projektu
     * @param project Projekt dla którego tworzony jest manifest
     */
    void createManifest(KDevelop::IProject* project);

    /**
     * @brief Otwiera manifest do edycji
     * @param project Projekt którego manifest ma być edytowany
     */
    void editManifest(KDevelop::IProject* project);

    /**
     * @brief Buduje projekt jako pakiet Flatpak
     * @param project Projekt do zbudowania
     * @param items Lista elementów projektu do zbudowania (ignorowana, budowany jest cały projekt)
     * @return KJob* Zadanie budowania
     */
    KJob* build(KDevelop::ProjectBaseItem* item) override;

    /**
     * @brief Instaluje zbudowany pakiet Flatpak
     * @param project Projekt do zainstalowania
     */
    KJob* install(KDevelop::ProjectBaseItem* item) override;

    /**
     * @brief Eksportuje pakiet jako plik bundle (.flatpak)
     * @param project Projekt do wyeksportowania
     */
    KJob* exportBundle(KDevelop::IProject* project);

    /**
     * @brief Zwraca konfigurację dla wtyczki
     * @return Obiekt konfiguracji
     */
    FlatpakBuilderConfig* config() const;

public Q_SLOTS:
    /**
     * @brief Slot wywoływany po kliknięciu akcji "Build Flatpak"
     */
    void slotBuildFlatpak();

    /**
     * @brief Slot wywoływany po kliknięciu akcji "Install Flatpak"
     */
    void slotInstallFlatpak();

    /**
     * @brief Slot wywoływany po kliknięciu akcji "Export Bundle"
     */
    void slotExportBundle();

    /**
     * @brief Slot wywoływany po kliknięciu akcji "Create Manifest"
     */
    void slotCreateManifest();

    /**
     * @brief Slot wywoływany po kliknięciu akcji "Edit Manifest"
     */
    void slotEditManifest();

private:
    FlatpakBuilderConfig* m_config;
    FlatpakManifestManager* m_manifestManager;
    QAction* m_buildAction;
    QAction* m_installAction;
    QAction* m_exportBundleAction;
    QAction* m_createManifestAction;
    QAction* m_editManifestAction;

    /**
     * @brief Inicjuje akcje wtyczki
     */
    void setupActions();
};

#endif // FLATPAKBUILDERPLUGIN_H