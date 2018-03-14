.PHONY: make
configuration=`pkg-config --static --libs glfw3` -lglfw3 -lkazmath
make:
	clang $(wildcard vendor/*.c source/*.c) -o out $(configuration) -O2 -g
