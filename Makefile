metapopulation_ibm: metapopulation_ibm.cpp
	g++ -Wall -g -o metapopulation_ibm metapopulation_ibm.cpp -lgsl -lgslcblas -lm
	
all:	metapopulation_ibm

clean:
	rm -f metapopulation_ibm
