# TODO - Gambling Tax Calculator

## High Priority

### Core Functionality
- [ ] Add unit tests for tax calculation logic
- [ ] Implement input validation for all user inputs
- [x] **Add date validation (ensure format MM-DD-YYYY)** - *Function exists but not used in input flow*
- [x] **Implement proper date handling (COMPLETED)** - Full timezone-aware getCurrentDate() implementation
- [ ] Add session editing capability (modify existing sessions)
- [ ] Add session deletion (delete individual sessions)

### Tax Rules & Accuracy
- [ ] Verify all 50 state tax rates with 2024-2025 official sources
- [ ] Implement 2026 federal rule changes (90% loss deduction)
- [ ] Add support for different tax filing statuses (single, married, HOH)
- [ ] Implement AMT (Alternative Minimum Tax) considerations
- [ ] Add local/city gambling tax support (e.g., NYC, Philadelphia)

### Data & Persistence
- [ ] Add JSON export/import option
- [ ] Implement database support (SQLite) for session storage
- [ ] Add backup/restore functionality
- [ ] Support multiple tax years in same database
- [ ] Add session search/filter functionality

## Medium Priority

### User Interface
- [ ] Build Qt GUI version (cross-platform desktop app)
- [ ] Add color-coded output (wins in green, losses in red)
- [ ] Improve menu navigation (breadcrumbs, back option)
- [ ] Add session summary dashboard
- [ ] Create visualization charts (win/loss trends)

### Reporting
- [ ] Generate printable tax report (HTML/PDF)
- [ ] Create Form W-2G pre-fill helper
- [ ] Add Schedule A itemized deduction worksheet
- [ ] Generate gambling diary/log template
- [ ] Create year-end tax summary report

### Documentation
- [ ] Add code documentation (Doxygen)
- [ ] Create user guide/manual
- [ ] Add FAQ document
- [ ] Create video tutorial
- [ ] Add example calculation walkthroughs

### Platform Support
- [ ] Test Windows build on actual Windows systems
- [ ] Add macOS support (native build)
- [ ] Create installer packages (DEB, RPM, MSI)
- [ ] Add to package managers (apt, homebrew, chocolatey)

## Low Priority

### Features
- [ ] Multi-user support (different profiles)
- [ ] Cloud sync capability
- [ ] Mobile app (Android/iOS)
- [ ] Web-based version
- [ ] API for third-party integrations
- [ ] Import from casino player's club statements
- [ ] Import from online gambling platforms

### Advanced Tax Features
- [ ] Support for professional gambler expenses
- [ ] Estimated tax payment calculator
- [ ] Quarterly tax estimation
- [ ] Tax loss harvesting strategies
- [ ] Compare itemizing vs standard deduction scenarios
- [ ] Multi-state apportionment for complex situations

### Integrations
- [ ] Export to TurboTax format
- [ ] Export to H&R Block format
- [ ] Export to TaxAct format
- [ ] Integration with accounting software (QuickBooks)
- [ ] Bank/credit card transaction import

### Code Quality
- [ ] Add comprehensive unit test suite
- [ ] Implement integration tests
- [ ] Set up CI/CD pipeline (GitHub Actions)
- [ ] Add code coverage reporting
- [ ] Implement fuzzing for input validation
- [ ] Static analysis integration (cppcheck, clang-tidy)
- [ ] Memory leak detection (valgrind)

### Performance
- [ ] Optimize config file parsing
- [ ] Add caching for frequently accessed rules
- [ ] Implement lazy loading for large datasets
- [ ] Profile and optimize hot paths

## Ideas / Future Consideration

- [ ] AI-powered tax advice assistant
- [ ] Blockchain-based receipt verification
- [ ] Real-time tax estimation during gambling
- [ ] Integration with casino loyalty programs
- [ ] Automatic W-2G detection from email
- [ ] Voice-based session entry
- [ ] OCR for ticket/receipt scanning
- [ ] Cryptocurrency gambling support
- [ ] International tax support (other countries)
- [ ] Tax treaty considerations for international gamblers

## Bug Fixes

- [ ] Fix stdin handling in getUserChoice() for batch input
- [ ] Validate state codes (reject invalid codes)
- [ ] Handle leap year dates correctly
- [ ] Ensure config files are read with correct encoding
- [ ] Fix potential overflow in loss calculations

## Documentation Needed

- [x] **Style guide (COMPLETED)** - Created comprehensive C++ style guide with Allman bracket requirements
- [ ] Architecture documentation
- [ ] API documentation
- [ ] Configuration file format specification
- [ ] Testing guide
- [ ] Release process documentation
- [ ] Security considerations document

## Code Style & Quality (RECENTLY COMPLETED)

- [x] **Standardized bracket style** - All source files now use consistent Allman bracket style
- [x] **Created STYLE_GUIDE.md** - Comprehensive style guide for contributors
- [x] **Code formatting review** - Verified all existing files follow Allman style conventions

## Release Milestones

### v1.0.0 - Initial Release ✅
- [x] Basic session tracking
- [x] Federal tax calculations
- [x] 50-state support
- [x] Dynamic config loading
- [x] Cross-platform builds

### v1.1.0 - Enhanced Functionality
- [ ] Unit tests
- [ ] Input validation
- [ ] Session editing/deletion
- [ ] Improved reporting

### v2.0.0 - GUI Version
- [ ] Qt-based desktop application
- [ ] Visual charts and graphs
- [ ] PDF report generation
- [ ] Installer packages

### v3.0.0 - Advanced Features
- [ ] Database support
- [ ] Cloud sync
- [ ] Tax software integrations
- [ ] Mobile apps

---

**Last Updated**: 2025-09-26

**Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md) for how to help with these items!
