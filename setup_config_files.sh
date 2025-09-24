#!/bin/bash

echo "🔧 Setting up Dynamic Tax Rules Configuration Files..."
echo "====================================================="

# Create config directory
echo "📁 Creating config directory..."
mkdir -p config

# Create federal_rules.cfg
echo "📝 Creating config/federal_rules.cfg..."
cat > config/federal_rules.cfg << 'EOF'
# Federal Gambling Tax Rules Configuration
# Format: key = value
# Lines starting with # are comments

[GENERAL]
tax_year = 2024
standard_deduction_single = 14600
standard_deduction_married = 29200
itemization_threshold = 1000

[LOSS_DEDUCTIONS]
# Can gambling losses offset winnings?
allows_loss_deduction = true

# Percentage of losses that can be deducted (1.0 = 100%)
# Starting 2026: 0.9 (90%) per "One Big Beautiful Bill"
loss_deduction_limit = 1.0

[WITHHOLDING_THRESHOLDS]
# Amounts that trigger automatic tax withholding
Lottery = 5000
Sweepstakes = 5000
Slot_Machine = 1200
Bingo = 1200
Keno = 1200
Poker_Tournament = 5000
Horse_Racing = 600
Dog_Racing = 600

[NOTES]
# Federal rule changes
recent_changes = "2026: Loss deductions limited to 90% of losses"
EOF

# Create state_rules.cfg with comprehensive rules
echo "📝 Creating config/state_rules.cfg..."
cat > config/state_rules.cfg << 'EOF'
# State Gambling Tax Rules Configuration
# Format: [STATE_CODE] followed by key = value pairs

# STATES THAT DON'T ALLOW LOSS DEDUCTIONS
[CT]
state_name = Connecticut
has_income_tax = true
tax_rate = 0.06
allows_loss_deduction = false
loss_deduction_percentage = 0.0
special_rules = Does not allow gambling losses to offset winnings
requires_nonresident_return = true
withholding_threshold = 5000

[IL]
state_name = Illinois
has_income_tax = true
tax_rate = 0.05
allows_loss_deduction = false
loss_deduction_percentage = 0.0
special_rules = Does not allow gambling losses to offset winnings
requires_nonresident_return = true
withholding_threshold = 5000

[OH]
state_name = Ohio
has_income_tax = true
tax_rate = 0.04
allows_loss_deduction = false
loss_deduction_percentage = 0.0
special_rules = Does not allow gambling losses to offset winnings
requires_nonresident_return = true
withholding_threshold = 5000

# STATES WITH NO INCOME TAX
[FL]
state_name = Florida
has_income_tax = false
tax_rate = 0.0
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = No state income tax
requires_nonresident_return = false
withholding_threshold = 0

[NV]
state_name = Nevada
has_income_tax = false
tax_rate = 0.0
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = No state income tax
requires_nonresident_return = false
withholding_threshold = 0

[TX]
state_name = Texas
has_income_tax = false
tax_rate = 0.0
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = No state income tax
requires_nonresident_return = false
withholding_threshold = 0

# MAJOR GAMBLING STATES
[NJ]
state_name = New Jersey
has_income_tax = true
tax_rate = 0.08875
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = Allows loss deductions, top marginal rate
requires_nonresident_return = true
withholding_threshold = 5000

[NY]
state_name = New York
has_income_tax = true
tax_rate = 0.10
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = High tax rate, allows loss deductions (as of 2024)
requires_nonresident_return = true
withholding_threshold = 5000

[PA]
state_name = Pennsylvania
has_income_tax = true
tax_rate = 0.0307
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = Flat tax rate, allows loss deductions
requires_nonresident_return = true
withholding_threshold = 5000

[CA]
state_name = California
has_income_tax = true
tax_rate = 0.13
allows_loss_deduction = true
loss_deduction_percentage = 1.0
special_rules = Highest state tax rate, allows loss deductions
requires_nonresident_return = true
withholding_threshold = 5000

# Easy to add more states without recompiling!
EOF

echo ""
echo "✅ Configuration files created successfully!"
echo ""
echo "📂 Files created:"
echo "   • config/federal_rules.cfg"
echo "   • config/state_rules.cfg"
echo ""
echo "🎯 Dynamic tax rules system ready!"
echo ""
echo "💡 You can now:"
echo "   • Edit these files to update tax rules without recompiling"
echo "   • Add new states by adding [STATE_CODE] sections"
echo "   • Modify existing rules as tax laws change"
echo ""
echo "🚀 Ready to build and test your dynamic tax calculator!"
EOF

chmod +x setup_config_files.sh