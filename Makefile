# Variáveis do compilador
CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# Nome do executável e fontes
TARGET   = main
SRC      = main.cpp

.PHONY: all run test clean

# Alvo padrão: compilar o programa
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Alvo para executar o programa
run: $(TARGET)
	./$(TARGET)

# Alvo para executar testes automaticamente.
# Neste exemplo, presume-se que exista um arquivo de entrada "test_input.txt"
# para alimentar os testes. Se não houver, remova ou ajuste este alvo.
test: $(TARGET)
	./$(TARGET) < test_input.txt

# Alvo para limpar os arquivos compilados
clean:
	rm -f $(TARGET)
