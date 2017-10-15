libraries:
	python ./version.py
	$(MAKE) -C ./libbtns library
	$(MAKE) -C ./libfbc library
	$(MAKE) -C ./libmtrmgr library
	# cp */*-template.zip .
