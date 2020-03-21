# <GNUmakefile>  -*- make -*-

define help_message

To build Sparta, perform the following actions depending on purpose:

   Optimized (NDEBUG, optimized):
      mkdir release; cd release
      CC=$(CC) CXX=$(CXX) cmake .. -DCMAKE_BUILD_TYPE=Release

   Debug (unoptimized):
      mkdir debug; cd debug
      CC=$(CC) CXX=$(CXX) cmake .. -DCMAKE_BUILD_TYPE=Debug

* Note: The values of CC and CXX can be changed to clang and clang++ if not already:
      CC=clang CXX=clang++ <etc>

endef
export help_message

#
# If a developer uses Sparta as an ExternalProject (via cmake), the
# invocation of cmake might be at the root level of Sparta.  If the
# user is using 'make', Cmake will create a Makefile and not clobber
# this file.  If we detect the user has a local Makefile, fire it off.
#
.DEFAULT all::
	@if [ -e Makefile ]; then $(MAKE) -f Makefile $@; else echo "$$help_message"; fi
