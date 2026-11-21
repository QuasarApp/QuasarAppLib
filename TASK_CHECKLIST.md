# Task Checklist - Issue #76: Move all main sources to src folder

## Status
- [x] Create src folder structure with logical subfolders
- [x] Move all .cpp and .h files to appropriate subfolders
- [x] Update CMakeLists.txt to include files from new locations
- [x] Update header includes to point to new locations
- [x] Verify build still works (structure verified)
- [ ] Run tests
- [ ] Commit changes to task_76 branch
- [ ] Push to origin
- [ ] Create pull request

## Planned Folder Structure
```
src/
├── Core/           # Core functionality (quasarapp, qaservice, qaglobalutils)
├── Params/         # Command line arguments parsing (params)
├── Settings/       # Settings management (settings, isettings, settingslistener)
├── Locales/        # Translation management (locales)
├── HelpData/       # Help printing (helpdata)
├── OptionData/     # Command line option definition (optiondata)
├── Utils/          # Utility classes (qalogger, qaplatformutils, qasecretservice, validableobject, humanreadableobject)
└── quasarapp.h     # Main header (stays at root level for public API)
```

## Files to move:
### Core/
- quasarapp.h, quasarapp.cpp
- qaservice.h, qaservice.cpp  
- quasarapp_global.h

### Params/
- params.h, params.cpp

### Settings/
- settings.h, settings.cpp
- isettings.h, isettings.cpp
- settingslistener.h, settingslistener.cpp

### Locales/
- locales.h, locales.cpp

### HelpData/
- helpdata.h, helpdata.cpp

### OptionData/
- optiondata.h, optiondata.cpp

### Utils/
- qalogger.h, qalogger.cpp
- qaplatformutils.h, qaplatformutils_linux.cpp
- qasecretservice.h, qasecretservice.cpp
- validableobject.h, validableobject.cpp
- humanreadableobject.h, humanreadableobject.cpp

### Files staying in src root:
- crc32constexper.h
- qaglobalutils.h, qaglobalutils.cpp