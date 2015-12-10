# src Makefile
include ../Makefile.common
OBJS+=startup.o
OBJS+=main.o

all: src 

src: libs app.a

app.a: $(OBJS)
	$(AR) cr app.a $(OBJS)

libs:
ifeq ($(FREE_RTOS_SUPPORTED), 1)
	@$(MAKE) -C freertos $@
endif

.PHONY: libs src clean tshow

clean:
	@echo "#############   cleaning src ##############"
	@rm -f app.a $(OBJS)
ifeq ($(FREE_RTOS_SUPPORTED), 1)
	@$(MAKE) -C freertos $@
endif

tshow:
	@echo "######################################################################################################"
	@echo "######## optimize settings: $(InfoTextLib), $(InfoTextSrc)"
	@echo "######################################################################################################"

