MPICXX = mpicxx
CXXFLAGS = --std=c++17

TARGET = oneside_comm

$(TARGET):
	$(MPICXX) $(CXXFLAGS) oneside_comm.cpp -o $(TARGET)

.PHONY: clean
clean:
	rm -f $(TARGET)