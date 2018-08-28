VPATH = osal:soem:oshw
vpath %.c osal
vpath %.c soem
vpath %.c oshw
vpath %.c test

TARGET = EtherCAT

CC = gcc

CFLAGS =  -I./oshw -I./osal -I./soem -I./

LIBS = -lrt -lpthread

OSAL_DIR = osal
OSHW_DIR = oshw
SOEM_DIR = soem
TEST_DIR = test


BUILD_DIR = Debug
OBJ_DIR  = $(BUILD_DIR)/obj
DEPS_DIR = $(BUILD_DIR)/dep

SRC_C	= $(wildcard $(OSAL_DIR)/*.c) $(wildcard $(OSHW_DIR)/*.c)\
	      $(wildcard $(SOEM_DIR)/*.c) $(wildcard $(TEST_DIR)/*.c)
OBJ_C	= $(addprefix $(OBJ_DIR)/,$(patsubst %.c,%.o,$(notdir $(SRC_C))))
SRC_MK  = $(addprefix $(DEPS_DIR)/,$(patsubst %.c,%.d,$(notdir $(SRC_C))))

#all:
#	echo $(SRC_MK)

$(TARGET):$(OBJ_C)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


$(OBJ_DIR)/%.o:%.c
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi;\
	$(CC) -o $@ -c $< $(CFLAGS) $(LIBS)



$(DEPS_DIR)/%.d:%.c
	@if [ ! -d $(DEPS_DIR) ]; then mkdir -p $(DEPS_DIR); fi;\
	set -e;\
	$(CC) -MM $< > $@.$$$$;\
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$

	-include(SRC_MK)


.PHONY:clean
clean:
	rm -rf $(TARGET) $(BUILD_DIR)














