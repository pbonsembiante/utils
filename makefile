all:
	@$(MAKE) -C src -f makefile.mk install
	@$(MAKE) -C src -f makefile.mk
	@$(MAKE) -C src -f makefile.mk clean

