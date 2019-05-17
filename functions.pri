################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################

# Copied and modified from qt_functions.prf

defineReplace(libTarget) {

    unset(LIBRARY_NAME)
    LIBRARY_NAME = $$1
    #
    !win32-msvc*:LIBRARY_NAME ~= s,^lib,,

    CONFIG(debug, debug|release) {

        !debug_and_release|build_pass {

            mac:RET = $$member(LIBRARY_NAME, 0)_debug
            win32:RET = $$member(LIBRARY_NAME, 0)_d
        }
    }

    isEmpty(RET):RET = $$LIBRARY_NAME
    return($$RET)
}

defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

defineReplace(targetPath) {
    return($$shell_path($$1))
}

defineReplace(platformSpecialPath){
    BASE_DIR = $$1
    PLATFORM_INCLUDE_PATH = $$BASE_DIR/$$basename(QMAKESPEC)/$$QT_ARCH
    !exists($$PLATFORM_INCLUDE_PATH){
        error("$${PLATFORM_INCLUDE_PATH} not found , need use origin lib configure.")
    }
    return($$PLATFORM_INCLUDE_PATH)
}
