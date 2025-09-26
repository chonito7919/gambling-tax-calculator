# C++ Style Guide for Gambling Tax Calculator

This document outlines the coding style conventions for the Gambling Tax Calculator project. All contributors must follow these guidelines to ensure consistent, readable, and maintainable code.

## Brace Style

### Allman Brace Style (Required)
All opening braces must be placed on their own line, aligned with the statement that precedes them.

#### ✅ Correct Examples:

**Functions:**
```cpp
void calculateTaxes()
{
    // function body
}

int main()
{
    return 0;
}
```

**Classes:**
```cpp
class TaxCalculator
{
private:
    double totalWinnings;

public:
    TaxCalculator();
    void calculate();
};
```

**Control Structures:**
```cpp
if (condition)
{
    // if body
}
else if (otherCondition)
{
    // else if body
}
else
{
    // else body
}

while (condition)
{
    // loop body
}

for (int i = 0; i < count; i++)
{
    // loop body
}

switch (value)
{
    case 1:
        doSomething();
        break;
    case 2:
        doSomethingElse();
        break;
    default:
        handleDefault();
        break;
}
```

**Namespaces:**
```cpp
namespace GamblingTax
{
    class Calculator
    {
        // class definition
    };
}
```

**Try-Catch Blocks:**
```cpp
try
{
    riskyOperation();
}
catch (const std::exception& e)
{
    handleException(e);
}
```

#### ❌ Incorrect Examples:

```cpp
// DO NOT do this - opening brace on same line
void calculateTaxes() {
    // function body
}

// DO NOT do this - inconsistent brace placement
if (condition) {
    // if body
}
else {
    // else body
}

class TaxCalculator {  // Wrong - brace should be on new line
private:
    double totalWinnings;
};
```

## Additional Style Guidelines

### Indentation
- Use 4 spaces for indentation (no tabs)
- Indent content within braces one level

### Naming Conventions
- **Classes**: PascalCase (e.g., `TaxCalculator`, `GamblingSession`)
- **Functions/Methods**: camelCase (e.g., `calculateTaxes()`, `getNetResult()`)
- **Variables**: camelCase (e.g., `totalWinnings`, `buyIn`)
- **Constants**: UPPER_SNAKE_CASE (e.g., `MAX_SESSIONS`, `DEFAULT_TAX_RATE`)
- **Private Members**: camelCase with no special prefix

### File Organization
- Header files: `.h` extension
- Implementation files: `.cpp` extension
- Include guards: Use `#pragma once`
- Include order: Standard library, third-party, project headers

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments when necessary
- Document public API functions with clear descriptions
- Avoid obvious comments (e.g., `i++; // increment i`)

### Example of Well-Formatted Code

```cpp
#pragma once
#include <vector>
#include <string>

class GamblingSession
{
private:
    std::string date;
    double buyIn;
    double cashOut;

public:
    GamblingSession(const std::string& sessionDate, double buy, double cash);

    double getNetResult() const
    {
        return cashOut - buyIn;
    }

    bool isWin() const
    {
        return getNetResult() > 0;
    }

    void processSession()
    {
        if (isWin())
        {
            std::cout << "You won!" << std::endl;
        }
        else
        {
            std::cout << "Better luck next time." << std::endl;
        }
    }
};
```

## Enforcement

### Pre-commit Requirements
- All code must follow the Allman brace style
- Code that doesn't conform will be rejected in code reviews
- Use consistent indentation (4 spaces)

### Tools and IDE Settings
Configure your IDE/editor to:
- Use 4 spaces instead of tabs
- Show whitespace characters
- Automatically format braces according to Allman style
- Enable bracket matching and highlighting

### Code Reviews
All pull requests will be reviewed for style compliance. Common issues to watch for:
- Opening braces on the same line as statements
- Inconsistent indentation
- Mixed tabs and spaces
- Improper naming conventions

## Rationale

The Allman brace style was chosen for this project because:
1. **Clarity**: Opening braces clearly delineate code blocks
2. **Consistency**: Provides uniform visual structure
3. **Readability**: Makes it easier to match opening and closing braces
4. **Debugging**: Clearer stack traces and easier to set breakpoints
5. **Team Preference**: Established convention for this project

## Exceptions

The following are the only acceptable exceptions to the brace style:
1. **Empty functions/methods**: May use `{}` on the same line for simple getters/setters
   ```cpp
   int getValue() const { return value; }
   void setValue(int v) { value = v; }
   ```

2. **Single-line conditionals**: Only if the entire statement fits on one readable line
   ```cpp
   if (isValid) { return true; }  // Acceptable for very simple cases
   ```

However, when in doubt, always use the full Allman style format.

---

**Remember**: Consistent code style makes our codebase easier to read, maintain, and collaborate on. When contributing to this project, please ensure your code follows these guidelines before submitting any pull requests.