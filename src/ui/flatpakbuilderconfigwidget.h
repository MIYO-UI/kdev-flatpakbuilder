/**
 * @file flatpakbuilderconfigwidget.h
 * @brief Widget konfiguracyjny dla wtyczki Flatpak Builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#ifndef FLATPAKBUILDERCONFIGWIDGET_H
#define FLATPAKBUILDERCONFIGWIDGET_H

#include <QWidget>

class FlatpakBuilderPlugin;
class FlatpakBuilderConfig;

namespace Ui {
    class FlatpakBuilderConfigWidget;
}

/**
 * @class FlatpakBuilderConfigWidget
 * @brief Widget konfiguracyjny dla wtyczki Flatpak Builder
 *
 * Ten widget umożliwia użytkownikowi konfigurację wtyczki Flatpak Builder,
 * w tym ustawienie ścieżek do narzędzi, katalogu wyjściowego itp.
 */
class FlatpakBuilderConfigWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Konstruktor
     * 
     * @param plugin Wtyczka dla której tworzony jest widget
     * @param parent Obiekt rodzica
     */
    explicit FlatpakBuilderConfigWidget(FlatpakBuilderPlugin* plugin, QWidget* parent = nullptr);
    
    /**
     * Destruktor
     */
    ~FlatpakBuilderConfigWidget() override;
    
    /**
     * @brief Zapisuje zmiany konfiguracji
     */
    void save();
    
    /**
     * @brief Ładuje konfigurację
     */
    void load();
    
    /**
     * @brief Przywraca domyślne ustawienia
     */
    void defaults();

private Q_SLOTS:
    /**
     * @brief Slot wywoływany po kliknięciu przycisku wyboru ścieżki do flatpak-builder
     */
    void slotBrowseFlatpakBuilder();
    
    /**
     * @brief Slot wywoływany po kliknięciu przycisku wyboru ścieżki do flatpak
     */
    void slotBrowseFlatpak();
    
    /**
     * @brief Slot wywoływany po kliknięciu przycisku wyboru katalogu wyjściowego
     */
    void slotBrowseBuildDir();

private:
    Ui::FlatpakBuilderConfigWidget* ui;
    FlatpakBuilderPlugin* m_plugin;
    FlatpakBuilderConfig* m_config;
};

#endif // FLATPAKBUILDERCONFIGWIDGET_H