NAME := modem
ARCH := x64
ID := debug

OUTPUT := $(NAME)-$(ARCH)-$(ID)

build :
	mkdir -p bin/
	clang++ src/*.cpp -I include/ -I ../common/include -L ../common/bin -l commonx64debug -l portaudio -o bin/$(OUTPUT)

clean :	
	rm -rf bin/* || true