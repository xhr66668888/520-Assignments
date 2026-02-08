#Compilers
CC          := g++ -std=c++17
DGEN        := doxygen

#The Target Binary Program
TARGET      := test

#The Directories, Source, Includes, Objects, Binary and Resources
SRCDIR      := .
INCDIR      := .
BUILDDIR    := ./build
TARGETDIR   := ./bin
SRCEXT      := c

HOMEBREW_PREFIX := $(shell brew --prefix)

# Flags, Libraries and Includes
CFLAGS      := # -fsanitize=address -ggdb
LIB         := -L$(HOMEBREW_PREFIX)/lib -lgtest -lgtest_main -lpthread 
INC         := -I$(INCDIR) -I$(HOMEBREW_PREFIX)/include
INCDEP      := -I$(INCDIR)

#Files
DGENCONFIG  := docs.config
HEADERS     := $(wildcard *.h)
SOURCES     := $(wildcard *.c)
OBJECTS     := $(patsubst %.c, $(BUILDDIR)/%.o, $(notdir $(SOURCES)))

#Default Make
all: directories $(TARGETDIR)/$(TARGET) 

#Remake
remake: clean all

#Make the Directories
directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

# Make the documentation
$(DGENCONFIG):
	$(DGEN) -g $(DGENCONFIG)

docs: $(SOURCES) $(HEADERS) $(DGENCONFIG)
	$(DGEN) $(DGENCONFIG)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)/*.o

#Full Clean, Objects and Binaries
spotless: clean
	@$(RM) -rf $(TARGETDIR)/$(TARGET) $(DGENCONFIG) *.db
	@$(RM) -rf build bin html latex

#Link
$(TARGETDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

.PHONY: directories remake clean spotless docs apidocs