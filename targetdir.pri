
isEmpty( MAIN_PWD ) {
    MAIN_PWD=$$PWD
    warning(you use targetdir but not added MAIN_PWD=\$\$PWD into main pro file)
}

equals( TEMPLATE, app) {

    isEmpty( TARGET_PATH_APP ) {

        DESTDIR = $$MAIN_PWD/build
        message("$$TARGET use default ($$DESTDIR) TARGET PATH. if you want changed path then set TARGET_PATH_APP in main pro file")
    } else {
        DESTDIR = $$TARGET_PATH_APP
    }
}

equals( TEMPLATE, lib) {
    isEmpty( TARGET_PATH_LIB ) {

        DESTDIR = $$MAIN_PWD/build
        message("$$TARGET use default ($$DESTDIR) TARGET PATH. if you want changed path then set TARGET_PATH_LIB in main pro file")
    } else {
        DESTDIR = $$TARGET_PATH_LIB
    }
}
