CC = g++
CFLAGS = -O3 -std=c++14
DEBUGFLAGS = -g

# Build miniray
all:
	if [ -d "render" ]; then \
		echo "render directory already exists."; \
	else \
		echo "creating render directory."; \
		mkdir render; \
	fi
	$(CC) src/main.cpp -o miniray $(CFLAGS)

debug:
	if [ -d "render" ]; then \
		echo "render directory already exists."; \
	else \
		echo "creating render directory."; \
		mkdir render; \
	fi
	$(CC) $(DEBUGFLAGS) src/main.cpp -o miniray

clean:
	rm -f miniray
	rm -rfv render
