#include "../include/GamblingSession.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cctype>

// Default constructor
GamblingSession::GamblingSession()
    : date(""), location(""), state(""), gameType(""), buyIn(0.0), cashOut(0.0),
      taxWithheld(false), withheldAmount(0.0), documentationNote(""), notes("")
{
}

// Parameterized constructor
GamblingSession::GamblingSession(const std::string& date, const std::string& location,
                               const std::string& state, const std::string& gameType,
                               double buyIn, double cashOut, bool taxWithheld,
                               double withheldAmount, const std::string& documentationNote,
                               const std::string& notes)
    : date(date), location(location), state(state), gameType(gameType), buyIn(buyIn),
      cashOut(cashOut), taxWithheld(taxWithheld), withheldAmount(withheldAmount),
      documentationNote(documentationNote), notes(notes)
{
}

bool GamblingSession::triggersWithholding() const
{
    double winnings = getNetResult();
    if (winnings <= 0) return false;
    
    // IRS withholding thresholds by game type
    if (gameType == "Lottery" || gameType == "Sweepstakes")
    {
        return winnings >= 5000.0;  // $5,000+ for lottery/sweepstakes
    }
    else if (gameType == "Slot Machine" || gameType == "Bingo" || gameType == "Keno")
    {
        return winnings >= 1200.0;  // $1,200+ for slots/bingo/keno
    }
    else if (gameType == "Poker Tournament")
    {
        return winnings >= 5000.0;  // $5,000+ for poker tournaments
    }
    else if (gameType == "Horse Racing" || gameType == "Dog Racing")
    {
        return winnings >= 600.0 && (winnings >= buyIn * 300); // $600+ and 300-to-1 odds
    }
    
    return false; // Default: no withholding required
}

std::string GamblingSession::toString() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Date: " << date << "\n"
        << "Location: " << location << " (" << state << ")\n"
        << "Game: " << gameType << "\n"
        << "Buy-in: $" << buyIn << "\n"
        << "Cash-out: $" << cashOut << "\n"
        << "Net Result: $" << getNetResult() << " (" 
        << (isWin() ? "WIN" : isLoss() ? "LOSS" : "BREAK EVEN") << ")\n";
    
    if (taxWithheld)
    {
        oss << "Tax Withheld: $" << withheldAmount << "\n";
    }
    
    if (triggersWithholding() && !taxWithheld)
    {
        oss << "⚠️  WARNING: This win may require tax withholding!\n";
    }
    
    if (!documentationNote.empty())
    {
        oss << "Documentation: " << documentationNote << "\n";
    }
    
    if (!notes.empty())
    {
        oss << "Notes: " << notes << "\n";
    }
    
    return oss.str();
}

std::string GamblingSession::toCSV() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << date << "," << location << "," << state << "," << gameType << "," 
        << buyIn << "," << cashOut << "," << (taxWithheld ? "1" : "0") << ","
        << withheldAmount << "," << documentationNote << "," << notes;
    return oss.str();
}

GamblingSession GamblingSession::fromCSV(const std::string& csvLine)
{
    std::istringstream iss(csvLine);
    std::string date, location, state, gameType, documentationNote, notes;
    std::string buyInStr, cashOutStr, taxWithheldStr, withheldAmountStr;
    
    // Parse CSV (basic implementation)
    std::getline(iss, date, ',');
    std::getline(iss, location, ',');
    std::getline(iss, state, ',');
    std::getline(iss, gameType, ',');
    std::getline(iss, buyInStr, ',');
    std::getline(iss, cashOutStr, ',');
    std::getline(iss, taxWithheldStr, ',');
    std::getline(iss, withheldAmountStr, ',');
    std::getline(iss, documentationNote, ',');
    std::getline(iss, notes);

    // Validate date format before creating session
    if (!isValidDate(date))
    {
        throw std::invalid_argument("Invalid date format in CSV: " + date + " (expected MM-DD-YYYY)");
    }

    double buyIn = std::stod(buyInStr);
    double cashOut = std::stod(cashOutStr);
    bool taxWithheld = (taxWithheldStr == "1");
    double withheldAmount = std::stod(withheldAmountStr);
    
    return GamblingSession(date, location, state, gameType, buyIn, cashOut,
                          taxWithheld, withheldAmount, documentationNote, notes);
}

bool GamblingSession::isValidDate(const std::string& date)
{
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5) continue;
        if (!std::isdigit(date[i])) return false;
    }

    int month = std::stoi(date.substr(0, 2));
    int day = std::stoi(date.substr(3, 2));
    int year = std::stoi(date.substr(6, 4));

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    // Basic month-specific day validation
    if (month == 2)
    {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30) return false;
    }

    return true;
}