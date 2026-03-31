SRC := $(shell find src -name "*.cpp")
BUILDDIR = build
TARGET = ./$(BUILDDIR)/main
OBJECTS := $(SRC:src/%.cpp=build/%.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

$(OBJECTS): build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) -c $^ -o $@

run: $(TARGET)
	$(TARGET)

clean:
	rm -r $(BUILDDIR)

.PHONY: run clean
