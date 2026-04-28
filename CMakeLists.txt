cmake_minimum_required(VERSION 3.16)
project(FinanceTracker VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core Widgets Sql Charts
)

add_executable(FinanceTracker
    src/main.cpp
    src/models/Transaction.cpp
    src/models/Account.cpp
    src/models/Budget.cpp
    src/database/DatabaseManager.cpp
    src/ui/MainWindow.cpp
    src/ui/AddTransactionDialog.cpp
    src/ui/DashboardWidget.cpp
    src/utils/CSVExporter.cpp
)

target_include_directories(FinanceTracker PRIVATE src)

target_link_libraries(FinanceTracker
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Charts
)

# ── Install rules ────────────────────────────────────────────────────────────
include(GNUInstallDirs)

install(TARGETS FinanceTracker
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES resources/icons/FinanceTracer1.png
    DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps
    RENAME FinanceTracker.png
)

install(FILES resources/FinanceTracker.desktop
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
)

# ── CPack – .deb installer ────────────────────────────────────────────────────
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "FinanceTracker")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_RELEASE 1)
set(CPACK_PACKAGE_VENDOR "NazmulHossain9")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Personal Finance Tracker")
set(CPACK_PACKAGE_DESCRIPTION "A Qt6 desktop application for tracking personal finances,\nmanaging budgets, accounts, and transactions.")
set(CPACK_PACKAGE_CONTACT "nazmul@glascutr.com")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/NazmulHossain9/FinanceTracker")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nazmul Hossain <nazmul@glascutr.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS
    "libqt6core6 | libqt6core6t64, libqt6widgets6 | libqt6widgets6t64, libqt6sql6 | libqt6sql6t64, libqt6charts6 | libqt6charts6t64"
)
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_DEBIAN_FILE_NAME "FinanceTracker-${PROJECT_VERSION}-amd64.deb")

# Run gtk-update-icon-cache after install so the icon shows immediately
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_SOURCE_DIR}/packaging/postinst"
    "${CMAKE_SOURCE_DIR}/packaging/postrm"
)

include(CPack)

# ── Optional unit-test target ─────────────────────────────────────────────────
find_package(Qt6 QUIET COMPONENTS Test)
if(Qt6Test_FOUND)
    add_executable(FinanceTrackerTests
        tests/test_transaction.cpp
        src/models/Transaction.cpp
        src/models/Budget.cpp
    )
    target_include_directories(FinanceTrackerTests PRIVATE src)
    target_link_libraries(FinanceTrackerTests
        Qt6::Core
        Qt6::Test
    )
    enable_testing()
    add_test(NAME UnitTests COMMAND FinanceTrackerTests)
endif()
