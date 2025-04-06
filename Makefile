CXX = g++
CXXFLAGS = -arch arm64 -std=c++17 `llvm-config --cppflags --system-libs` -Wall -MMD -I/opt/X11/include
LDFLAGS = `llvm-config --ldflags --libs core` -L/opt/X11/lib -lX11 -L/opt/homebrew/Cellar/llvm/20.1.2/lib -lLLVM-20
EXEC = cpascal
OBJECTS = token.o lexer.o main.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} ${LDFLAGS} -o ${EXEC}

-include ${DEPENDS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@
	# Generate dependency files
	${CXX} ${CXXFLAGS} -MM $< > ${@:.o=.d}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS}
