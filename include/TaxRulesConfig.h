#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>

struct FederalTaxRules {
    int taxYear;
    double standardDeduction;
    double itemizationThreshold;
    bool allowsLossDeduction;
    double lossDeductionLimit;  // 1.0 = 100%, 0.9 = 90% (for 2026+ rules)
    std::map<std::string, double> withholdingThresholds;  // game_type -> amount
    
    FederalTaxRules() : taxYear(2024), standardDeduction(14600), itemizationThreshold(1000),
                       allowsLossDeduction(true), lossDeductionLimit(1.0)
    {
        // Default withholding thresholds
        withholdingThresholds["Lottery"] = 5000.0;
        withholdingThresholds["Slot Machine"] = 1200.0;
        withholdingThresholds["Bingo"] = 1200.0;
        withholdingThresholds["Keno"] = 1200.0;
        withholdingThresholds["Poker Tournament"] = 5000.0;
    }
};

struct StateTaxRule {
    std::string stateName;
    std::string stateCode;
    bool hasIncomeTax;
    double taxRate;
    bool allowsLossDeduction;
    double lossDeductionPercentage;  // 1.0 = 100%, 0.5 = 50%, 0.0 = no deduction
    std::string specialRules;
    bool requiresNonResidentReturn;
    double withholdingThreshold;
    
    StateTaxRule() : stateName(""), stateCode(""), hasIncomeTax(true), taxRate(0.0),
                    allowsLossDeduction(true), lossDeductionPercentage(1.0), specialRules(""),
                    requiresNonResidentReturn(false), withholdingThreshold(5000.0) {}
};

class TaxRulesConfig
{
private:
    FederalTaxRules federalRules;
    std::map<std::string, StateTaxRule> stateRules;
    std::string configDirectory;
    
public:
    TaxRulesConfig(const std::string& configDir = "config");
    
    // Load/Save configuration files
    bool loadFederalRules(const std::string& filename = "federal_rules.cfg");
    bool loadStateRules(const std::string& filename = "state_rules.cfg");
    bool saveFederalRules(const std::string& filename = "federal_rules.cfg");
    bool saveStateRules(const std::string& filename = "state_rules.cfg");
    
    // Create default configuration files if they don't exist
    void createDefaultConfigs();
    
    // Federal rules access
    const FederalTaxRules& getFederalRules() const { return federalRules; }
    void setFederalRules(const FederalTaxRules& rules) { federalRules = rules; }
    
    // State rules access
    const StateTaxRule* getStateRule(const std::string& stateCode) const;
    void addStateRule(const std::string& stateCode, const StateTaxRule& rule);
    std::vector<std::string> getAvailableStates() const;
    
    // Rule queries
    bool allowsLossDeduction(const std::string& stateCode) const;
    double getLossDeductionPercentage(const std::string& stateCode) const;
    double getStateTaxRate(const std::string& stateCode) const;
    double getWithholdingThreshold(const std::string& gameType) const;
    
    // Update rules for specific tax years
    void updateForTaxYear(int year);
    
    // Utility functions
    std::string getConfigPath(const std::string& filename) const;
    void printCurrentRules() const;
    
private:
    // File parsing helpers
    std::map<std::string, std::string> parseConfigLine(const std::string& line);
    std::string trim(const std::string& str);
    bool parseBool(const std::string& value);
    double parseDouble(const std::string& value);
};