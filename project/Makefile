# -------------- #
# -- Makefile -- #
# -------------- #

# Copyright (c) 2000-2017 Lionel Lacassagne


# -- File list ----------
FILE =  nrutil.c  vnrutil.c morpho_SIMD.c mouvement_SIMD.c test_morpho_SIMD.c

# -- Paths ----------
# standalone version
SRC_PATH = src
OBJ_PATH = obj
EXE_PATH = exe
INC_PATH = include

# -- OS ----------

# -- Config ----------
# if CONFIG = CLI  (Command Line Interface, no Apple Framework)
CONFIG = CLI

# -- Macros ----------
CC = aarch64-linux-gnu-gcc
AR = ar -rc

# -- Flags ----------
C_CC_FLAGS    = -std=c99 -DNOALIAS -DALIGNED -DmyNeon -DENABLE_FMA -Wformat=0 -Wno-unused-result
#C_CC_FLAGS    = -std=c99 -DNOALIAS -DALIGNED -DmyNeon -Wformat=0 -Wno-unused-result
C_CC_FLAGS    = -std=c99  -Wformat=0 -Wno-unused-result -flax-vector-conversions
C_CC_FLAGS    = -std=gnu99  -Wformat=0 -Wno-unused-result -flax-vector-conversions
#gnu99 permet d'eviter de faire un #define _GNU_SOURCE avant d'utiliser clock_gettime qui est posix (hors standard)

C_DEBUG_FLAGS = -O0
C_OPTIMISATION_FLAGS = -O3 -fstrict-aliasing
C_OPTIMISATION_FLAGS = -O0 -fstrict-aliasing
# tester -O0, -O1, -O2, -O3 (-O3) pour la mise au point du code = compilation plus rapide

C_ARCH_FLAGS = #-mcpu=native -mtune=native
C_OS_FLAGS = 
#C_CONFIG_FLAGS = -openmp -openmp-report=0 -D$(CONFIG)
C_CONFIG_FLAGS = -D$(CONFIG) -fopenmp -DOPENMP
C_CONFIG_FLAGS = -D$(CONFIG)
#C_CONFIG_FLAGS = -D$(CONFIG)
C_INC_FLAGS = -I$(INC_PATH)

# NRC + vNRC
LIB_INC_PATH = -I$(INC_NRC_PATH) -I$(INC_VNRC_PATH)
LIB_LIB_PATH = -L$(LIB_NRC_PATH) -L$(LIB_VNRC_PATH)


#version benchmark
CFLAGS =  $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_CC_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_INC_PATH)
LDFLAGS = $(C_OPTIMISATION_FLAGS) $(C_ARCH_FLAGS) $(C_CC_FLAGS) $(C_CONFIG_FLAGS) $(C_INC_FLAGS) $(LIB_LIB_PATH) 

# -- Final product ----------
PRODUCT   = tp_neon_$(CC).exe

# -- src and obj List ----------
SRC = $(addprefix ${SRC_PATH}/, $(FILE))
OBJ = $(addprefix ${OBJ_PATH}/, $(addsuffix .o, $(basename $(FILE))))

# -- Base rules ----------
$(OBJ_PATH)/%.o : $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
   
#-----Main rule ----------
$(EXE_PATH)/$(PRODUCT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(OPTFLAGS) $(CFG) $(INC) $(LIB) -lm 

# -- Other stuff ----------
run:
	qemu-aarch64 -L /usr/aarch64-linux-gnu/ ./exe/tp_neon_aarch64-linux-gnu-gcc.exe
	
clean:
	rm -f $(OBJ)
	rm -f ${LIB_PATH}/${PRODUCT}
