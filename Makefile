CC = gcc
CFLAGS = -O2 -Wall -Iinclude
AR = ar
ARFLAGS = rcs

SRC = src/matrix.c src/matrix_ops.c src/matrix_utils.c src/LU.c src/QR.c src/eigen.c
OBJ = $(SRC:.c=.o)

LIB = libmlalc.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install:
	mkdir -p /usr/local/include/mlalc
	cp include/*.h /usr/local/include/mlalc
	cp $(LIB) /usr/local/lib
	ldconfig

clean:
	rm -f $(OBJ) $(LIB)
