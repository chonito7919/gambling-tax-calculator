#pragma once
#include "GamblingSession.h"
#include "TaxCalculator.h"
#include <vector>
#include <string>

class ConsoleInterface {
private:
    std::vector<GamblingSession> sessions;
    TaxCalculator calculator;
    
public:
    ConsoleInterface();
    
    // Main menu and program flow
    void run();
    void showMainMenu();
    int getUserChoice();
    
    // Session management
    void addSingleSession();
    void addBulkLosingSessions();  // For entering lots of losing tickets
    void viewAllSessions();
    void viewSessionSummary();
    
    // Tax calculations and reports
    void calculateAndShowTaxes();
    void showDocumentationReminders();
    
    // Data management
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void clearAllSessions();
    
    // Settings
    void showTaxRulesConfiguration();
    void setProfessionalMode();
    
private:
    // Input helpers
    std::string getStringInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    bool getBoolInput(const std::string& prompt);
    std::string getGameType();
    std::string getStateCode();
    std::string getCurrentDate();  // Helper for default dates
    
    // Display helpers
    void clearScreen();
    void pauseForUser();
    void showHeader(const std::string& title);
};