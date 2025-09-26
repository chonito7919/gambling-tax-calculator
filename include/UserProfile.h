#pragma once
#include <string>
#include <map>

enum class FilingStatus
{
    SINGLE,
    MARRIED_FILING_JOINTLY,
    MARRIED_FILING_SEPARATELY,
    HEAD_OF_HOUSEHOLD
};

class UserProfile
{
private:
    std::string configDirectory;
    std::string homeState;
    std::string timezone;
    FilingStatus filingStatus;
    bool profileExists;

    std::map<std::string, std::string> timezoneOptions;
    std::map<std::string, std::string> stateOptions;

    void initializeOptions();
    std::string filingStatusToString(FilingStatus status) const;
    FilingStatus stringToFilingStatus(const std::string& status) const;

public:
    UserProfile(const std::string& configDir = "config");

    // Profile management
    bool loadProfile();
    bool saveProfile();
    bool hasProfile() const
    {
        return profileExists;
    }

    // Setup wizard
    void runSetupWizard();

    // Getters
    std::string getHomeState() const
    {
        return homeState;
    }
    std::string getTimezone() const
    {
        return timezone;
    }
    FilingStatus getFilingStatus() const
    {
        return filingStatus;
    }
    std::string getFilingStatusString() const
    {
        return filingStatusToString(filingStatus);
    }

    // Setters
    void setHomeState(const std::string& state)
    {
        homeState = state;
    }
    void setTimezone(const std::string& tz)
    {
        timezone = tz;
    }
    void setFilingStatus(FilingStatus status)
    {
        filingStatus = status;
    }

    // Utility functions
    std::string getConfigFilePath() const;
    void displayCurrentProfile() const;
};