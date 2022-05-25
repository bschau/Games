all: distclean

distclean:
	make -C adventure distclean
	make -C catch distclean
	make -C craps distclean
	make -C galaxypatrol distclean
	make -C tictactoe distclean
	find . -iname "*~" -type f -exec rm -f {} \;

