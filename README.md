# Gambling Tax Calculator

A comprehensive, cross-platform C++ application for tracking gambling sessions and calculating tax implications for both federal and state taxes across all 50 US states.

## Features

- 📊 **Session Tracking** - Record wins, losses, location, game type, and documentation
- 💰 **Tax Calculation** - Accurate federal and state tax calculations with dynamic rule loading
- 🗺️ **50-State Support** - Complete tax rules for all US states, including:
  - States with no income tax (FL, NV, TX, WA, WY, SD, TN, NH, AK)
  - States that restrict gambling loss deductions (CT, IL, OH, NC)
  - All other states with varying tax rates and rules
- 📋 **Documentation Checklist** - IRS-compliant record-keeping reminders
- 💾 **Data Persistence** - Save/load sessions to CSV files
- ⚙️ **Dynamic Configuration** - Update tax rules without recompiling
- 🔄 **Professional Gambler Mode** - Special treatment for professional gamblers
- 🖥️ **Cross-Platform** - Builds for Linux and Windows

## Quick Start

### Prerequisites

**Linux:**
```bash
sudo apt-get install cmake g++ build-essential
```

**Windows Cross-Compilation (from Linux):**
```bash
sudo apt-get install mingw-w64
```

### Building

**For Linux:**
```bash
mkdir -p build-linux
cd build-linux
cmake ..
make
cd ..
./build-linux/gambling-calc
```

**For Windows (cross-compile from Linux):**
```bash
mkdir -p build-windows
cd build-windows
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows.cmake ..
make
```

The Windows executable will be at `build-windows/gambling-calc.exe`

## Usage

Run the calculator:
```bash
./build-linux/gambling-calc
```

Main menu options:
1. Add Single Gambling Session
2. Bulk Add Losing Tickets (Quick Entry)
3. View All Sessions
4. Calculate Tax Implications
5. Show Documentation Checklist
6. Save Sessions to File
7. Load Sessions from File
8. View Tax Rules & Configuration
9. Toggle Professional Gambler Mode
10. Clear All Sessions

## Configuration

Tax rules are stored in editable configuration files:

- `config/federal_rules.cfg` - Federal tax rules, withholding thresholds
- `config/state_rules.cfg` - All 50 state tax rules

**Update tax rules without recompiling** by editing these files!

### Example Federal Rules:
```ini
[GENERAL]
tax_year = 2024
standard_deduction_single = 14600
itemization_threshold = 1000

[WITHHOLDING_THRESHOLDS]
Lottery = 5000
Slot_Machine = 1200
```

### Example State Rules:
```ini
[NJ]
state_name = New Jersey
has_income_tax = true
tax_rate = 0.08875
allows_loss_deduction = true
loss_deduction_percentage = 1.0
```

## Project Structure

```
gambling-tax-calculator/
├── src/                    # Source files
├── include/                # Header files
├── config/                 # Tax rule configuration files
├── build-linux/            # Linux build directory
├── build-windows/          # Windows build directory
├── CMakeLists.txt          # CMake build configuration
├── toolchain-windows.cmake # Windows cross-compilation toolchain
└── README.md              # This file
```

## Tax Calculation Features

### Federal Tax Rules
- Tracks total winnings (always taxable)
- Calculates deductible losses (limited to winnings amount)
- Applies federal withholding thresholds (W-2G reporting)
- Supports 2026 rule changes (90% loss deduction limit)
- Itemization recommendations

### State Tax Rules
- Individual tax rates for all 50 states
- State-specific loss deduction rules
- States with no income tax: FL, NV, TX, WA, WY, SD, TN, NH, AK
- States restricting loss deductions: CT, IL, OH, NC
- Nonresident return requirements

### Professional Gambler Mode
- Schedule C business income treatment
- Business expense deductions
- Self-employment tax considerations

## Documentation & Record Keeping

The calculator provides IRS-compliant documentation reminders:
- Keep all winning tickets/receipts
- Maintain gambling diary/log
- Save W-2G forms
- Record session details (date, location, game type)
- Track all losses with supporting documentation

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development guidelines.

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is for informational purposes only and does not constitute tax advice. Consult a qualified tax professional for personalized tax guidance. Tax laws change frequently; verify all calculations with current IRS and state tax authority publications.

## Support

For issues, questions, or contributions, please open an issue on GitHub.

---

**Built with C++17 | Cross-platform | GPL-3.0 Licensed**