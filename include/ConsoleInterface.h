#pragma once
#include "GamblingSession.h"
#include "TaxCalculator.h"
#include "UserProfile.h"
#include <vector>
#include <string>

class ConsoleInterface {
private:
    std::vector<GamblingSession> sessions;
    TaxCalculator calculator;
    UserProfile userProfile;
    
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
    void showUserProfile();
    void editUserProfile();
    
private:
    // Input helpers
    std::string getStringInput(const std::string& prompt);
    std::string getDateInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    bool getBoolInput(const std::string& prompt);
    std::string getGameType();
    std::string getStateCode();
    std::string getCurrentDate();  // Helper for default dates
    bool isValidDate(const std::string& date);  // Validate MM-DD-YYYY format
    
    // Display helpers
    void clearScreen();
    void pauseForUser();
    void showHeader(const std::string& title);
};