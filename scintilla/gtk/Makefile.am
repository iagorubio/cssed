noinst_LIBRARIES=libscintilla.a

CC=g++

AM_CXXFLAGS = -DGTK -DGTK2 -DSCI_LEXER

LEXER_SRCS= \
../src/LexYAML.cxx \
../src/LexVHDL.cxx \
../src/LexVerilog.cxx \
../src/LexVB.cxx \
../src/LexTeX.cxx \
../src/LexTADS3.cxx \
../src/LexSQL.cxx \
../src/LexSpecman.cxx \
../src/LexSmalltalk.cxx \
../src/LexScriptol.cxx \
../src/LexRuby.cxx \
../src/LexRebol.cxx \
../src/LexPython.cxx \
../src/LexPS.cxx \
../src/LexPOV.cxx \
../src/LexPerl.cxx \
../src/LexPB.cxx \
../src/LexPascal.cxx \
../src/LexOthers.cxx \
../src/LexNsis.cxx \
../src/LexMSSQL.cxx \
../src/LexMPT.cxx \
../src/LexMMIXAL.cxx \
../src/LexMetapost.cxx \
../src/LexMatlab.cxx \
../src/LexLua.cxx \
../src/LexLout.cxx \
../src/LexLisp.cxx \
../src/LexKix.cxx \
../src/LexHTML.cxx \
../src/LexHaskell.cxx \
../src/LexGui4Cli.cxx \
../src/LexFortran.cxx \
../src/LexForth.cxx \
../src/LexFlagship.cxx \
../src/LexEScript.cxx \
../src/LexErlang.cxx \
../src/LexEiffel.cxx \
../src/LexCSS.cxx \
../src/LexCsound.cxx \
../src/LexCrontab.cxx \
../src/LexCPP.cxx \
../src/LexConf.cxx \
../src/LexCLW.cxx \
../src/LexCaml.cxx \
../src/LexBullant.cxx \
../src/LexBasic.cxx \
../src/LexBash.cxx \
../src/LexBaan.cxx \
../src/LexAVE.cxx \
../src/LexAU3.cxx \
../src/LexAsn1.cxx \
../src/LexAsm.cxx \
../src/LexAPDL.cxx \
../src/LexAda.cxx \
../src/LexInno.cxx \
../src/LexOpal.cxx \
../src/LexSpice.cxx \
../src/LexTCL.cxx \
../src/LexCmake.cxx \
../src/LexD.cxx \
../src/LexGAP.cxx \
../src/LexPLM.cxx  \
../src/LexProgress.cxx   \
../src/LexAbaqus.cxx  \
../src/LexASY.cxx  \
../src/LexR.cxx 

SRCS= \
../src/AutoComplete.cxx \
../src/CallTip.cxx \
../src/CellBuffer.cxx \
../src/CharClassify.cxx \
../src/ContractionState.cxx \
../src/Document.cxx \
../src/DocumentAccessor.cxx \
../src/Editor.cxx \
../src/ExternalLexer.cxx \
../src/Indicator.cxx \
../src/KeyMap.cxx \
../src/KeyWords.cxx \
../src/LineMarker.cxx \
../src/PositionCache.cxx \
../src/PropSet.cxx \
../src/RESearch.cxx \
../src/ScintillaBase.cxx \
../src/Style.cxx \
../src/StyleContext.cxx \
../src/UniConversion.cxx \
../src/ViewStyle.cxx \
../src/WindowAccessor.cxx \
../src/XPM.cxx \
../src/Decoration.cxx \
../src/RunStyles.cxx \
../src/*.h \
PlatGTK.cxx \
ScintillaGTK.cxx \
$(LEXER_SRCS)

libscintilla_a_SOURCES = $(SRCS) 

INCLUDES = -I$(top_srcdir) \
	-I$(top_srcdir)/scintilla/include \
	-I$(top_srcdir)/scintilla/gtk \
	-I$(top_srcdir)/scintilla/src \
	@PACKAGE_CFLAGS@

libscintilla_a_LIBADD = scintilla-marshal.o

scintilla-marshal.o: scintilla-marshal.h
	gcc @PACKAGE_CFLAGS@ -c scintilla-marshal.c

marshallers: scintilla-marshal.list
	glib-genmarshal --prefix scintilla_marshal $(srcdir)/scintilla-marshal.list --header > scintilla-marshal.h
	glib-genmarshal --prefix scintilla_marshal $(srcdir)/scintilla-marshal.list --body > scintilla-marshal.c 
	
EXTRA_DIST=scintilla-marshal.list scintilla-marshal.c scintilla-marshal.h
