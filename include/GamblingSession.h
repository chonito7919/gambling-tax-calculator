#pragma once
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

class GamblingSession {
private:
    std::string date;           // MM-DD-YYYY format
    std::string location;       // Casino name, online site, etc.
    std::string state;          // State where gambling occurred (critical for taxes)
    std::string gameType;       // Lottery, Slots, Poker, Sports Betting, etc.
    double buyIn;              // Amount you started with
    double cashOut;            // Amount you left with
    bool taxWithheld;          // Was federal tax withheld?
    double withheldAmount;     // Amount withheld (if any)
    std::string documentationNote; // Reminder about keeping receipts
    std::string notes;         // Optional additional notes

public:
    // Constructors
    GamblingSession();
    GamblingSession(const std::string& date, const std::string& location, 
                   const std::string& state, const std::string& gameType, 
                   double buyIn, double cashOut, bool taxWithheld = false,
                   double withheldAmount = 0.0, const std::string& documentationNote = "",
                   const std::string& notes = "");

    // Getters
    std::string getDate() const { return date; }
    std::string getLocation() const { return location; }
    std::string getState() const { return state; }
    std::string getGameType() const { return gameType; }
    double getBuyIn() const { return buyIn; }
    double getCashOut() const { return cashOut; }
    bool getTaxWithheld() const { return taxWithheld; }
    double getWithheldAmount() const { return withheldAmount; }
    std::string getDocumentationNote() const { return documentationNote; }
    std::string getNotes() const { return notes; }

    // Calculated properties
    double getNetResult() const { return cashOut - buyIn; }
    bool isWin() const { return getNetResult() > 0; }
    bool isLoss() const { return getNetResult() < 0; }
    bool triggersWithholding() const;  // Based on IRS thresholds

    // Setters
    void setDate(const std::string& date) { this->date = date; }
    void setLocation(const std::string& location) { this->location = location; }
    void setState(const std::string& state) { this->state = state; }
    void setGameType(const std::string& gameType) { this->gameType = gameType; }
    void setBuyIn(double buyIn) { this->buyIn = buyIn; }
    void setCashOut(double cashOut) { this->cashOut = cashOut; }
    void setTaxWithheld(bool taxWithheld) { this->taxWithheld = taxWithheld; }
    void setWithheldAmount(double withheldAmount) { this->withheldAmount = withheldAmount; }
    void setDocumentationNote(const std::string& note) { this->documentationNote = note; }
    void setNotes(const std::string& notes) { this->notes = notes; }

    // Utility functions
    std::string toString() const;
    std::string toCSV() const;
    
    // For file I/O
    static GamblingSession fromCSV(const std::string& csvLine);
};