DIRS = ./doc \
	   ./logic

default:
	@for dir in $(DIRS); \
		do \
		cd $$dir;make auto; \
		done

.PHONY: clean

clean: 
	@for dir in $(DIRS); \
		do \
		cd $$dir;make clean; \
		done
