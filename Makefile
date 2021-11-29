build:
	javac com/lib/Add.java
	native-image --shared -H:Name=libadd
	CGO_ENABLE=1 go build -o hello hello.go

clean:
	rm -rf graal* 
	rm -rf lib*
	rm -rf com/lib/*.class
	rm -rf hello

.PHONY: buildlib build clean