/**
 * @file flatpakbuildoutputparser.h
 * @brief Parser wyjścia narzędzia flatpak-builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#ifndef FLATPAKBUILDOUTPUTPARSER_H
#define FLATPAKBUILDOUTPUTPARSER_H

#include <outputview/outputexecutejob.h>
#include <QRegularExpression>

/**
 * @class FlatpakBuildOutputParser
 * @brief Klasa analizująca i formatująca wyjście z narzędzia flatpak-builder
 *
 * Ta klasa odpowiada za parsowanie wyjścia z procesu flatpak-builder,
 * wyciąganie informacji o błędach i ostrzeżeniach oraz formatowanie
 * tego wyjścia w sposób przyjazny dla użytkownika.
 */
class FlatpakBuildOutputParser : public KDevelop::OutputExecuteJobExecutor::StandardToolView
{
    Q_OBJECT

public:
    /**
     * Konstruktor
     * 
     * @param parent Obiekt rodzica
     */
    explicit FlatpakBuildOutputParser(QObject* parent = nullptr);
    
    /**
     * Destruktor
     */
    ~FlatpakBuildOutputParser() override;

protected:
    /**
     * @brief Przetwarzanie linii wyjścia
     * @param line Linia do przetworzenia
     * @return Przetworzona linia
     */
    QString processLine(const QString& line) override;
    
    /**
     * @brief Wyciąga informacje o błędzie z linii wyjścia
     * @param line Linia do analizy
     * @return true jeśli linia zawiera błąd
     */
    bool parseError(const QString& line);
    
    /**
     * @brief Wyciąga informacje o ostrzeżeniu z linii wyjścia
     * @param line Linia do analizy
     * @return true jeśli linia zawiera ostrzeżenie
     */
    bool parseWarning(const QString& line);
    
    /**
     * @brief Wyciąga informacje o postępie z linii wyjścia
     * @param line Linia do analizy
     * @return true jeśli linia zawiera informacje o postępie
     */
    bool parseProgress(const QString& line);

private:
    QRegularExpression m_errorRegex;
    QRegularExpression m_warningRegex;
    QRegularExpression m_progressRegex;
};

#endif // FLATPAKBUILDOUTPUTPARSER_H