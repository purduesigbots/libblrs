libraries:
	$(MAKE) -C ./libbtns library
	$(MAKE) -C ./libfbc library
	$(MAKE) -C ./liblcd library
	$(MAKE) -C ./libmtrmgr library
	# cp */*-template.zip .
