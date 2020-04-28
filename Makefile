#Bin
OUTPUT=./bin/exe

#Include path
DIRI=include
DIRIG=$(DIRI)/Game
DIRIGE=$(DIRI)/GE
DIRICORE=$(DIRIGE)/Core
DIRIRESS=$(DIRIGE)/Resources
DIRIPHYSICS=$(DIRIGE)/Physics
DIRILR=$(DIRIGE)/LowRenderer
DIRISDK=$(DIRIGE)/SDK
IDIRCOR=-I$(DIRICORE)/Maths -I$(DIRICORE)/Debug -I$(DIRICORE)/Parsers -I$(DIRICORE)/Rendering/Renderer -I$(DIRICORE)/Rendering/Window -I$(DIRICORE)/DataStructure -I$(DIRICORE)/InputSystem -I$(DIRICORE)/TimeSystem
IDIRSDK=-I$(DIRISDK)/glad -I$(DIRISDK)/KHR
IDIRLR=-I$(DIRILR)/Light
IDIR=-I$(DIRI) -I$(DIRIGE) -I$(DIRICORE) -I$(DIRIPHYSICS) -I$(DIRILR) -I$(DIRIG) -I$(DIRISDK) -I$(DIRIRESS) $(IDIRCOR) $(IDIRSDK) $(IDIRLR)
#IDIR=$(-I,$(shell find $(DIRI)) -name "*.h") $(shell find $(DIRI) -name "*.hpp")

#Relase or debug option
CXXFLAGSDEBUG=-Og -g -pg -no-pie -MMD -Wall -Werror -Wno-unknown-pragmas $(IDIR)
CXXFLAGSRELEASE=-O3 -DNDEBUG -W -Wno-unknown-pragmas $(IDIR)

#Valgrind flag
#VFLAG=--leak-check=yes
VFLAG=--leak-check=full --show-leak-kinds=all

#Lib
LIBSGL=-lGL -lGLU -ldl
LIBSDL2=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx
LDLIBS= $(LIBSDL2) $(LIBSGL)

#Cpp and C wildcard
SRCPPS=$(wildcard src/*.cpp)
SRCS=$(wildcard src/*.c)
OBJS=$(SRCPPS:.cpp=.o)  $(SRCS:.c=.o) 

all: $(OUTPUT)

multi :
	mkdir -p bin
	make -j all

-include $(OBJS:.o=.d)

%.o: %.cpp
	g++ -c $(CXXFLAGSRELEASE) $< -o $@

%.o: %.c
	gcc -c $(CXXFLAGSRELEASE) $< -o $@

$(OUTPUT): $(OBJS)
	mkdir -p bin
	g++ -pg -no-pie $^ $(LDLIBS) -o $@

run : $(OUTPUT) 
	./$(OUTPUT)

#display function stats
gprof :
	gprof $(OUTPUT) gmon.out > output.txt

#display leak
leak : 
	valgrind $(OUTPUT)

#display leak detail
leakFull : 
	valgrind $(VFLAG) $(OUTPUT)

#function to find word in project
foundPrintf:
	grep -r $(OBJS) "printf"

#TODO Function that create launcher
launcher :
	touch game.desktop
	echo "[Desktop Entry]" >> game.desktop
	echo "Encoding=UTF-8">> game.desktop
	echo "Version=1.0                                     # version of an app.">> game.desktop
	echo "Name[en_US]=game                                # name of an app." >> game.desktop
	echo "GenericName=game                                # longer name of an app." >> game.desktop
	echo "Exec=./bin/game                                 # command used to launch an app." >> game.desktop
	echo "Terminal=false                                  # whether an app requires to be run in a terminal." >> game.desktop
	echo "Icon[en_US]=/opt/yed-3.11.1/icons/yicon32.png   # location of icon file." >> game.desktop
	echo "Type=Application                                # type." >> game.desktop
	echo "Categories=game;                                # categories in which this app should be listed." >> game.desktop
	echo "Comment[en_US]=Game                             # comment which appears as a tooltip. " >> game.desktop

cleanAll:
	rm -rf $(OBJS) $(OBJS:.o=.d) $(OUTPUT)

clean :
	rm -rf $(OBJS) $(OBJS:.o=.d)

re : clean all

med : 
	find ./Resources -name "*" > ./Resources.txt
	g++ -Wall -Werror mediaCreator.cpp -o mediaCreator && ./mediaCreator
	rm ./Resources.txt && rm ./mediaCreator
