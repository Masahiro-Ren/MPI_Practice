MPICXX = mpic++
MPIFLAGS = -std=c++17

send_bnd:
	$(MPICXX) $(MPIFLAGS) send_bnd.cpp -o send_bnd

send_bnd2:
	$(MPICXX) $(MPIFLAGS) send_bnd2.cpp -o send_bnd2

.PHONY: clean
clean:
	rm -f send_bnd send_bnd2