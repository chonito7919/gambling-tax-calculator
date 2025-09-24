#include "../include/ConsoleInterface.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>

ConsoleInterface::ConsoleInterface() : calculator(false) {
}

void ConsoleInterface::run() {
    showHeader("GAMBLING TAX CALCULATOR");
    std::cout << "Welcome! This calculator helps track gambling wins/losses for tax purposes.\n\n";
    
    bool running = true;
    while (running) {
        showMainMenu();
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                addSingleSession();
                break;
            case 2:
                addBulkLosingSessions();
                break;
            case 3:
                viewAllSessions();
                break;
            case 4:
                calculateAndShowTaxes();
                break;
            case 5:
                showDocumentationReminders();
                break;
            case 6:
                saveToFile("gambling_sessions.csv");
                break;
            case 7:
                loadFromFile("gambling_sessions.csv");
                break;
            case 8:
                showTaxRulesConfiguration();    // <-- CHANGE THIS LINE
                break;
            case 9:
                setProfessionalMode();
                break;
            case 10:
                clearAllSessions();
                break;
            case 0:
                running = false;
                std::cout << "Thank you for using Gambling Tax Calculator!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
        
        if (running) pauseForUser();
    }
}

void ConsoleInterface::showMainMenu() {
    clearScreen();
    showHeader("MAIN MENU");
    std::cout << "Sessions loaded: " << sessions.size() << "\n\n";
    
    std::cout << "1.  Add Single Gambling Session\n";
    std::cout << "2.  Bulk Add Losing Tickets (Quick Entry)\n";
    std::cout << "3.  View All Sessions\n";
    std::cout << "4.  Calculate Tax Implications\n";
    std::cout << "5.  Show Documentation Checklist\n";
    std::cout << "6.  Save Sessions to File\n";
    std::cout << "7.  Load Sessions from File\n";
    std::cout << "8.  View Tax Rules & Configuration\n";    // <-- CHANGE THIS LINE
    std::cout << "9.  Toggle Professional Gambler Mode\n";
    std::cout << "10. Clear All Sessions\n";
    std::cout << "0.  Exit\n\n";
    std::cout << "Choose an option: ";
}

int ConsoleInterface::getUserChoice() {
    int choice;
    while (!(std::cin >> choice)) {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // Clear the newline
    return choice;
}

void ConsoleInterface::addSingleSession() {
    showHeader("ADD GAMBLING SESSION");
    
    std::string date = getStringInput("Date (YYYY-MM-DD) [Enter for today]: ");
    if (date.empty()) {
        date = getCurrentDate();
    }
    
    std::string location = getStringInput("Location (casino, store, etc.): ");
    std::string state = getStateCode();
    std::string gameType = getGameType();
    
    double buyIn = getDoubleInput("Amount spent/wagered: $");
    double cashOut = getDoubleInput("Amount won/received: $");
    
    bool taxWithheld = false;
    double withheldAmount = 0.0;
    
    if (cashOut > buyIn) {
        taxWithheld = getBoolInput("Was tax withheld? (y/n): ");
        if (taxWithheld) {
            withheldAmount = getDoubleInput("Amount withheld: $");
        }
    }
    
    std::string docNote = getStringInput("Documentation note (optional): ");
    std::string notes = getStringInput("Additional notes (optional): ");
    
    GamblingSession session(date, location, state, gameType, buyIn, cashOut,
                           taxWithheld, withheldAmount, docNote, notes);
    
    sessions.push_back(session);
    
    std::cout << "\n✅ Session added successfully!\n";
    std::cout << "Net result: $" << std::fixed << std::setprecision(2) 
              << session.getNetResult() << " (" 
              << (session.isWin() ? "WIN" : session.isLoss() ? "LOSS" : "BREAK EVEN") << ")\n";
}

void ConsoleInterface::addBulkLosingSessions() {
    showHeader("BULK ADD LOSING TICKETS");
    std::cout << "Quick entry for multiple losing tickets/sessions\n\n";
    
    std::string defaultDate = getStringInput("Default date (YYYY-MM-DD) [Enter for today]: ");
    if (defaultDate.empty()) {
        defaultDate = getCurrentDate();
    }
    
    std::string defaultLocation = getStringInput("Default location: ");
    std::string defaultState = getStateCode();
    std::string defaultGameType = getGameType();
    
    std::cout << "\nNow enter losing amounts (Enter 0 to finish):\n";
    
    int count = 0;
    while (true) {
        double amount = getDoubleInput("Losing ticket amount $");
        if (amount <= 0) break;
        
        // Create losing session (buyIn = amount, cashOut = 0)
        GamblingSession session(defaultDate, defaultLocation, defaultState, 
                               defaultGameType, amount, 0.0, false, 0.0, 
                               "Keep losing ticket", "Bulk entry loss");
        sessions.push_back(session);
        count++;
        
        std::cout << "Added loss #" << count << ": $" << std::fixed << std::setprecision(2) << amount << "\n";
    }
    
    std::cout << "\n✅ Added " << count << " losing sessions totaling $";
    
    double totalLosses = 0;
    for (int i = sessions.size() - count; i < sessions.size(); i++) {
        totalLosses += std::abs(sessions[i].getNetResult());
    }
    
    std::cout << std::fixed << std::setprecision(2) << totalLosses << "\n";
}

void ConsoleInterface::viewAllSessions() {
    showHeader("ALL GAMBLING SESSIONS");
    
    if (sessions.empty()) {
        std::cout << "No sessions recorded yet.\n";
        return;
    }
    
    double totalWinnings = 0, totalLosses = 0;
    
    for (size_t i = 0; i < sessions.size(); i++) {
        std::cout << "\n--- Session " << (i + 1) << " ---\n";
        std::cout << sessions[i].toString();
        
        if (sessions[i].isWin()) {
            totalWinnings += sessions[i].getNetResult();
        } else if (sessions[i].isLoss()) {
            totalLosses += std::abs(sessions[i].getNetResult());
        }
    }
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "SUMMARY:\n";
    std::cout << "Total Sessions: " << sessions.size() << "\n";
    std::cout << "Total Winnings: $" << std::fixed << std::setprecision(2) << totalWinnings << "\n";
    std::cout << "Total Losses: $" << totalLosses << "\n";
    std::cout << "Net Result: $" << (totalWinnings - totalLosses) << "\n";
}

void ConsoleInterface::calculateAndShowTaxes() {
    showHeader("TAX CALCULATION");
    
    if (sessions.empty()) {
        std::cout << "No sessions to calculate. Add some gambling sessions first.\n";
        return;
    }
    
    TaxSummary summary = calculator.calculateTaxes(sessions);
    std::cout << calculator.generateTaxReport(summary) << "\n";
    
    // Show any important reminders
    if (!summary.documentationReminders.empty()) {
        std::cout << "\nIMPORTANT REMINDERS:\n";
        for (const auto& reminder : summary.documentationReminders) {
            std::cout << "• " << reminder << "\n";
        }
    }
}

void ConsoleInterface::showDocumentationReminders() {
    showHeader("DOCUMENTATION CHECKLIST");
    std::cout << calculator.generateDocumentationChecklist() << "\n";
}

void ConsoleInterface::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "❌ Error: Could not save to file " << filename << "\n";
        return;
    }
    
    // Write CSV header
    file << "Date,Location,State,GameType,BuyIn,CashOut,TaxWithheld,WithheldAmount,DocumentationNote,Notes\n";
    
    // Write sessions
    for (const auto& session : sessions) {
        file << session.toCSV() << "\n";
    }
    
    file.close();
    std::cout << "✅ Saved " << sessions.size() << " sessions to " << filename << "\n";
}

void ConsoleInterface::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "❌ Error: Could not load from file " << filename << "\n";
        return;
    }
    
    sessions.clear();
    std::string line;
    
    // Skip header line
    std::getline(file, line);
    
    int loaded = 0;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                sessions.push_back(GamblingSession::fromCSV(line));
                loaded++;
            } catch (const std::exception& e) {
                std::cout << "Warning: Skipped invalid line: " << line << "\n";
            }
        }
    }
    
    file.close();
    std::cout << "✅ Loaded " << loaded << " sessions from " << filename << "\n";
}

// Helper functions implementation continues...

std::string ConsoleInterface::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

double ConsoleInterface::getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            std::cin.ignore();
            return value;
        }
        std::cout << "Please enter a valid positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool ConsoleInterface::getBoolInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty()) return false;
        
        char c = std::tolower(input[0]);
        if (c == 'y' || c == '1') return true;
        if (c == 'n' || c == '0') return false;
        
        std::cout << "Please enter 'y' for yes or 'n' for no.\n";
    }
}

std::string ConsoleInterface::getGameType() {
    std::cout << "\nGame Types:\n";
    std::cout << "1. Lottery/Scratch-off\n";
    std::cout << "2. Slot Machine\n";
    std::cout << "3. Poker\n";
    std::cout << "4. Blackjack\n";
    std::cout << "5. Sports Betting\n";
    std::cout << "6. Other\n";
    std::cout << "Choose game type (1-6): ";
    
    int choice = getUserChoice();
    switch (choice) {
        case 1: return "Lottery";
        case 2: return "Slot Machine";
        case 3: return "Poker";
        case 4: return "Blackjack";
        case 5: return "Sports Betting";
        case 6: return getStringInput("Enter game type: ");
        default: return "Other";
    }
}

std::string ConsoleInterface::getStateCode() {
    std::cout << "\nCommon States:\n";
    std::cout << "1. NJ (New Jersey)  2. PA (Pennsylvania)  3. NY (New York)\n";
    std::cout << "4. FL (Florida)     5. NV (Nevada)        6. CA (California)\n";
    std::cout << "7. Other state\n";
    std::cout << "Choose state (1-7): ";
    
    int choice = getUserChoice();
    switch (choice) {
        case 1: return "NJ";
        case 2: return "PA";
        case 3: return "NY";
        case 4: return "FL";
        case 5: return "NV";
        case 6: return "CA";
        case 7: return getStringInput("Enter state code (e.g., TX, OH): ");
        default: return "NJ";
    }
}

std::string ConsoleInterface::getCurrentDate() {
    // Simple date - in a real app you'd use proper date handling
    return "2024-01-01"; // Placeholder - user can override
}

void ConsoleInterface::clearScreen() {
    // Simple screen clear - works on most terminals
    std::cout << "\n" << std::string(60, '=') << "\n";
}

void ConsoleInterface::pauseForUser() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void ConsoleInterface::showHeader(const std::string& title) {
    clearScreen();
    std::cout << title << "\n";
    std::cout << std::string(title.length(), '-') << "\n\n";
}

void ConsoleInterface::showTaxRulesConfiguration() {
    showHeader("TAX RULES CONFIGURATION");
    std::cout << calculator.generateRulesReport() << "\n\n";
    
    std::cout << "CONFIGURATION FILES:\n";
    std::cout << "• Federal rules: config/federal_rules.cfg\n";
    std::cout << "• State rules: config/state_rules.cfg\n\n";
    
    std::cout << "💡 TIP: Edit these files to update tax rules without recompiling!\n";
    std::cout << "💡 TIP: Add new states or modify existing rules as needed\n";
}

void ConsoleInterface::setProfessionalMode() {
    showHeader("PROFESSIONAL GAMBLER MODE");
    std::cout << "Current mode: " << (calculator.isProfessionalMode() ? "Professional" : "Casual") << "\n\n";
    std::cout << "Professional gamblers:\n";
    std::cout << "• Report on Schedule C (business income)\n";
    std::cout << "• Can deduct business expenses\n";
    std::cout << "• Subject to self-employment tax\n\n";
    
    bool professional = getBoolInput("Enable professional gambler mode? (y/n): ");
    calculator.setProfessionalMode(professional);
    
    std::cout << "✅ Mode set to: " << (professional ? "Professional" : "Casual") << "\n";
}

void ConsoleInterface::clearAllSessions() {
    if (sessions.empty()) {
        std::cout << "No sessions to clear.\n";
        return;
    }
    
    std::cout << "This will delete all " << sessions.size() << " sessions.\n";
    bool confirm = getBoolInput("Are you sure? (y/n): ");
    
    if (confirm) {
        sessions.clear();
        std::cout << "✅ All sessions cleared.\n";
    } else {
        std::cout << "Cancelled.\n";
    }
}
