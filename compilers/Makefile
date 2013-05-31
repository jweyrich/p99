define-dump-${CC}.txt :
	 ${CC} -I../p99 -x c -pthread ../p99/p99_[b-z]*  -std=c99 -E -dM | sort -u > $@
