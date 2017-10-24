CC= g++
CFLAGS = -Wall -c
CLIB_EXT = -lmp3lame
CLIBS = 
CLIB_ALL = $(CLIBS) $(CLIB_EXT) -Llib

#directories
MODE= Release
OBJDIR=$(MODE)
BINDIR=$(MODE)
	
$(BINDIR)/encode: makeFolders $(OBJDIR)/main.o $(OBJDIR)/InputParser.o $(OBJDIR)/LameWrapper.o $(OBJDIR)/Wave.o
	$(CC) $(OBJDIR)/main.o $(OBJDIR)/InputParser.o $(OBJDIR)/LameWrapper.o $(OBJDIR)/Wave.o $(CLIB_ALL) -o $(BINDIR)/encode

$(OBJDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/InputParser.o: InputParser.cpp InputParser.h
	$(CC) $(CFLAGS) InputParser.cpp -o $(OBJDIR)/InputParser.o

$(OBJDIR)/LameWrapper.o: LameWrapper.cpp LameWrapper.h
	$(CC) $(CFLAGS) LameWrapper.cpp -o $(OBJDIR)/LameWrapper.o

$(OBJDIR)/Wave.o: Wave.cpp Wave.h
	$(CC) $(CFLAGS) Wave.cpp -o $(OBJDIR)/Wave.o

makeFolders:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/encoder   encoder 