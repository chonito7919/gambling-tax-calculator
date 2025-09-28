#include "../include/ConsoleInterface.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>
#include <chrono>
#include <ctime>
#include <set>
#include <algorithm>
#include <cctype>
#include <cmath>

ConsoleInterface::ConsoleInterface() : calculator(false)
{
    // Check if user profile exists, run setup wizard if needed
    if (!userProfile.hasProfile()) {
        userProfile.runSetupWizard();
    }
}

void ConsoleInterface::run()
{
    showHeader("GAMBLING TAX CALCULATOR");
    std::cout << "Welcome! This calculator helps track gambling wins/losses for tax purposes.\n\n";
    
    bool running = true;
    while (running)
    {
        showMainMenu();
        int choice = getUserChoice();
        
        switch (choice)
        {
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
                showUserProfile();
                break;
            case 9:
                editUserProfile();
                break;
            case 10:
                showTaxRulesConfiguration();
                break;
            case 11:
                setProfessionalMode();
                break;
            case 12:
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

void ConsoleInterface::showMainMenu()
{
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
    std::cout << "8.  View User Profile\n";
    std::cout << "9.  Edit User Profile\n";
    std::cout << "10. View Tax Rules & Configuration\n";
    std::cout << "11. Toggle Professional Gambler Mode\n";
    std::cout << "12. Clear All Sessions\n";
    std::cout << "0.  Exit\n\n";
    std::cout << "Choose an option: ";
}

int ConsoleInterface::getUserChoice()
{
    int choice;
    while (!(std::cin >> choice))
    {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(); // Clear the newline
    return choice;
}

void ConsoleInterface::addSingleSession()
{
    showHeader("ADD GAMBLING SESSION");
    
    std::string date = getDateInput("Date (MM-DD-YYYY) [Enter for today]: ");
    
    std::string location = getLocationInput("Location (casino, store, etc.): ");
    std::string state = getStateCode();
    std::string gameType = getGameType();
    
    double buyIn = getDoubleInput("Amount spent/wagered: $");
    double cashOut = getDoubleInput("Amount won/received: $");
    
    bool taxWithheld = false;
    double withheldAmount = 0.0;
    
    if (cashOut > buyIn)
    {
        taxWithheld = getBoolInput("Was tax withheld? (y/n): ");
        if (taxWithheld)
        {
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

void ConsoleInterface::addBulkLosingSessions()
{
    showHeader("BULK ADD LOSING TICKETS");
    std::cout << "Quick entry for multiple losing tickets/sessions\n\n";
    
    std::string defaultDate = getDateInput("Default date (MM-DD-YYYY) [Enter for today]: ");
    
    std::string defaultLocation = getLocationInput("Default location: ");
    std::string defaultState = getStateCode();
    std::string defaultGameType = getGameType();
    
    std::cout << "\nNow enter losing amounts (Enter 0 to finish):\n";
    
    int count = 0;
    while (true)
    {
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
    for (int i = sessions.size() - count; i < sessions.size(); i++)
    {
        totalLosses += std::abs(sessions[i].getNetResult());
    }
    
    std::cout << std::fixed << std::setprecision(2) << totalLosses << "\n";
}

void ConsoleInterface::viewAllSessions()
{
    showHeader("ALL GAMBLING SESSIONS");
    
    if (sessions.empty())
    {
        std::cout << "No sessions recorded yet.\n";
        return;
    }
    
    double totalWinnings = 0, totalLosses = 0;
    
    for (size_t i = 0; i < sessions.size(); i++)
    {
        std::cout << "\n--- Session " << (i + 1) << " ---\n";
        std::cout << sessions[i].toString();
        
        if (sessions[i].isWin())
        {
            totalWinnings += sessions[i].getNetResult();
        }
        else if (sessions[i].isLoss())
        {
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

void ConsoleInterface::calculateAndShowTaxes()
{
    showHeader("TAX CALCULATION");
    
    if (sessions.empty())
    {
        std::cout << "No sessions to calculate. Add some gambling sessions first.\n";
        return;
    }
    
    TaxSummary summary = calculator.calculateTaxes(sessions);
    std::cout << calculator.generateTaxReport(summary) << "\n";
    
    // Show any important reminders
    if (!summary.documentationReminders.empty())
    {
        std::cout << "\nIMPORTANT REMINDERS:\n";
        for (const auto& reminder : summary.documentationReminders)
        {
            std::cout << "• " << reminder << "\n";
        }
    }
}

void ConsoleInterface::showDocumentationReminders()
{
    showHeader("DOCUMENTATION CHECKLIST");
    std::cout << calculator.generateDocumentationChecklist() << "\n";
}

void ConsoleInterface::saveToFile(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cout << "❌ Error: Could not save to file " << filename << "\n";
        return;
    }
    
    // Write CSV header
    file << "Date,Location,State,GameType,BuyIn,CashOut,TaxWithheld,WithheldAmount,DocumentationNote,Notes\n";
    
    // Write sessions
    for (const auto& session : sessions)
    {
        file << session.toCSV() << "\n";
    }
    
    file.close();
    std::cout << "✅ Saved " << sessions.size() << " sessions to " << filename << "\n";
}

void ConsoleInterface::loadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "❌ Error: Could not load from file " << filename << "\n";
        return;
    }
    
    sessions.clear();
    std::string line;
    
    // Skip header line
    std::getline(file, line);
    
    int loaded = 0;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            try
            {
                sessions.push_back(GamblingSession::fromCSV(line));
                loaded++;
            }
            catch (const std::exception& e)
            {
                std::cout << "Warning: Skipped invalid line: " << line << "\n";
            }
        }
    }
    
    file.close();
    std::cout << "✅ Loaded " << loaded << " sessions from " << filename << "\n";
}

// Helper functions implementation continues...

std::string ConsoleInterface::getStringInput(const std::string& prompt)
{
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string ConsoleInterface::getLocationInput(const std::string& prompt)
{
    std::string input;
    const size_t MAX_LENGTH = 100;  // Reasonable maximum for location names

    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\r\n"));
        input.erase(input.find_last_not_of(" \t\r\n") + 1);

        if (input.empty())
        {
            std::cout << "Location cannot be empty. Please enter a location name.\n";
            continue;
        }

        if (input.length() > MAX_LENGTH)
        {
            std::cout << "Location name too long (max " << MAX_LENGTH
                     << " characters). Please enter a shorter name.\n";
            continue;
        }

        // Check for reasonable characters (letters, numbers, spaces, basic punctuation)
        bool hasValidChars = true;
        for (char c : input)
        {
            if (!std::isalnum(c) && c != ' ' && c != '-' && c != '\'' && c != '.' && c != '&' && c != ',')
            {
                hasValidChars = false;
                break;
            }
        }

        if (!hasValidChars)
        {
            std::cout << "Location contains invalid characters. Please use only letters, numbers, spaces, and basic punctuation.\n";
            continue;
        }

        return input;
    }
}

std::string ConsoleInterface::getDateInput(const std::string& prompt)
{
    std::string input;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);

        // Allow empty input for default date
        if (input.empty())
        {
            return getCurrentDate();
        }

        // Validate the entered date
        if (isValidDate(input))
        {
            return input;
        }

        std::cout << "Invalid date format. Please enter date as MM-DD-YYYY (e.g., 01-15-2024).\n";
    }
}

double ConsoleInterface::getDoubleInput(const std::string& prompt)
{
    double value;
    const double MAX_AMOUNT = 999999.99;  // Reasonable maximum for gambling amounts

    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            std::cin.ignore();

            if (value < 0)
            {
                std::cout << "Amount cannot be negative. Please enter a positive number.\n";
                continue;
            }

            if (value > MAX_AMOUNT)
            {
                std::cout << "Amount too large (max $" << std::fixed << std::setprecision(2)
                         << MAX_AMOUNT << "). Please enter a reasonable amount.\n";
                continue;
            }

            // Check for excessive decimal places (more than 2)
            double rounded = std::round(value * 100.0) / 100.0;
            if (std::abs(value - rounded) > 0.001)
            {
                std::cout << "Please enter amount with at most 2 decimal places (e.g., 123.45).\n";
                continue;
            }

            return rounded;  // Return rounded value for currency consistency
        }

        std::cout << "Please enter a valid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool ConsoleInterface::getBoolInput(const std::string& prompt)
{
    std::string input;
    while (true)
    {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty()) return false;
        
        char c = std::tolower(input[0]);
        if (c == 'y' || c == '1') return true;
        if (c == 'n' || c == '0') return false;
        
        std::cout << "Please enter 'y' for yes or 'n' for no.\n";
    }
}

std::string ConsoleInterface::getGameType()
{
    std::cout << "\nGame Types:\n";
    std::cout << "1. Lottery/Scratch-off\n";
    std::cout << "2. Slot Machine\n";
    std::cout << "3. Poker\n";
    std::cout << "4. Blackjack\n";
    std::cout << "5. Sports Betting\n";
    std::cout << "6. Other\n";
    std::cout << "Choose game type (1-6): ";
    
    int choice = getUserChoice();
    switch (choice)
    {
        case 1: return "Lottery";
        case 2: return "Slot Machine";
        case 3: return "Poker";
        case 4: return "Blackjack";
        case 5: return "Sports Betting";
        case 6: return getStringInput("Enter game type: ");
        default: return "Other";
    }
}

std::string ConsoleInterface::getStateCode()
{
    std::string homeState = userProfile.getHomeState();

    std::cout << "\nState for this session:\n";
    std::cout << "1. Use home state (" << homeState << ")\n";
    std::cout << "2. NJ (New Jersey)  3. PA (Pennsylvania)  4. NY (New York)\n";
    std::cout << "5. FL (Florida)     6. NV (Nevada)        7. CA (California)\n";
    std::cout << "8. Other state\n";
    std::cout << "Choose state (1-8): ";

    int choice = getUserChoice();
    switch (choice)
    {
        case 1: return homeState;
        case 2: return "NJ";
        case 3: return "PA";
        case 4: return "NY";
        case 5: return "FL";
        case 6: return "NV";
        case 7: return "CA";
        case 8:
        {
            std::string stateCode;
            while (true)
            {
                stateCode = getStringInput("Enter state code (e.g., TX, OH): ");
                if (isValidStateCode(stateCode))
                {
                    // Convert to uppercase for consistency
                    std::transform(stateCode.begin(), stateCode.end(), stateCode.begin(), ::toupper);
                    return stateCode;
                }
                std::cout << "Invalid state code '" << stateCode << "'. Please enter a valid 2-letter US state code.\n";
            }
        }
        default: return homeState;
    }
}

std::string ConsoleInterface::getCurrentDate()
{
    std::time_t now = std::time(nullptr);

    // If timezone is not set in environment, adjust for user's configured timezone
    if (!getenv("TZ")) {
        std::string userTz = userProfile.getTimezone();

        // Simple timezone offset adjustments (this is a basic implementation)
        // In production, you'd want a proper timezone library
        if (userTz == "America/New_York") {
            now -= 18000; // UTC-5 (EST)
        } else if (userTz == "America/Chicago") {
            now -= 21600; // UTC-6 (CST)
        } else if (userTz == "America/Denver") {
            now -= 25200; // UTC-7 (MST)
        } else if (userTz == "America/Los_Angeles") {
            now -= 28800; // UTC-8 (PST)
        } else if (userTz == "America/Phoenix") {
            now -= 25200; // UTC-7 (no DST)
        } else if (userTz == "America/Anchorage") {
            now -= 32400; // UTC-9 (AKST)
        } else if (userTz == "Pacific/Honolulu") {
            now -= 36000; // UTC-10 (HST)
        }
    }

    std::tm* local_tm = std::localtime(&now);

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << (local_tm->tm_mon + 1) << "-"
        << std::setfill('0') << std::setw(2) << local_tm->tm_mday << "-"
        << (local_tm->tm_year + 1900);
    return oss.str();
}

bool ConsoleInterface::isValidDate(const std::string& date)
{
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;

    for (int i = 0; i < 10; i++) {
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
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (isLeap ? 29 : 28)) return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }

    return true;
}

bool ConsoleInterface::isValidStateCode(const std::string& stateCode)
{
    // List of all 50 US states + DC (extracted from state_rules.cfg)
    static const std::set<std::string> validStates = {
        "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "FL", "GA",
        "HI", "ID", "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD",
        "MA", "MI", "MN", "MS", "MO", "MT", "NE", "NV", "NH", "NJ",
        "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC",
        "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV", "WI", "WY",
        "DC"  // Washington D.C.
    };

    // Convert to uppercase for case-insensitive comparison
    std::string upper = stateCode;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

    return validStates.find(upper) != validStates.end();
}

void ConsoleInterface::clearScreen()
{
    // Simple screen clear - works on most terminals
    std::cout << "\n" << std::string(60, '=') << "\n";
}

void ConsoleInterface::pauseForUser()
{
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void ConsoleInterface::showHeader(const std::string& title)
{
    clearScreen();
    std::cout << title << "\n";
    std::cout << std::string(title.length(), '-') << "\n\n";
}

void ConsoleInterface::showTaxRulesConfiguration()
{
    showHeader("TAX RULES CONFIGURATION");
    std::cout << calculator.generateRulesReport() << "\n\n";
    
    std::cout << "CONFIGURATION FILES:\n";
    std::cout << "• Federal rules: config/federal_rules.cfg\n";
    std::cout << "• State rules: config/state_rules.cfg\n\n";
    
    std::cout << "💡 TIP: Edit these files to update tax rules without recompiling!\n";
    std::cout << "💡 TIP: Add new states or modify existing rules as needed\n";
}

void ConsoleInterface::setProfessionalMode()
{
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

void ConsoleInterface::clearAllSessions()
{
    if (sessions.empty())
    {
        std::cout << "No sessions to clear.\n";
        return;
    }
    
    std::cout << "This will delete all " << sessions.size() << " sessions.\n";
    bool confirm = getBoolInput("Are you sure? (y/n): ");
    
    if (confirm)
    {
        sessions.clear();
        std::cout << "✅ All sessions cleared.\n";
    }
    else
    {
        std::cout << "Cancelled.\n";
    }
}

void ConsoleInterface::showUserProfile()
{
    showHeader("USER PROFILE");
    userProfile.displayCurrentProfile();
    std::cout << "\nNote: Your profile settings affect tax calculations and date handling.\n";
    std::cout << "Use 'Edit User Profile' from the main menu to modify these settings.\n";
}

void ConsoleInterface::editUserProfile()
{
    showHeader("EDIT USER PROFILE");
    std::cout << "This will launch the setup wizard to reconfigure your profile.\n";
    std::cout << "Your current settings will be overwritten.\n\n";

    bool confirm = getBoolInput("Continue with profile setup? (y/n): ");
    if (confirm) {
        userProfile.runSetupWizard();
        std::cout << "\n✅ Profile updated successfully!\n";
    } else {
        std::cout << "Profile edit cancelled.\n";
    }
}
