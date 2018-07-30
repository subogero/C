%: %.c
	gcc -o $@ $<
	./$@ 2>&1 | tee log
