# module load gnu/gcc/10.2.0

all : listing2_9 listing3_5

listing2_9: chapter2/listing2_9.cpp
	g++ -o listing2_9 -lpthread chapter2/listing2_9.cpp

listing3_5: chapter3/listing3_5.cpp
	g++ -o listing3_5 -lpthread chapter3/listing3_5.cpp

#   helloworld: helloworld.cpp
#       g++ -o helloworld -lpthread helloworld.cpp

#   create_thread_pass_smart_pointer: create_thread_pass_smart_pointer.cpp
#       g++ -o create_thread_pass_smart_pointer -lpthread create_thread_pass_smart_pointer.cpp

.PHONY: clean
clean:
	rm listing2_9 listing3_5