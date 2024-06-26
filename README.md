# QuasarAppLib
Global functions used in applications QuasarApp.
This lib include include next modules:

* Locales - This module use for parse local files. This module allows you to work with translations. 
* Params - This module use for parese app params and create log.
* Settings - This is a module that allows you to work with application settings. 
* Global - This module contains other global functions.


## Build


 * git clone https://github.com/QuasarApp/QuasarAppLib.git
 * git submodule update --init --recursive
 * cd QuasarAppLib
 * cmake -DCMAKE_PREFIX_PATH=Yuor/Qt/Dir/Path .  
 * make -j8
 * make test #(for testing)


### Available build options:

```cmake
option(QA_ALLOW_NOT_SUPPORTED_OPTIONS "Enable for allow any command line options" ON)
option(QA_DISABLE_LOG "Disabled all logs (force sets verbose to 0)" OFF)

```

## Include

#### The cmake build do not required Qt libraries. 
 
 * cd yourRepo
 * git submodule add https://github.com/QuasarApp/QuasarAppLib.git # add the repository of QtBigInt into your repo like submodule
 * git submodule update --init --recursive
 * Include in your CMakeLists.txt file the main CMakeLists.txt file of QuasarAppLib library
  >> add_subdirectory(QuasarAppLib)
 * Rebuild yuor project

```cmake
add_subdirectory(QuasarAppLib)
target_link_libraries(MyBinary PUBLIC QuasarApp)
```

### Note 

By Default QuasarAppLib makes as a static library. If you want to create a shared library just add the BUILD_SHARED_LIBS into your main CMakeLists.txt file.
Example :

```cmake
set(BUILD_SHARED_LIBS ON)
add_subdirectory(QuasarAppLib)
target_link_libraries(MyBinary PUBLIC QuasarApp)

```

# Usage

```cpp
#include <quasarapp.h>

if (!QuasarAppUtils::Params::parseParams(argc, argv)) {
    QuasarAppUtils::Params::log("Warning message", QuasarAppUtils::Warning);
    QuasarAppUtils::Params::showHelp();
    exit(0);
}
```
