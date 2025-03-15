/**
 * @file flatpakbuildoutputparser.cpp
 * @brief Implementacja parsera wyjścia narzędzia flatpak-builder
 * @author Twoje Imię <twój@email.com>
 * @license GPL
 */

#include "flatpakbuildoutputparser.h"
#include <interfaces/icore.h>
#include <interfaces/iprojectcontroller.h>
#include <interfaces/iproject.h>
#include <project/projectmodel.h>
#include <util/path.h>

#include <QRegularExpressionMatch>

FlatpakBuildOutputParser::FlatpakBuildOutputParser(QObject* parent)
    : KDevelop::OutputExecuteJobExecutor::StandardToolView(parent)
{
    // Wyrażenia regularne do wykrywania błędów i ostrzeżeń
    m_errorRegex = QRegularExpression("(error|ERROR|Error):(.*)");
    m_warningRegex = QRegularExpression("(warning|WARNING|Warning):(.*)");
    m_progressRegex = QRegularExpression("(\\d+)/(\\d+):.(.*)");
}

FlatpakBuildOutputParser::~FlatpakBuildOutputParser()
{
}

QString FlatpakBuildOutputParser::processLine(const QString& line)
{
    // Sprawdź czy linia zawiera błąd
    if (parseError(line)) {
        return QString("<span style=\"color:red; font-weight:bold;\">%1</span>").arg(line);
    }
    
    // Sprawdź czy linia zawiera ostrzeżenie
    if (parseWarning(line)) {
        return QString("<span style=\"color:orange; font-weight:bold;\">%1</span>").arg(line);
    }
    
    // Sprawdź czy linia zawiera informacje o postępie
    if (parseProgress(line)) {
        return QString("<span style=\"color:blue;\">%1</span>").arg(line);
    }
    
    // Linie informacyjne o budowaniu
    if (line.contains("Building") || line.contains("Downloading") || 
        line.contains("Installing") || line.contains("Exporting")) {
        return QString("<span style=\"color:green; font-weight:bold;\">%1</span>").arg(line);
    }
    
    // Domyślne formatowanie
    return line;
}

bool FlatpakBuildOutputParser::parseError(const QString& line)
{
    QRegularExpressionMatch match = m_errorRegex.match(line);
    if (match.hasMatch()) {
        emit problemFound(KDevelop::IProblem::Error, match.captured(2).trimmed(), 
                          -1, -1, "flatpak-builder");
        return true;
    }
    
    return false;
}

bool FlatpakBuildOutputParser::parseWarning(const QString& line)
{
    QRegularExpressionMatch match = m_warningRegex.match(line);
    if (match.hasMatch()) {
        emit problemFound(KDevelop::IProblem::Warning, match.captured(2).trimmed(), 
                          -1, -1, "flatpak-builder");
        return true;
    }
    
    return false;
}

bool FlatpakBuildOutputParser::parseProgress(const QString& line)
{
    QRegularExpressionMatch match = m_progressRegex.match(line);
    if (match.hasMatch()) {
        int current = match.captured(1).toInt();
        int total = match.captured(2).toInt();
        int percentage = (current * 100) / total;
        
        emit progress(percentage, 100);
        return true;
    }
    
    return false;
}