%: %.c
	gcc -o $@ $<
	./$@ 2>&1 | tee log
%: %.cpp
	g++ -o $@ $<
	./$@ 2>&1 | tee log
