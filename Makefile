DIRS = ./logic \
	   ./doc

DOCDIR = ./doc

default:
	@for dir in $(DIRS); \
		do \
		cd $$dir;make auto;cd ..; \
		done

.PHONY: doc

doc: 
	cd $(DOCDIR); make; cd ..  

.PHONY: clean

clean: 
	@for dir in $(DIRS); \
		do \
		cd $$dir;make clean;cd ..; \
		done
