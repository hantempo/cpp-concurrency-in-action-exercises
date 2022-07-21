# module load gnu/gcc/10.2.0

# Turn on debug info
CXXFLAGS = -g
LDFLAGS = -lpthread

executables = listing2_9 listing3_5
all : $(executables)

$(executables) : % : %.cpp

.PHONY: clean
clean:
	rm $(executables)
