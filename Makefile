all: testing

testing: src/testing.cpp
	@echo "---"
	@echo "Compiling testing.cpp"

	gcc src/testing.cpp -lstdc++ -o src/testing

	@echo "Finished compiling testing.cpp"\n