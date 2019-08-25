test_read: test_read.cpp MCP3002_Driver.cpp
	g++ -o test_read test_read.cpp MCP3002_Driver.cpp -I.

clean:
	rm test_read
