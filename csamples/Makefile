SRC_DIR   = $(shell pwd)
SRC_FILES = $(shell find $(SRC_DIR) -name "*.c")
BIN_DIR   = bin
BINS      = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SRC_FILES))
CC        = gcc
LFLAGS    = -lpthread -lrt
CFLAGS    += -Wall
CFLAGS    += $(LFLAGS)

.SILENT:

all: clean makedir $(BINS)
	@echo "******** DONE ********"

clean:
	@rm -rf $(BIN_DIR)

makedir:
	@mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: $(SRC_DIR)/%.c
	@echo "  CC     $< --> $@"
	@$(CC) $< -o $@ $(CFLAGS)
