cmake_minimum_required(VERSION 2.8.12)
project(ScintillaEdit)

SET( CMAKE_CXX_STANDARD 11 )
SET( CMAKE_CXX_STANDARD_REQUIRED ON )

  SET(SCINTILLA_SRCS
    ScintillaEdit.cpp
    ScintillaDocument.cpp
    ../ScintillaEditBase/PlatQt.cpp
    ../ScintillaEditBase/ScintillaQt.cpp
    ../ScintillaEditBase/ScintillaEditBase.cpp
    ../../src/XPM.cxx
    ../../src/ViewStyle.cxx
    ../../src/UniConversion.cxx
    ../../src/Style.cxx
    ../../src/Selection.cxx
    ../../src/ScintillaBase.cxx
    ../../src/RunStyles.cxx
    ../../src/RESearch.cxx
    ../../src/PositionCache.cxx
    ../../src/PerLine.cxx
    ../../src/MarginView.cxx
    ../../src/LineMarker.cxx
    ../../src/KeyMap.cxx
    ../../src/Indicator.cxx
    ../../src/ExternalLexer.cxx
    ../../src/EditView.cxx
    ../../src/Editor.cxx
    ../../src/EditModel.cxx
    ../../src/Document.cxx
    ../../src/Decoration.cxx
    ../../src/ContractionState.cxx
    ../../src/CharClassify.cxx
    ../../src/CellBuffer.cxx
    ../../src/Catalogue.cxx
    ../../src/CaseFolder.cxx
    ../../src/CaseConvert.cxx
    ../../src/CallTip.cxx
    ../../src/AutoComplete.cxx
    ../../lexlib/WordList.cxx
    ../../lexlib/StyleContext.cxx
    ../../lexlib/PropSetSimple.cxx
    ../../lexlib/LexerSimple.cxx
    ../../lexlib/LexerNoExceptions.cxx
    ../../lexlib/LexerModule.cxx
    ../../lexlib/LexerBase.cxx
    ../../lexlib/CharacterSet.cxx
    ../../lexlib/CharacterCategory.cxx
    ../../lexlib/Accessor.cxx
    ../../lexers/LexA68k.cxx
    ../../lexers/LexAbaqus.cxx
    ../../lexers/LexAda.cxx
    ../../lexers/LexAPDL.cxx
    ../../lexers/LexAsm.cxx
    ../../lexers/LexAsn1.cxx
    ../../lexers/LexASY.cxx
    ../../lexers/LexAU3.cxx
    ../../lexers/LexAVE.cxx
    ../../lexers/LexAVS.cxx
    ../../lexers/LexBaan.cxx
    ../../lexers/LexBash.cxx
    ../../lexers/LexBasic.cxx
    ../../lexers/LexBatch.cxx
    ../../lexers/LexBibTeX.cxx
    ../../lexers/LexBullant.cxx
    ../../lexers/LexCaml.cxx
    ../../lexers/LexCLW.cxx
    ../../lexers/LexCmake.cxx
    ../../lexers/LexCOBOL.cxx
    ../../lexers/LexCoffeeScript.cxx
    ../../lexers/LexConf.cxx
    ../../lexers/LexCPP.cxx
    ../../lexers/LexCrontab.cxx
    ../../lexers/LexCsound.cxx
    ../../lexers/LexCSS.cxx
    ../../lexers/LexD.cxx
    ../../lexers/LexDiff.cxx
    ../../lexers/LexDMAP.cxx
    ../../lexers/LexDMIS.cxx
    ../../lexers/LexECL.cxx
    ../../lexers/LexEDIFACT.cxx
    ../../lexers/LexEiffel.cxx
    ../../lexers/LexErlang.cxx
    ../../lexers/LexErrorList.cxx
    ../../lexers/LexEScript.cxx
    ../../lexers/LexFlagship.cxx
    ../../lexers/LexForth.cxx
    ../../lexers/LexFortran.cxx
    ../../lexers/LexGAP.cxx
    ../../lexers/LexGui4Cli.cxx
    ../../lexers/LexHaskell.cxx
    ../../lexers/LexHex.cxx
    ../../lexers/LexHTML.cxx
    ../../lexers/LexIndent.cxx
    ../../lexers/LexInno.cxx
    ../../lexers/LexJSON.cxx
    ../../lexers/LexKix.cxx
    ../../lexers/LexKVIrc.cxx
    ../../lexers/LexLaTeX.cxx
    ../../lexers/LexLisp.cxx
    ../../lexers/LexLout.cxx
    ../../lexers/LexLua.cxx
    ../../lexers/LexMagik.cxx
    ../../lexers/LexMake.cxx
    ../../lexers/LexMarkdown.cxx
    ../../lexers/LexMatlab.cxx
    ../../lexers/LexMetapost.cxx
    ../../lexers/LexMMIXAL.cxx
    ../../lexers/LexModula.cxx
    ../../lexers/LexMPT.cxx
    ../../lexers/LexMSSQL.cxx
    ../../lexers/LexMySQL.cxx
    ../../lexers/LexNimrod.cxx
    ../../lexers/LexNsis.cxx
    ../../lexers/LexNull.cxx
    ../../lexers/LexOpal.cxx
    ../../lexers/LexOScript.cxx
    ../../lexers/LexPascal.cxx
    ../../lexers/LexPB.cxx
    ../../lexers/LexPerl.cxx
    ../../lexers/LexPLM.cxx
    ../../lexers/LexPO.cxx
    ../../lexers/LexPOV.cxx
    ../../lexers/LexPowerPro.cxx
    ../../lexers/LexPowerShell.cxx
    ../../lexers/LexProgress.cxx
    ../../lexers/LexProps.cxx
    ../../lexers/LexPS.cxx
    ../../lexers/LexPython.cxx
    ../../lexers/LexR.cxx
    ../../lexers/LexRebol.cxx
    ../../lexers/LexRegistry.cxx
    ../../lexers/LexRuby.cxx
    ../../lexers/LexRust.cxx
    ../../lexers/LexScriptol.cxx
    ../../lexers/LexSmalltalk.cxx
    ../../lexers/LexSML.cxx
    ../../lexers/LexSorcus.cxx
    ../../lexers/LexSpecman.cxx
    ../../lexers/LexSpice.cxx
    ../../lexers/LexSQL.cxx
    ../../lexers/LexSTTXT.cxx
    ../../lexers/LexTACL.cxx
    ../../lexers/LexTADS3.cxx
    ../../lexers/LexTAL.cxx
    ../../lexers/LexTCL.cxx
    ../../lexers/LexTCMD.cxx
    ../../lexers/LexTeX.cxx
    ../../lexers/LexTxt2tags.cxx
    ../../lexers/LexVB.cxx
    ../../lexers/LexVerilog.cxx
    ../../lexers/LexVHDL.cxx
    ../../lexers/LexVisualProlog.cxx
    ../../lexers/LexYAML.cxx
  )

  
  SET( SCINTILLA_MOC_HDRS
    ScintillaEdit.h
    ScintillaDocument.h
    ../ScintillaEditBase/ScintillaEditBase.h
    ../ScintillaEditBase/ScintillaQt.h
  )

  INCLUDE_DIRECTORIES( "../ScintillaEditBase" )
  INCLUDE_DIRECTORIES( "../../include" )
  INCLUDE_DIRECTORIES( "../../src" )
  INCLUDE_DIRECTORIES( "../../lexlib" )
  INCLUDE_DIRECTORIES( "${CMAKE_CURRENT_BINARY_DIR}" )
  INCLUDE_DIRECTORIES( "." )
  INCLUDE_DIRECTORIES( "/usr/local/include" )

  add_definitions( -DSCINTILLA_QT=1 -DMAKING_LIBRARY=1 -DSCI_LEXER=1 -D_CRT_SECURE_NO_DEPRECATE=1 -DSTATIC_BUILD=1)


if (BUILD_QT5)

  FIND_PACKAGE( Qt5Widgets 5.2.1 REQUIRED )
  FIND_PACKAGE( Qt5Core REQUIRED )
  FIND_PACKAGE( Qt5Concurrent REQUIRED )

  set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  
  SET(CMAKE_AUTOMOC ON)
  SET(CMAKE_INCLUDE_CURRENT_DIR ON)

  INCLUDE_DIRECTORIES( "${Qt5Widgets_INCLUDE_DIRS}" )

  add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} )
  target_link_libraries( scintillaedit Qt5::Widgets Qt5::Concurrent )

else (BUILD_QT5)
 
  SET( QT_DONT_USE_QTGUI FALSE )
  
  FIND_PACKAGE( Qt4 REQUIRED )
  
  INCLUDE( "${QT_USE_FILE}" )

  QT4_WRAP_CPP( SCINTILLA_MOC_SRCS ${SCINTILLA_MOC_HDRS} )


  add_library( scintillaedit STATIC ${SCINTILLA_SRCS} ${SCINTILLA_MOC_SRCS} )
  target_link_libraries( scintillaedit ${QT_LIBRARIES} )

endif (BUILD_QT5)

