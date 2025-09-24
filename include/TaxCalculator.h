#pragma once
#include "GamblingSession.h"
#include "TaxRulesConfig.h"
#include <vector>
#include <string>
#include <map>
#include <set>

struct TaxSummary {
    // Federal totals
    double totalWinnings;
    double totalLosses;
    double netFederalResult;        // Can be negative
    double deductibleLosses;        // Limited to winnings amount and percentage
    double federalTaxableIncome;    // Winnings (losses deducted separately)
    
    // State totals (per state)
    std::map<std::string, double> stateWinnings;
    std::map<std::string, double> stateLosses;
    std::map<std::string, double> stateDeductibleLosses;  // Considering state percentages
    std::map<std::string, double> stateNetResults;
    
    // Withholding tracking
    double totalWithheld;
    
    // Summary flags
    bool hasWinnings;
    bool hasDeductibleLosses;
    bool itemizingRecommended;      // Based on loss amounts
    
    // Documentation reminders
    std::vector<std::string> documentationReminders;
    
    // Tax year and rule info
    int taxYear;
    std::string rulesVersion;
};

class TaxCalculator {
private:
    TaxRulesConfig taxRules;
    bool professionalGambler;  // Different rules for professionals
    
public:
    TaxCalculator(bool isProfessional = false, const std::string& configDir = "config");
    
    // Main calculation function
    TaxSummary calculateTaxes(const std::vector<GamblingSession>& sessions) const;
    
    // Rule access (now dynamic)
    bool triggersWithholding(const std::string& gameType, double winnings) const;
    double getWithholdingThreshold(const std::string& gameType) const;
    
    // Professional vs casual gambler settings
    void setProfessionalMode(bool isProfessional) { professionalGambler = isProfessional; }
    bool isProfessionalMode() const { return professionalGambler; }
    
    // Configuration access
    const TaxRulesConfig& getTaxRules() const { return taxRules; }
    TaxRulesConfig& getTaxRules() { return taxRules; }
    
    // State-specific calculations (now using dynamic rules)
    double calculateStateTax(const std::string& stateCode, double winnings, double losses) const;
    
    // Utility functions
    std::string generateTaxReport(const TaxSummary& summary) const;
    std::string generateDocumentationChecklist() const;
    std::string generateRulesReport() const;  // Show current tax rules being used
    
    // Tax year management
    void setTaxYear(int year);
    int getTaxYear() const;
    
private:
    void calculateFederalTotals(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const;
    void calculateStateTotals(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const;
    void generateReminders(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const;
};