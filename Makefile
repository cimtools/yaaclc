main: src/main.cpp src/Analizer_ACL.cpp
	g++ -g src/main.cpp src/Analizer_ACL.cpp -o bin/main  -I/lib/

clean:
	rm bin/main
