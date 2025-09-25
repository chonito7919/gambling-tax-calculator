#include "../include/TaxRulesConfig.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

TaxRulesConfig::TaxRulesConfig(const std::string& configDir) : configDirectory(configDir)
{
    // Create config directory if it doesn't exist
    try
    {
        std::filesystem::create_directories(configDirectory);
    }
    catch (const std::exception& e)
    {
        std::cout << "Warning: Could not create config directory: " << e.what() << std::endl;
    }
    
    // Try to load existing configs, create defaults if they don't exist
    if (!loadFederalRules())
    {
        createDefaultConfigs();
        loadFederalRules();
    }
    if (!loadStateRules())
    {
        loadStateRules();  // Try again after creating defaults
    }
}

bool TaxRulesConfig::loadFederalRules(const std::string& filename)
{
    std::ifstream file(getConfigPath(filename));
    if (!file.is_open())
    {
        std::cout << "Could not load federal rules from " << filename << std::endl;
        return false;
    }
    
    std::string line, currentSection;
    while (std::getline(file, line))
    {
        line = trim(line);
        
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Check for section headers
        if (line[0] == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.length() - 2);
            continue;
        }
        
        // Parse key-value pairs
        auto keyValue = parseConfigLine(line);
        if (keyValue.empty()) continue;
        
        std::string key = keyValue.begin()->first;
        std::string value = keyValue.begin()->second;
        
        if (currentSection == "GENERAL")
        {
            if (key == "tax_year") federalRules.taxYear = std::stoi(value);
            else if (key == "standard_deduction_single") federalRules.standardDeduction = parseDouble(value);
            else if (key == "itemization_threshold") federalRules.itemizationThreshold = parseDouble(value);
        }
        else if (currentSection == "LOSS_DEDUCTIONS")
        {
            if (key == "allows_loss_deduction") federalRules.allowsLossDeduction = parseBool(value);
            else if (key == "loss_deduction_limit") federalRules.lossDeductionLimit = parseDouble(value);
        }
        else if (currentSection == "WITHHOLDING_THRESHOLDS")
        {
            federalRules.withholdingThresholds[key] = parseDouble(value);
        }
    }
    
    file.close();
    return true;
}

bool TaxRulesConfig::loadStateRules(const std::string& filename)
{
    std::ifstream file(getConfigPath(filename));
    if (!file.is_open())
    {
        std::cout << "Could not load state rules from " << filename << std::endl;
        return false;
    }
    
    std::string line, currentState;
    StateTaxRule currentRule;
    
    while (std::getline(file, line))
    {
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') continue;
        
        // Check for state section headers [STATE_CODE]
        if (line[0] == '[' && line.back() == ']')
        {
            // Save previous state if we have one
            if (!currentState.empty())
            {
                stateRules[currentState] = currentRule;
            }
            
            // Start new state
            currentState = line.substr(1, line.length() - 2);
            currentRule = StateTaxRule();  // Reset to defaults
            currentRule.stateCode = currentState;
            continue;
        }
        
        // Parse key-value pairs for current state
        auto keyValue = parseConfigLine(line);
        if (keyValue.empty()) continue;
        
        std::string key = keyValue.begin()->first;
        std::string value = keyValue.begin()->second;
        
        if (key == "state_name") currentRule.stateName = value;
        else if (key == "has_income_tax") currentRule.hasIncomeTax = parseBool(value);
        else if (key == "tax_rate") currentRule.taxRate = parseDouble(value);
        else if (key == "allows_loss_deduction") currentRule.allowsLossDeduction = parseBool(value);
        else if (key == "loss_deduction_percentage") currentRule.lossDeductionPercentage = parseDouble(value);
        else if (key == "special_rules") currentRule.specialRules = value;
        else if (key == "requires_nonresident_return") currentRule.requiresNonResidentReturn = parseBool(value);
        else if (key == "withholding_threshold") currentRule.withholdingThreshold = parseDouble(value);
    }
    
    // Don't forget the last state
    if (!currentState.empty())
    {
        stateRules[currentState] = currentRule;
    }
    
    file.close();
    return true;
}

void TaxRulesConfig::createDefaultConfigs()
{
    // Create federal rules config file
    std::ofstream federalFile(getConfigPath("federal_rules.cfg"));
    if (federalFile.is_open())
    {
        federalFile << "# Federal Gambling Tax Rules Configuration\n";
        federalFile << "# Format: key = value\n\n";
        federalFile << "[GENERAL]\n";
        federalFile << "tax_year = 2024\n";
        federalFile << "standard_deduction_single = 14600\n";
        federalFile << "itemization_threshold = 1000\n\n";
        federalFile << "[LOSS_DEDUCTIONS]\n";
        federalFile << "allows_loss_deduction = true\n";
        federalFile << "loss_deduction_limit = 1.0\n\n";
        federalFile << "[WITHHOLDING_THRESHOLDS]\n";
        federalFile << "Lottery = 5000\n";
        federalFile << "Slot_Machine = 1200\n";
        federalFile.close();
    }
    
    // Create state rules config file with a few examples
    std::ofstream stateFile(getConfigPath("state_rules.cfg"));
    if (stateFile.is_open())
    {
        stateFile << "# State Gambling Tax Rules Configuration\n\n";
        stateFile << "[NJ]\n";
        stateFile << "state_name = New Jersey\n";
        stateFile << "has_income_tax = true\n";
        stateFile << "tax_rate = 0.08875\n";
        stateFile << "allows_loss_deduction = true\n";
        stateFile << "loss_deduction_percentage = 1.0\n";
        stateFile << "special_rules = Allows loss deductions\n\n";
        
        stateFile << "[NY_HISTORICAL_2007]\n";
        stateFile << "state_name = New York (2007-2008 Rules)\n";
        stateFile << "has_income_tax = true\n";
        stateFile << "tax_rate = 0.08\n";
        stateFile << "allows_loss_deduction = true\n";
        stateFile << "loss_deduction_percentage = 0.5\n";
        stateFile << "special_rules = Historical rule - only 50% of losses could be deducted\n\n";
        
        stateFile.close();
    }
}

const StateTaxRule* TaxRulesConfig::getStateRule(const std::string& stateCode) const {
    auto it = stateRules.find(stateCode);
    return (it != stateRules.end()) ? &it->second : nullptr;
}

void TaxRulesConfig::addStateRule(const std::string& stateCode, const StateTaxRule& rule)
{
    stateRules[stateCode] = rule;
}

std::vector<std::string> TaxRulesConfig::getAvailableStates() const
{
    std::vector<std::string> states;
    for (const auto& pair : stateRules)
    {
        states.push_back(pair.second.stateName + " (" + pair.first + ")");
    }
    return states;
}

bool TaxRulesConfig::allowsLossDeduction(const std::string& stateCode) const
{
    const StateTaxRule* rule = getStateRule(stateCode);
    return rule ? rule->allowsLossDeduction : true; // Default: allow deductions
}

double TaxRulesConfig::getLossDeductionPercentage(const std::string& stateCode) const
{
    const StateTaxRule* rule = getStateRule(stateCode);
    return rule ? rule->lossDeductionPercentage : 1.0; // Default to 100%
}

double TaxRulesConfig::getStateTaxRate(const std::string& stateCode) const
{
    const StateTaxRule* rule = getStateRule(stateCode);
    return rule ? rule->taxRate : 0.0; // Default: no state tax
}

double TaxRulesConfig::getWithholdingThreshold(const std::string& gameType) const
{
    const FederalTaxRules& federalRules = getFederalRules();
    auto it = federalRules.withholdingThresholds.find(gameType);
    return (it != federalRules.withholdingThresholds.end()) ? it->second : 0.0;
}

void TaxRulesConfig::updateForTaxYear(int year)
{
    // This could automatically adjust rules based on the year
    federalRules.taxYear = year;
    
    // Example: Apply 2026 rule changes
    if (year >= 2026)
    {
        federalRules.lossDeductionLimit = 0.9; // 90% limit starting 2026
    }
    else
    {
        federalRules.lossDeductionLimit = 1.0; // 100% before 2026
    }
}

std::string TaxRulesConfig::getConfigPath(const std::string& filename) const
{
    return configDirectory + "/" + filename;
}

std::map<std::string, std::string> TaxRulesConfig::parseConfigLine(const std::string& line)
{
    std::map<std::string, std::string> result;
    
    size_t equalPos = line.find('=');
    if (equalPos == std::string::npos) return result;
    
    std::string key = trim(line.substr(0, equalPos));
    std::string value = trim(line.substr(equalPos + 1));
    
    if (!key.empty() && !value.empty())
    {
        result[key] = value;
    }
    
    return result;
}

std::string TaxRulesConfig::trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool TaxRulesConfig::parseBool(const std::string& value)
{
    std::string lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return (lower == "true" || lower == "yes" || lower == "1");
}

double TaxRulesConfig::parseDouble(const std::string& value)
{
    try
    {
        return std::stod(value);
    }
    catch (const std::exception&)
    {
        return 0.0;
    }
}