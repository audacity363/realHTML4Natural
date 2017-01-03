all: html_parser
	@echo "Done.."

html_parser: var_lib
	cd ./libs/html_parser/ && make lib

lda_parser: var_lib
	cd ./libs/nat-lda-parser/ && make lib

var_lib:
	cd ./libs/varhandle_lib/ && make lib
