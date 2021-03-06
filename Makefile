# GNU Make solution makefile autogenerated by Premake
# Type "make help" for usage help

ifndef config
  config=debug
endif
export config

PROJECTS := crossword-editor

.PHONY: all clean help $(PROJECTS)

all: $(PROJECTS)

crossword-editor: 
	@echo "==== Building crossword-editor ($(config)) ===="
	@${MAKE} --no-print-directory -C . -f crossword-editor.make

clean:
	@${MAKE} --no-print-directory -C . -f crossword-editor.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "   debug"
	@echo "   debugnocurses"
	@echo "   release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   crossword-editor"
	@echo ""
	@echo "For more information, see http://industriousone.com/premake/quick-start"
