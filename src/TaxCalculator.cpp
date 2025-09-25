#include "../include/TaxCalculator.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

TaxCalculator::TaxCalculator(bool isProfessional, const std::string& configDir)
    : taxRules(configDir), professionalGambler(isProfessional)
{
}

TaxSummary TaxCalculator::calculateTaxes(const std::vector<GamblingSession>& sessions) const
{
    TaxSummary summary = {};
    summary.taxYear = taxRules.getFederalRules().taxYear;
    summary.rulesVersion = "Dynamic Config v1.0";
    
    calculateFederalTotals(sessions, summary);
    calculateStateTotals(sessions, summary);
    generateReminders(sessions, summary);
    
    return summary;
}

void TaxCalculator::calculateFederalTotals(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const
{
    summary.totalWinnings = 0.0;
    summary.totalLosses = 0.0;
    summary.totalWithheld = 0.0;
    
    for (const auto& session : sessions)
    {
        double netResult = session.getNetResult();
        
        if (netResult > 0)
        {
            summary.totalWinnings += netResult;
        }
        else if (netResult < 0)
        {
            summary.totalLosses += std::abs(netResult);
        }
        
        summary.totalWithheld += session.getWithheldAmount();
    }
    
    // Federal rules: Apply loss deduction limit (e.g., 90% starting 2026)
    const FederalTaxRules& federalRules = taxRules.getFederalRules();
    double maxDeductibleLosses = std::min(summary.totalLosses, summary.totalWinnings);
    summary.deductibleLosses = maxDeductibleLosses * federalRules.lossDeductionLimit;
    
    summary.netFederalResult = summary.totalWinnings - summary.totalLosses;
    summary.federalTaxableIncome = summary.totalWinnings; // Winnings always taxable
    
    summary.hasWinnings = summary.totalWinnings > 0;
    summary.hasDeductibleLosses = summary.deductibleLosses > 0;
    
    // Recommend itemizing if losses are significant
    summary.itemizingRecommended = summary.deductibleLosses >= federalRules.itemizationThreshold;
}

void TaxCalculator::calculateStateTotals(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const
{
    // First, calculate raw winnings and losses per state
    for (const auto& session : sessions)
    {
        std::string state = session.getState();
        double netResult = session.getNetResult();
        
        if (netResult > 0)
        {
            summary.stateWinnings[state] += netResult;
        }
        else if (netResult < 0)
        {
            summary.stateLosses[state] += std::abs(netResult);
        }
    }
    
    // Now apply state-specific rules
    for (const auto& stateWinning : summary.stateWinnings)
    {
        std::string state = stateWinning.first;
        double winnings = stateWinning.second;
        double losses = summary.stateLosses[state];
        
        // Get state-specific loss deduction rules
        double lossDeductionPercentage = taxRules.getLossDeductionPercentage(state);
        bool allowsLossDeduction = taxRules.allowsLossDeduction(state);
        
        if (allowsLossDeduction && lossDeductionPercentage > 0.0)
        {
            // Calculate deductible losses based on state percentage
            double maxDeductibleLosses = std::min(losses, winnings);
            summary.stateDeductibleLosses[state] = maxDeductibleLosses * lossDeductionPercentage;
            summary.stateNetResults[state] = winnings - summary.stateDeductibleLosses[state];
        }
        else
        {
            // State doesn't allow loss deductions - full winnings are taxable
            summary.stateDeductibleLosses[state] = 0.0;
            summary.stateNetResults[state] = winnings;
        }
    }
}

void TaxCalculator::generateReminders(const std::vector<GamblingSession>& sessions, TaxSummary& summary) const
{
    std::set<std::string> addedReminders; // Avoid duplicates
    
    if (summary.hasWinnings)
    {
        summary.documentationReminders.push_back("📄 Keep all winning tickets, receipts, and payment records");
    }
    
    if (summary.hasDeductibleLosses)
    {
        summary.documentationReminders.push_back("📄 Keep all losing tickets and receipts for deduction proof");
        summary.documentationReminders.push_back("📝 Maintain detailed gambling diary with dates, locations, and amounts");
    }
    
    if (summary.totalWithheld > 0)
    {
        summary.documentationReminders.push_back("📋 Keep all W-2G forms from gambling establishments");
    }
    
    // Check for states that don't allow loss deductions or have special rules
    for (const auto& session : sessions)
    {
        std::string state = session.getState();
        const StateTaxRule* stateRule = taxRules.getStateRule(state);
        
        if (stateRule && session.isWin())
        {
            if (!stateRule->allowsLossDeduction)
            {
                std::string reminder = "⚠️  " + state + " does not allow gambling losses to offset winnings";
                if (addedReminders.find(reminder) == addedReminders.end())
                {
                    summary.documentationReminders.push_back(reminder);
                    addedReminders.insert(reminder);
                }
            }
            else if (stateRule->lossDeductionPercentage < 1.0)
            {
                std::ostringstream oss;
                oss << "⚠️  " << state << " only allows " 
                    << std::fixed << std::setprecision(0) 
                    << (stateRule->lossDeductionPercentage * 100) << "% of losses to be deducted";
                std::string reminder = oss.str();
                if (addedReminders.find(reminder) == addedReminders.end())
                {
                    summary.documentationReminders.push_back(reminder);
                    addedReminders.insert(reminder);
                }
            }
        }
    }
    
    // Check for withholding thresholds using dynamic rules
    for (const auto& session : sessions)
    {
        if (triggersWithholding(session.getGameType(), session.getNetResult()) && !session.getTaxWithheld())
        {
            summary.documentationReminders.push_back("⚠️  Some winnings may have required withholding - check with establishment");
            break; // Only show this once
        }
    }
    
    // Add federal rule changes reminder
    const FederalTaxRules& federalRules = taxRules.getFederalRules();
    if (federalRules.lossDeductionLimit < 1.0)
    {
        std::ostringstream oss;
        oss << "📢 Federal rule: Loss deductions limited to " 
            << std::fixed << std::setprecision(0) 
            << (federalRules.lossDeductionLimit * 100) << "% for tax year " << federalRules.taxYear;
        summary.documentationReminders.push_back(oss.str());
    }
}

std::string TaxCalculator::generateTaxReport(const TaxSummary& summary) const
{
    std::ostringstream report;
    report << std::fixed << std::setprecision(2);
    
    report << "=== GAMBLING TAX SUMMARY ===\n";
    report << "Tax Year: " << summary.taxYear << " | Rules: " << summary.rulesVersion << "\n\n";
    
    // Federal Summary
    report << "FEDERAL TAX IMPLICATIONS:\n";
    report << "Total Winnings: $" << summary.totalWinnings << "\n";
    report << "Total Losses: $" << summary.totalLosses << "\n";
    report << "Deductible Losses: $" << summary.deductibleLosses;
    
    const FederalTaxRules& federalRules = taxRules.getFederalRules();
    if (federalRules.lossDeductionLimit < 1.0)
    {
        report << " (limited to " << (federalRules.lossDeductionLimit * 100) << "% of qualified losses)";
    }
    else if (summary.deductibleLosses < summary.totalLosses)
    {
        report << " (limited to winnings amount)";
    }
    report << "\n";
    report << "Net Result: $" << summary.netFederalResult << "\n";
    
    if (summary.totalWithheld > 0)
    {
        report << "Total Tax Withheld: $" << summary.totalWithheld << "\n";
    }
    
    report << "\nFEDERAL TAX FILING:\n";
    report << "• Report winnings as 'Other Income' on Form 1040\n";
    if (summary.hasDeductibleLosses)
    {
        report << "• Deduct losses on Schedule A (itemized deductions)\n";
        if (summary.itemizingRecommended)
        {
            report << "• ✅ Itemizing is likely beneficial with $" << summary.deductibleLosses << " in losses\n";
        }
    }
    
    // State Summary with dynamic rules
    if (!summary.stateWinnings.empty())
    {
        report << "\nSTATE TAX IMPLICATIONS:\n";
        for (const auto& stateResult : summary.stateNetResults)
        {
            std::string state = stateResult.first;
            double netResult = stateResult.second;
            const StateTaxRule* stateRule = taxRules.getStateRule(state);
            
            report << state << ": ";
            if (!stateRule || !stateRule->hasIncomeTax)
            {
                report << "No state income tax";
            }
            else
            {
                report << "Taxable amount: $" << netResult;
                
                // Show loss deduction info
                auto deductibleIt = summary.stateDeductibleLosses.find(state);
                if (deductibleIt != summary.stateDeductibleLosses.end() && deductibleIt->second > 0)
                {
                    double lossPercentage = taxRules.getLossDeductionPercentage(state);
                    if (lossPercentage < 1.0)
                    {
                        report << " (only " << (lossPercentage * 100) << "% of losses deductible)";
                    }
                }
                else if (stateRule && !stateRule->allowsLossDeduction)
                {
                    report << " (losses not deductible)";
                }
            }
            report << "\n";
            
            if (stateRule && !stateRule->specialRules.empty())
            {
                report << "  Note: " << stateRule->specialRules << "\n";
            }
        }
    }
    
    return report.str();
}

std::string TaxCalculator::generateDocumentationChecklist() const
{
    std::ostringstream checklist;
    
    checklist << "=== DOCUMENTATION CHECKLIST ===\n\n";
    checklist << "Keep these records for IRS audit protection:\n\n";
    checklist << "📋 WINNING RECORDS:\n";
    checklist << "  • Original winning tickets/receipts\n";
    checklist << "  • W-2G forms from casinos/lottery\n";
    checklist << "  • Bank deposit records\n";
    checklist << "  • Photos of winning tickets (backup)\n\n";
    
    checklist << "📋 LOSING RECORDS:\n";
    checklist << "  • All losing tickets and receipts\n";
    checklist << "  • ATM withdrawal receipts at gambling venues\n";
    checklist << "  • Credit card statements showing gambling purchases\n\n";
    
    checklist << "📋 GAMBLING DIARY:\n";
    checklist << "  • Date and time of each session\n";
    checklist << "  • Location/establishment name\n";
    checklist << "  • Type of gambling activity\n";
    checklist << "  • Amount wagered and won/lost\n";
    checklist << "  • Names of witnesses (if applicable)\n\n";
    
    checklist << "💡 TIP: Store physical documents in a dedicated folder\n";
    checklist << "💡 TIP: Take photos as digital backup\n";
    checklist << "💡 TIP: Keep records for at least 3 years after filing\n";
    
    return checklist.str();
}

std::string TaxCalculator::generateRulesReport() const
{
    std::ostringstream report;
    
    report << "=== CURRENT TAX RULES ===\n\n";
    
    const FederalTaxRules& federalRules = taxRules.getFederalRules();
    report << "FEDERAL RULES (Tax Year " << federalRules.taxYear << "):\n";
    report << "• Loss Deduction Limit: " << std::fixed << std::setprecision(0) 
           << (federalRules.lossDeductionLimit * 100) << "%\n";
    report << "• Standard Deduction: $" << std::fixed << std::setprecision(0) 
           << federalRules.standardDeduction << "\n";
    report << "• Itemization Threshold: $" << std::fixed << std::setprecision(0) 
           << federalRules.itemizationThreshold << "\n\n";
    
    report << "WITHHOLDING THRESHOLDS:\n";
    for (const auto& threshold : federalRules.withholdingThresholds)
    {
        report << "• " << threshold.first << ": $" << std::fixed << std::setprecision(0) 
               << threshold.second << "\n";
    }
    
    report << "\nSTATE RULES LOADED:\n";
    std::vector<std::string> states = taxRules.getAvailableStates();
    for (const std::string& stateInfo : states)
    {
        report << "• " << stateInfo << "\n";
    }
    
    return report.str();
}

bool TaxCalculator::triggersWithholding(const std::string& gameType, double winnings) const
{
    if (winnings <= 0) return false;
    
    double threshold = getWithholdingThreshold(gameType);
    return threshold > 0 && winnings >= threshold;
}

double TaxCalculator::getWithholdingThreshold(const std::string& gameType) const
{
    const FederalTaxRules& federalRules = taxRules.getFederalRules();
    
    auto it = federalRules.withholdingThresholds.find(gameType);
    if (it != federalRules.withholdingThresholds.end())
    {
        return it->second;
    }
    
    // Try alternative names
    if (gameType == "Lottery" || gameType == "Sweepstakes")
    {
        auto lotIt = federalRules.withholdingThresholds.find("Lottery");
        return (lotIt != federalRules.withholdingThresholds.end()) ? lotIt->second : 5000.0;
    }
    
    return 0.0; // No threshold found
}

void TaxCalculator::setTaxYear(int year)
{
    taxRules.updateForTaxYear(year);
}

int TaxCalculator::getTaxYear() const
{
    return taxRules.getFederalRules().taxYear;
}

double TaxCalculator::calculateStateTax(const std::string& stateCode, double winnings, double losses) const
{
    const StateTaxRule* rule = taxRules.getStateRule(stateCode);
    if (!rule || !rule->hasIncomeTax)
    {
        return 0.0; // No state income tax
    }
    
    double taxableAmount = winnings;
    if (rule->allowsLossDeduction && rule->lossDeductionPercentage > 0.0)
    {
        double deductibleLosses = std::min(losses, winnings) * rule->lossDeductionPercentage;
        taxableAmount = winnings - deductibleLosses;
    }
    
    return taxableAmount * rule->taxRate;
}