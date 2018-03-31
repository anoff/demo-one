build:
	g++ *.cpp -lsdl2 -o demo.out
debug:
	g++ *.cpp -lsdl2 -o demo.out -g
run:
	exec ./demo.out
