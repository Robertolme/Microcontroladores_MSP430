# -------------------------------
# Configuración de herramientas
# -------------------------------
CC = /home/foxi/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430
LD = $(CC)
OBJCOPY = /home/foxi/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/hex430
MSPDEBUG = mspdebug tilib

# -------------------------------
# Configuración de proyecto
# -------------------------------
TARGET = main
OUT = $(TARGET).out
HEX = $(TARGET).hex
MAP = $(TARGET).map
LNK_CMD = /home/foxi/ti/ccs2002/ccs/ccs_base/msp430/include/lnk_msp430g2553.cmd

# -------------------------------
# Archivos y directorios
# -------------------------------
SRC = $(TARGET).c
OBJ = $(TARGET).obj
INCLUDES = -I/home/foxi/ti/ccs2002/ccs/ccs_base/msp430/include \
           -I/home/foxi/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include
LIBDIR = -I/home/foxi/ti/ccs2002/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib

# -------------------------------
# Opciones de compilación y enlace
# -------------------------------
CFLAGS = -vmsp --use_hw_mpy=none --define=__MSP430G2553__ -g \
         --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number $(INCLUDES)
LDFLAGS = -z -m$(MAP) --heap_size=80 --stack_size=80 $(LIBDIR) --reread_libs \
          --warn_sections --rom_model $(LNK_CMD) -llibc.a

# -------------------------------
# Reglas
# -------------------------------
.PHONY: all clean flash

all: $(OUT)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(LD) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@
	$(OBJCOPY) -o $(HEX) -O ihex $@

flash: $(OUT)
	$(MSPDEBUG) "prog $(OUT)" "run"

clean:
	rm -f $(OBJ) $(OUT) $(HEX) $(MAP)
debug:
	$(MSPDEBUG)
