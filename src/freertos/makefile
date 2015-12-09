# src Makefile
include ../../Makefile.common
CFLAGSlib+=-c

all: libs

libs: rtos.a

rtos.a: 
	@cd ARM_CM3 && \
	$(CC) $(CFLAGSlib) \
	-D"assert_param(expr)=((void)0)" \
	-I../inc \
	*.c \

	@cd src && \
	$(CC) $(CFLAGSlib) \
	-D"assert_param(expr)=((void)0)" \
	-I../inc \
	*.c \

	@$(AR) cr -o librtos.a  \
	ARM_CM3/*.o \
	src/*.o \


.PHONY: src clean tshow

clean:
	@echo "#############   cleaning rtos  #########"
	@rm -f librtos.a 
	@rm -f ARM_CM3/*.o
	@rm -f src/*.o

tshow:
	@echo "######################################################################################################"
	@echo "######## optimize settings: $(InfoTextLib), $(InfoTextSrc)"
	@echo "######################################################################################################"

