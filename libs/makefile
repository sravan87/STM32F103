# libs Makefile
include ../Makefile.common
LIBS+= libstm32.a
CFLAGSlib+=-c


all: libs

libs: $(LIBS)

libstm32.a:
	@echo -n "Building $@ ..."
	@cd $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates && \
	$(CC) $(CFLAGSlib) \
	system_stm32f10x.c
	@cd $(STMLIB)/STM32F10x_StdPeriph_Driver/src && \
	$(CC) $(CFLAGSlib) \
	-D"assert_param(expr)=((void)0)" \
	-I../../CMSIS/Include \
	-I../../CMSIS/Device/ST/STM32F10x/Include \
	-I../inc \
	*.c
#	@cd $(STMLIB)/STM32_USB-FS-Device_Driver/src && \
#	$(CC) $(CFLAGSlib) \
#	-D"assert_param(expr)=((void)0)" \
#	-I../../CMSIS/Include \
#	-I../../CMSIS/Device/ST/STM32F10x/Include \
#	-I../inc \
#	*.c
	@$(AR) cr $(LIBDIR)/$@ \
	$(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/system_stm32f10x.o \
	$(STMLIB)/STM32F10x_StdPeriph_Driver/src/*.o \
#	$(STMLIB)/STM32_USB-FS-Device_Driver/src/*.o
	@echo "done."


libprj.a:
	@echo -n "Building $@ ..."
	@cd $(STMLIB)/../Projects/Mass_Storage/src && \
	$(CC) $(CFLAGSlib) \
	-D"assert_param(expr)=((void)0)" \
	-I../../../Libraries/CMSIS/Include \
	-I../../../Libraries/CMSIS/Device/ST/STM32F10x/Include \
	-I../../../Libraries/STM32F10x_StdPeriph_Driver/Inc \
	-I../inc \
	*.c
	@$(AR) cr $(LIBDIR)/$@ \

#	$(STMLIB)/STM32_USB-FS-Device_Driver/src/*.o
	@echo "done."

.PHONY: libs clean tshow

clean:
	@echo "#############   cleaning CMSIS and StdPeriph_Driver  ##########"
	@rm -f $(STMLIB)/CMSIS/Device/ST/STM32F10x/Source/Templates/system_stm32f10x.o
	@rm -f $(STMLIB)/STM32F10x_StdPeriph_Driver/src/*.o
#	@rm -f $(STMLIB)/STM32_USB-FS-Device_Driver/src/*.o
	@rm -f $(LIBS)

tshow:
	@echo "######################################################################################################"
	@echo "######## optimize settings: $(InfoTextLib), $(InfoTextSrc)"
	@echo "######################################################################################################"

