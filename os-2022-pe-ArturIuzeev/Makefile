all: pe-parser.cpp
	g++ -o pe-parser pe-parser.cpp
clean:
	${RM} pe-parser
validation-pe-tests: all
	python3 -m tests ValidatingPeTestCases -f
import-dll-tests: all
	python3 -m tests ImportDllTestCases -f
import-function-tests: all
	python3 -m tests ImportFunctionTestCases -f
