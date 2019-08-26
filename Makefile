main: src/main.cpp src/Analizer_ACL.cpp
	g++ -o bin/main src/main.cpp src/Analizer_ACL.cpp -I/lib/

clean:
	rm bin/main
