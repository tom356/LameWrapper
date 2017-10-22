CC= g++
CFLAGS = -Wall -c
CLIB_EXT = -lmp3lame
CLIBS = 
CLIB_ALL = $(CLIBS) $(CLIB_EXT) -Llib

#directories
MODE= Release
OBJDIR=$(MODE)
BINDIR=$(MODE)
	
$(BINDIR)/encoder: makeFolders $(OBJDIR)/main.o $(OBJDIR)/InputParser.o $(OBJDIR)/LameWrapper.o
	$(CC) $(OBJDIR)/main.o $(OBJDIR)/InputParser.o $(OBJDIR)/LameWrapper.o $(CLIB_ALL) -o $(BINDIR)/encoder

$(OBJDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(OBJDIR)/main.o

$(OBJDIR)/InputParser.o: InputParser.cpp InputParser.h
	$(CC) $(CFLAGS) InputParser.cpp -o $(OBJDIR)/InputParser.o

$(OBJDIR)/LameWrapper.o: LameWrapper.cpp LameWrapper.h
	$(CC) $(CFLAGS) LameWrapper.cpp -o $(OBJDIR)/LameWrapper.o

makeFolders:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

clean:
	rm $(OBJDIR)/*.o $(BINDIR)/encoder   encoder 