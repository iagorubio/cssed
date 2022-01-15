noinst_LIBRARIES=libcroco.a

if DEBUG
AM_CFLAGS = -g @PACKAGE_CFLAGS@
else
AM_CFLAGS = @PACKAGE_CFLAGS@
endif

#the core parser files
SRCS=\
cr-additional-sel.c \
cr-declaration.c \
cr-fonts.c \
cr-om-parser.c \
cr-prop-list.c \
cr-selector.c \
cr-statement.c \
cr-stylesheet.c \
cr-token.c \
cr-attr-sel.c \
cr-doc-handler.c \
cr-input.c \
cr-parser.c \
cr-pseudo.c \
cr-sel-eng.c \
cr-string.c \
cr-term.c \
cr-utils.c \
cr-cascade.c \
cr-enc-handler.c \
cr-num.c \
cr-parsing-location.c \
cr-rgb.c \
cr-simple-sel.c \
cr-style.c  \
cr-tknzr.c \
*.h


libcroco_a_SOURCES= $(SRCS)

INCLUDES=-I$(top_srcdir) -I$(top_srcdir)/intl
