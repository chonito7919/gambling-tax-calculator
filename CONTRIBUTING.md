# Contributing to Gambling Tax Calculator

Thank you for your interest in contributing! This document provides guidelines for contributing to this project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Commit Guidelines](#commit-guidelines)
- [Pull Request Process](#pull-request-process)

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## Getting Started

1. Fork the repository
2. Clone your fork: `git clone https://github.com/yourusername/gambling-tax-calculator.git`
3. Add upstream remote: `git remote add upstream https://github.com/original/gambling-tax-calculator.git`
4. Create a feature branch: `git checkout -b feature/your-feature-name`

## Development Setup

### Prerequisites

- CMake 3.10+
- C++17 compatible compiler (g++ 7+ or clang 6+)
- For Windows builds: MinGW-w64

### Building for Development

**Linux:**
```bash
mkdir -p build-linux
cd build-linux
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

**Windows (cross-compile):**
```bash
mkdir -p build-windows
cd build-windows
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows.cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

### Running Tests

```bash
cd build-linux
./gambling-calc
```

## How to Contribute

### Reporting Bugs

When reporting bugs, please include:
- Operating system and version
- Compiler version
- Steps to reproduce
- Expected vs actual behavior
- Relevant configuration files (sanitize personal data)

### Suggesting Features

Feature requests are welcome! Please:
- Check existing issues first
- Describe the use case clearly
- Explain why this feature benefits users
- Consider backwards compatibility

### Contributing Code

Areas where contributions are especially welcome:
- Tax rule updates (state/federal changes)
- Additional state support improvements
- Bug fixes
- Documentation improvements
- Test coverage
- Performance optimizations
- UI/UX enhancements

## Coding Standards

### C++ Style Guidelines

- **Standard**: C++17
- **Naming Conventions**:
  - Classes: `PascalCase` (e.g., `TaxCalculator`)
  - Functions/Methods: `camelCase` (e.g., `calculateTaxes`)
  - Variables: `camelCase` (e.g., `totalWinnings`)
  - Constants: `UPPER_CASE` (e.g., `MAX_SESSIONS`)
  - Private members: prefix with underscore optional
- **Indentation**: 4 spaces (no tabs)
- **Line length**: 100 characters max (flexible for readability)
- **Headers**: Use `#pragma once` for header guards
- **Includes**: Group by standard library, third-party, then local

### Code Quality

- Write self-documenting code with clear variable names
- Add comments for complex logic only
- Use const correctness
- Prefer RAII for resource management
- Avoid raw pointers; use smart pointers when needed
- Handle errors appropriately (exceptions or error codes)

### Example:

```cpp
class TaxCalculator {
private:
    TaxRulesConfig taxRules;
    bool professionalGambler;

public:
    TaxCalculator(bool isProfessional = false, const std::string& configDir = "config");

    TaxSummary calculateTaxes(const std::vector<GamblingSession>& sessions) const;

    void setProfessionalMode(bool isProfessional) {
        professionalGambler = isProfessional;
    }
};
```

## Configuration File Guidelines

When updating tax rules in `config/` files:

1. **Verify sources**: Use official IRS or state tax authority publications
2. **Document changes**: Add comments with source and effective date
3. **Maintain format**: Follow existing `.cfg` file structure
4. **Test thoroughly**: Ensure calculations remain accurate
5. **Update documentation**: Reflect changes in README or comments

Example:
```ini
# Updated 2025-01-15, Source: IRS Publication 505
[WITHHOLDING_THRESHOLDS]
Lottery = 5000  # Increased from $600, effective 2025
```

## Commit Guidelines

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style/formatting (no functional changes)
- `refactor`: Code refactoring
- `test`: Adding/updating tests
- `chore`: Build process, tooling, dependencies

**Examples:**
```
feat(tax): Add 2026 federal rule changes for 90% loss deduction

Implements the "One Big Beautiful Bill" rule limiting loss deductions
to 90% starting in tax year 2026.

Closes #42
```

```
fix(config): Correct Nevada tax rate in state_rules.cfg

Nevada has no state income tax. Updated tax_rate from 0.05 to 0.0.

Fixes #38
```

## Pull Request Process

1. **Update your fork**:
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Ensure all builds pass**:
   ```bash
   # Test Linux build
   mkdir -p build-linux && cd build-linux
   cmake .. && make

   # Test Windows build
   mkdir -p build-windows && cd build-windows
   cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows.cmake .. && make
   ```

3. **Create Pull Request**:
   - Use a clear, descriptive title
   - Reference related issues
   - Describe changes and rationale
   - Include testing performed
   - Update documentation if needed

4. **Code Review**:
   - Address reviewer feedback promptly
   - Keep discussions professional and constructive
   - Be open to suggestions

5. **Merge Requirements**:
   - At least one approving review
   - All CI checks passing
   - No merge conflicts
   - Documentation updated

## Tax Rule Contributions

When contributing tax rule updates:

1. **Cite sources**: Include IRS publication, state revenue website, or legal statute
2. **Effective dates**: Clearly indicate when rules take effect
3. **Backwards compatibility**: Consider supporting historical rules
4. **Accuracy**: Double-check all percentages, thresholds, and rates
5. **Testing**: Verify calculations with known test cases

### State Rule Template

```ini
[STATE_CODE]
state_name = Full State Name
has_income_tax = true/false
tax_rate = 0.0XX  # Top marginal rate
allows_loss_deduction = true/false
loss_deduction_percentage = 1.0  # 1.0 = 100%
special_rules = Brief description
requires_nonresident_return = true/false
withholding_threshold = 5000
```

## Questions?

- Open an issue for general questions
- Tag maintainers for urgent matters
- Check existing issues and discussions first

Thank you for contributing to make gambling tax calculations more accessible and accurate!

---

**Remember**: This project is GPL-3.0 licensed. All contributions will be under the same license.