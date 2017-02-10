all:
	@$(MAKE) -C collections/src -f makefile.mk install
	@$(MAKE) -C collections/src -f makefile.mk
	@$(MAKE) -C collections/src -f makefile.mk clean

