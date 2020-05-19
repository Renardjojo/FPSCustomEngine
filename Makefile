#Bin
OUTPUT=./bin/exe

#Include path
IDIR=-Iinclude

#exclude from clean
EXCLUDE= src/glad.o

#Relase or debug option
CXX_DEBUG=-Og -std=gnu++17 -g -pg -no-pie -MMD -Wno-unknown-pragmas $(IDIR)
CXX_DEBUGERROR=-Og -std=gnu++17  -g -pg -no-pie -MMD -W -Wall -Werror -Wno-unknown-pragmas $(IDIR)

CXX_EDITOR=-O3 -std=gnu++17 -DNDEBUG -Wno-unknown-pragmas $(IDIR)
CXX_BUILD=-O3 -std=gnu++17 -DNDEBUG -DNEDITOR -Wno-unknown-pragmas $(IDIR)

C_DEBUG=-Og -g -pg -no-pie -MMD -Wno-unknown-pragmas $(IDIR)
C_DEBUGERROR=-Og -g -pg -no-pie -MMD -W -Wall -Werror -Wno-unknown-pragmas $(IDIR)

C_EDITOR=-O3 -DNDEBUG -Wno-unknown-pragmas $(IDIR)
C_BUILD=-O3 -DNDEBUG -DNEDITOR -Wno-unknown-pragmas $(IDIR)

#Valgrind flag
#VFLAG=--leak-check=yes
VFLAG=--leak-check=full --show-leak-kinds=all

#Lib
LIBSGL= -ldl
LIBSDL2=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx
LDLIBS= $(LIBSDL2) $(LIBSGL)

#Cpp and C wildcard
LIBIMGUI = $(wildcard include/imgui/*.cpp)
SRCPPS=$(wildcard src/*.cpp) 
SRCS=$(wildcard src/*.c) 
OBJS=$(SRCPPS:.cpp=.o)  $(SRCS:.c=.o) $(LIBIMGUI:.cpp=.o)

all: $(OUTPUT)

multi :
	mkdir -p bin
	make -j all

-include $(OBJS:.o=.d)

%.o: %.cpp
	g++ -c $(CXX_DEBUG) $< -o $@

%.o: %.c
	gcc -c $(C_DEBUG) $< -o $@

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
	rm -f $(OBJS) $(OBJS:.o=.d) $(OUTPUT)

#SRC_FILES = $(filter-out src/bar.cpp, $(wildcard src/*.cpp))
clean :
	rm -f $(filter-out $(EXCLUDE) $(EXCLUDE:.o=.d),$(OBJS:.o=.d) $(OBJS))

re : clean all

med : 
	find ./Ressources -name "*" > ./Ressources.txt
	g++ -Wall -Werror mediaCreator.cpp -o mediaCreator && ./mediaCreator
	rm ./Ressources.txt && rm ./mediaCreator
