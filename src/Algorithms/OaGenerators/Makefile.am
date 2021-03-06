# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.
#
# Author:  Andreas Waechter           IBM    2006-04-13

########################################################################
#                            liboainterface                            #
########################################################################

# Name of the library in this directory
noinst_LTLIBRARIES = libbonoagenerators.la

# List all source files, including headers
libbonoagenerators_la_SOURCES = \
	BonDummyHeuristic.cpp \
	BonOaNlpOptim.cpp \
	BonOACutGenerator2.cpp \
	BonOaFeasChecker.cpp \
	BonOaDecBase.cpp \
	BonEcpCuts.cpp \
	BonFpForMinlp.cpp \
	BonOAMessages.cpp

# Here list all include flags, relative to this "srcdir" directory.
AM_CPPFLAGS = \
	-I$(srcdir)/../../Interfaces \
	-I$(srcdir)/../../CbcBonmin \
	-I$(srcdir)/.. \
	$(BONMINLIB_CFLAGS)

########################################################################
#                Headers that need to be installed                     #
########################################################################

# Here list all the header files that are required by a user of the library,
# and that therefore should be installed in 'include/coin-or'
includecoindir = $(includedir)/coin-or
includecoin_HEADERS = \
	BonDummyHeuristic.hpp \
	BonOaNlpOptim.hpp \
	BonOACutGenerator2.hpp \
	BonOaFeasChecker.hpp \
	BonOaDecBase.hpp \
	BonEcpCuts.hpp \
	BonOAMessages.hpp

########################################################################
#                            Astyle stuff                              #
########################################################################

# Here repeat all source files, with "bak" appended
ASTYLE_FILES = \
	BonDummyHeuristic.cppbak BonDummyHeuristic.hppbak \
	BonOaNlpOptim.cppbak BonOaNlpOptim.hppbak \
	BonOACutGenerator2.cppbak BonOACutGenerator2.hppbak \
	BonOaFeasChecker.cppbak BonOaFeasChecker.hppbak \
	BonOaDecBase.cppbak BonOaDecBase.hppbak \
	BonEcpCuts.cppbak BonEcpCuts.hppbak \
	BonOAMessages.cppbak BonOAMessages.hppbak

ASTYLE = @ASTYLE@
ASTYLEFLAGS = @ASTYLEFLAGS@

CLEANFILES = $(ASTYLE_FILES)

SUFFIXES = .cppbak .hppbak

astyle: $(ASTYLE_FILES)

.hpp.hppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@

.cpp.cppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@
