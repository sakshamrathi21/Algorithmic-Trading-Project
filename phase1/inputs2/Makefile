# Define the source files and output files
SOURCES = input1.txt input2.txt input3.txt input4.txt input5.txt input6.txt input7.txt input8.txt input9.txt input10.txt input11.txt input12.txt input13.txt
OUTPUTS = output1.txt output2.txt output3.txt output4.txt output5.txt output6.txt output7.txt output8.txt output9.txt output10.txt output11.txt output12.txt

# SOURCES = input17.txt input18.txt
# OUTPUTS = output17.txt output18.txt
# Define the common compile and run commands
CXX = g++
CXXFLAGS = -std=c++20
TRADER = ../trader
IML = ../iml
SLEEP = sleep 1
DIFF = diff -bw

# Define the rules
all: $(OUTPUTS)

# Rule to compile the C++ code
$(IML): ../iml.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Rule to generate an output file for each input file
output%.txt: input%.txt $(IML) $(TRADER)
	cat $< > ../inputs/data2.txt
	$(CXX) $(CXXFLAGS) $(IML).cpp -o $(IML)
	$(TRADER) 2 > ../output.txt &
	$(SLEEP)
	$(IML) ../inputs/data2.txt
	$(SLEEP)
	$(DIFF) ../output.txt ../outputs2/$@

# Clean up temporary files
clean:
	rm -f $(IML) ../output.txt

.PHONY: all clean
