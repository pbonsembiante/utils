all:
	@$(MAKE) -C collections -f Makefile install
	@$(MAKE) -C collections -f Makefile
	@$(MAKE) -C collections -f Makefile clean

