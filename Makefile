DIRS = ./new_graphic \
	   ./logic

default:
	@for dir in $(DIRS); \
		do \
		cd $$dir;make; \
		done

.PHONY: clean

clean: 
	@for dir in $(DIRS); \
		do \
		cd $$dir;make clean; \
		done
