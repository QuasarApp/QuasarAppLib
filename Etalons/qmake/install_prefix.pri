!isEmpty(INSTALL_REFIX_PRI_INCLUDED):error("install_prefix.pri already included")
INSTALL_REFIX_PRI_INCLUDED = 1

unix:libfiletype=*.so*
win32:libfiletype=*.dll
unix:runfiletype=*
win32:runfiletype=*.exe


isEmpty(PREFIX) {
    message(install target not complited if you want added install rules use PREFIX)
} else {
    message(install target  (use target_dir): $$PREFIX)

    install_data.path = $$PREFIX
#    target_dir.CONFIG += no_check_exist
    INSTALLS += install_data
}








