#**
#	Makefile
#	panomaticSURF is the SURF lib from panomatic (http://aorlinsk2.free.fr/panomatic/)
#	
#	Author : Matthieu DANIEL
#	
#**
EXECUTABLE	:= panomatic-surf

# Exception for Mac OSX build
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
ARCHFLAGS 	:=-arch i386 -arch x86_64
endif

# Basic directory setup 
SRCDIR = ./panomaticsurf

#compilers
CXX	= g++ -O0

# Warning flags
CXXWARN_FLAGS := -W -Wall -Wno-write-strings

run: 
	@echo ""
	@echo "########################################################################"
	@echo "# 	Build and execute PanomaticSURF		 		      #"
	@echo "########################################################################"
	@echo ""
	$(CXX) $(CXXWARN_FLAGS) $(SRCDIR)/*.cpp -o $(EXECUTABLE) $(ARCHFLAGS)
	./$(EXECUTABLE)
