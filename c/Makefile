CFLAGS=-Wall
BUILD_DIR=build
SRC_DIR=src
MAIN=build/main.bin

$(MAIN): $(SRC_DIR)/main.c
	mkdir $(BUILD_DIR) -p
	$(CC) $(CFLAGS) $? -o $@

run: $(MAIN)
	$(MAIN) ./src/IBM_logo.ch8

clean:
	rm -rf $(BUILD_DIR)