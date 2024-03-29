# module load gnu/gcc/10.2.0

# Turn on debug info
CXXFLAGS = -g -std=c++17
LDFLAGS = -lpthread

executables = listing2_9 listing3_5 listing3_8 listing3_13 listing4_1 listing4_7 listing4_10 shared_future quick_sort detach
all : $(executables)

$(executables) : % : %.cpp

.PHONY: clean
clean:
	rm $(executables)
