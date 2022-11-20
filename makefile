spellChecker: spellChecker.cpp
	g++ -g spellChecker.cpp -o spellChecker
  
clean:
	rm spellChecker

run: spellChecker
	./spellChecker

debug: spellChecker
	gdb spellChecker
