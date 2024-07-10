IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=./obj

_SERVER_DEPS = manage_users.h manage_books.h manage_transactions.h server_utils.h
SERVER_DEPS = $(patsubst %,$(IDIR)/%,$(_SERVER_DEPS))

_SERVER_OBJ = manage_users.o manage_books.o manage_transactions.o server_utils.o server.o
SERVER_OBJ = $(patsubst %,$(ODIR)/%,$(_SERVER_OBJ))

_CLIENT_DEPS = client_utils.h
CLIENT_DEPS = $(patsubst %,$(IDIR)/%,$(_CLIENT_DEPS))

_CLIENT_OBJ = client_utils.o client.o
CLIENT_OBJ = $(patsubst %,$(ODIR)/%,$(_CLIENT_OBJ))

$(ODIR)/%.o: ./src/%.c $(CLIENT_DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: ./src/%.c $(SERVER_DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

server: $(SERVER_OBJ)
			$(CC) -o $@ $^ $(CFLAGS)

client: $(CLIENT_OBJ)
			$(CC) -o $@ $^ $(CFLAGS)