#------------------------
# Do not modify this file
#------------------------

all: iml.cpp trader.cpp
	g++-11 -std=c++20 iml.cpp -lpthread -o iml
	g++-11 -std=c++20 trader.cpp -lpthread -o trader
	./trader 2 > output.txt &
	sleep 2
	./iml 2

clean:
	rm -rf trader iml output.txt
	rm -rf *~
