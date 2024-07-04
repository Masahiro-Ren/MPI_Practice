MPICXX = mpicxx
CXXFLAGS = --std=c++17


TARGET1 = persistent_comm
TARGET2 = persistent_comm2

$(TARGET1):
	$(MPICXX) $(CXXFLAGS) persistent_comm.cpp -o $(TARGET1)

$(TARGET2):
	$(MPICXX) $(CXXFLAGS) persistent_comm2.cpp -o $(TARGET2)

.PHONY: clean
clean:
	rm -f $(TARGET1) $(TARGET2)

