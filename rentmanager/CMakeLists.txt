cmake_minimum_required(VERSION 2.8.7)
project(rentmanager)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}
../xlslib
${Qt4Gui_PRIVATE_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set(ui_files rentmanagermainwindow.ui  
    companyinitializationdialog.ui  
    companiesdialog.ui  
    propertiesdialog.ui  
    unitsdialog.ui  
    tenantsdialog.ui  
    assignunittotenantdialog.ui  
    terminateoccupationdialog.ui  
    receivepaymentdialog.ui  
    paymentsdialog.ui  
    createinvoicedialog.ui
    additemtoinvoicedialog.ui
    createmultiinvoice.ui
    smartpaymentdialog.ui
    companyfilepassword.ui
    customeraccountdialog.ui
    depositdialog.ui
    tenantaccountrebuilddialog.ui
    terminateleasedialog.ui
)

set(qrc_files res.qrc)


set(h_files
    rentmanagermainwindow.h
    publics.h
    companyinitializationdialog.h
    companiesdialog.h
    propertiesdialog.h
    unitsdialog.h
    tenantsdialog.h
    assignunittotenantdialog.h
    terminateoccupationdialog.h
    receivepaymentdialog.h
    paymentsdialog.h
    createinvoicedialog.h
    additemtoinvoicedialog.h
    createmultiinvoice.h
    smartpaymentdialog.h
    companyfilepassword.h
    simplecrypt.h
    customeraccountdialog.h
    depositdialog.h
    tenantaccountrebuilddialog.h
    terminateleasedialog.h
)

set(cpp_files main.cpp
    rentmanagermainwindow.cpp
    publics.cpp
    companyinitializationdialog.cpp
    companiesdialog.cpp
    propertiesdialog.cpp
    unitsdialog.cpp
    tenantsdialog.cpp
    assignunittotenantdialog.cpp
    terminateoccupationdialog.cpp
    receivepaymentdialog.cpp
    paymentsdialog.cpp
    createinvoicedialog.cpp
    additemtoinvoicedialog.cpp
    createmultiinvoice.cpp
    smartpaymentdialog.cpp
    companyfilepassword.cpp
    simplecrypt.cpp
    customeraccountdialog.cpp
    depositdialog.cpp
    tenantaccountrebuilddialog.cpp
    terminateleasedialog.cpp
)



if(MINGW)
    #MinGW resource compilation
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/windows.o
    COMMAND i686-w64-mingw32-windres -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/Windows.rc
            -o ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
    set(rc ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
    ADD_DEFINITIONS( -D__USE_MINGW_ANSI_STDIO=1 )
else(MINGW)
    #set(rc windows.rc)
endif(MINGW)

if(QT5)
qt5_wrap_ui(uis ${ui_files})
qt5_add_resources(qrcs ${qrc_files})
set(moc )
else()
qt4_wrap_ui(uis ${ui_files})
qt4_add_resources(qrcs ${qrc_files})
qt4_wrap_cpp(moc ${h_files})
endif()

set(rentManagerSources ${rc} ${uis} ${qrcs} ${cpp_files} ${moc})

if (APPLE)
      set (ExecutableName RentManager )
      set (MACOSX_BUNDLE_ICON_FILE      cottage.icns)
      set (MACOSX_BUNDLE_GUI_IDENTIFIER org.smartsoftware.rentman)
      set (MACOSX_BUNDLE_LONG_VERSION_STRING ${VERSION})
      set (MACOSX_BUNDLE_BUNDLE_NAME RentManager)
      set (MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
      set (MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})
      set (MACOSX_BUNDLE_COPYRIGHT smartsoftware.co.ke)
else (APPLE)
      set (ExecutableName rentmanager)
endif (APPLE)

if(MINGW)
    set(ExecutableName RentManager)
endif(MINGW)

add_executable(${ExecutableName} WIN32 MACOSX_BUNDLE ${rentManagerSources})

IF( APPLE )
    SET( RentManagerResourceDIR ${CMAKE_BINARY_DIR}/bin/${ExecutableName}.app/Contents/Resources )
    message(${RentManagerResourceDIR})
    ADD_CUSTOM_COMMAND( TARGET ${ExecutableName} POST_BUILD
        COMMAND mkdir ARGS -p ${RentManagerResourceDIR}
        COMMAND cp ARGS -f ${CMAKE_CURRENT_SOURCE_DIR}/cottage.icns ${RentManagerResourceDIR} )
ENDIF( APPLE )

target_link_libraries(${ExecutableName} xlslib ${libs})

if(APPLE)
    set(plistPath ${CMAKE_CURRENT_SOURCE_DIR}/rentman.plist.in)
      set_target_properties (${ExecutableName}
          PROPERTIES
          MACOSX_BUNDLE_INFO_PLIST ${plistPath})
endif(APPLE)
