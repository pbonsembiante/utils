all:
	@$(MAKE) -C collections/src -f Makefile install
	@$(MAKE) -C collections/src -f Makefile
	@$(MAKE) -C collections/src -f Makefile clean

