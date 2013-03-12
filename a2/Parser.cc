#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

/**
 * Some utility functions
 */
namespace Util {
  bool isNumber(const std::string &content);
  double toDouble(const std::string &s);
}

bool Util::isNumber(const std::string &content) {
  const char *p = content.c_str();
  bool dot = false;
  while (*p) {
    switch (*p) {
      case '.':
        if (dot) {
          return false;
        } else {
          dot = true;
        }
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        break;
      default:
        return false;
    }
    p++;
  }
  return true;
}

double Util::toDouble(const std::string &s) {
  std::istringstream i(s);
  double x;
  if (!(i >> x)) {
    throw "toDouble: failed to cast string to double";
  }
  return x;
}

/**
 * Struct and class declarations
 */

class VM {
  public:
  enum opcode {SET, SETI, PRINT, PRINTI};
  struct Instruction { /* This is the "Variable" struct, except for instructions */
    Instruction();
    Instruction(const Instruction &other);
    opcode op;
    int dst;
    union {
      int src;
      double imm;
      union { int src; double imm; } data; /* hack to make it easier to copy */
    };
  };
  struct Memory { /* This is the Memory struct */
    std::vector<Instruction> instructions; /** Objective complete: Use a container class in the C++ library */
    std::vector<double> variables; /* As required, variables are kept in a stl container */
  };
  Memory memory;
  void run();
};

class Scanner {
  public:
  enum token_type {T_INVAL, T_SET, T_PRINT, T_EQUALS, T_VALUE, T_IDENTIFIER};
  struct Token {
    token_type type;
    std::string content;
    void to_string(std::string &s) const;
    std::ostream &dump(std::ostream &o) const;
  };
  Scanner(const char *filename);
  ~Scanner();
  bool nextToken(Token &token); /* Returns true if a token was successfully scanned */
  int getLine();
  std::string getFilename();
  private:
  std::string name;
  int lineno;
  char *buffer, *p;
  bool eol;
};

class Parser {
  public:
  Parser(Scanner &s, VM::Memory &m);
  void parse();
  private:
  int resolve_symbol(const std::string &symbol, bool createIfMissing = false);
  void print_error(const char *err) const;
  void unexpected(const Scanner::Token &t) const;
  Scanner &scanner;
  VM::Memory &memory;
  std::vector<std::string> symbol_table;
};

/**
 * Struct and class definitions
 */

Scanner::Scanner(const char *filename) {
  /* Assumption: The files are small (less than ~10MB), it is fastest to read the whole thing into memory and process from there */
  int size;
  std::ifstream file(filename, std::ios::in|std::ios::binary|std::ios::ate);
  if (file.is_open()) {
    size = (int)file.tellg();
    try {
      buffer = new char[size+1]; /* For terminal null */
    } catch (std::bad_alloc&) {
      throw "Scanner: Error allocating memory.";
    }
    buffer[size] = 0; /* To mark EOF */
    file.seekg (0, std::ios::beg);
    file.read (buffer, size); /** Objective complete: Learn how to read from a file using a stream */
    file.close();
    p = buffer; /* read pointer */
    lineno = 1;
    eol = false;
    name.assign(filename);
  } else {
    throw "Scanner: Error opening file.";
  }
}
Scanner::~Scanner() {
  delete [] buffer;
}
bool Scanner::nextToken(Scanner::Token &token) {
  token.content.clear();
  eol = false;
  /* Get contents of token */
  while (*p) {
    char &c = *p;
    p++;
    if (c == '\r') { /* in case of wandowz */
      continue;
    } else if (c == '!') { /* Comment */
      while (*p && *p != '\n') { /* consume until end of line or file, whichever comes first */
        p++;
      }
    } else if (c != ' ' && c != '\n') {
      token.content.push_back(c);
    } else {
      if (c == '\n') {
        lineno++;
        eol = true;
      }
      if (!token.content.empty()) { /* to handle multiple delimiters in a row */
        break;
      } else {
        eol = false;
      }
    }
  }
  /* Classify the token */
  if (token.content.empty()) {
    token.type = Scanner::T_INVAL;
    return false;
  } else if (token.content.compare("SET") == 0) {
    token.type = Scanner::T_SET;
  } else if (token.content.compare("PRINT") == 0) {
    token.type = Scanner::T_PRINT;
  } else if (token.content.compare("=") == 0) {
    token.type = Scanner::T_EQUALS;
  } else if (Util::isNumber(token.content)) {
    token.type = Scanner::T_VALUE;
  } else {
    token.type = Scanner::T_IDENTIFIER;
  }
  return true;
}

int Scanner::getLine() {
  return (lineno - (eol?1:0));
}

std::string Scanner::getFilename() {
  return name;
}

Parser::Parser(Scanner &s, VM::Memory &m) : scanner(s), memory(m) {}

void Scanner::Token::to_string(std::string &s) const {
  switch (type) {
    case T_SET:
      s.assign("T_SET");
      break;
    case T_PRINT:
      s.assign("T_PRINT");
      break;
    case T_EQUALS:
      s.assign("T_EQUALS");
      break;
    case T_VALUE:
      s.assign("T_VALUE");
      break;
    case T_IDENTIFIER:
      s.assign("T_IDENTIFIER");
      break;
    default:
      throw "Invalid token.";
      break;
  }
}

std::ostream &Scanner::Token::dump(std::ostream &o) const {
  std::string out;
  this->to_string(out);
  return o << out;
}
std::ostream &operator<<(std::ostream &o, const Scanner::Token &t) { return t.dump(o); }

void Parser::print_error(const char *err) const {
  std::cerr << "Parse error: " << err << " in " << scanner.getFilename() << " on line " << scanner.getLine() << "\n";
}

void Parser::unexpected(const Scanner::Token &t) const {
  std::cerr << "Parse error: syntax error, unexpected " << t << " in " << scanner.getFilename() << " on line " << scanner.getLine() << "\n";
}

/** Translates from variable name to "memory address" using the symbol table, this satisfies the requirement
 *  "For both print and set, you need to search through the list of variables in the container to see
 *   if any of them match the variable name specified."
 */
int Parser::resolve_symbol(const std::string &symbol, bool createIfMissing) {
  std::vector<std::string>::iterator it;
  if ((it = std::find(symbol_table.begin(), symbol_table.end(), symbol)) != symbol_table.end()) { /** Objective complete: Use a function from algorithm.h from the C++ library (find) */
    return it - symbol_table.begin();
  } else if (createIfMissing) {
    symbol_table.push_back(symbol);
    return symbol_table.size() - 1;
  } else {
    std::string err("Undefined variable: ");
    err += symbol;
    this->print_error(err.c_str());
    throw 1;
  }
}

void Parser::parse() {
  Scanner::Token token;
  VM::Instruction instruction;
  int state = 0;
  while (scanner.nextToken(token)) {
    switch (state) {
      case 0:
        /* Beginning of line */
        if (token.type == Scanner::T_SET) {
          state = 1;
        } else if (token.type == Scanner::T_PRINT) {
          state = 2;
        } else {
          this->unexpected(token);
          throw 1;
        }
        break;
      case 1:
        /* SET _ */
        if (token.type == Scanner::T_IDENTIFIER) {
          instruction.dst = resolve_symbol(token.content, true);
          state = 3;
        } else {
          this->unexpected(token);
          throw 1;
        }
        break;
      case 2:
        /* PRINT _ */
        if (token.type == Scanner::T_IDENTIFIER) {
          instruction.op = VM::PRINT;
          instruction.src = resolve_symbol(token.content);
        } else if (token.type == Scanner::T_VALUE) {
          instruction.op = VM::PRINTI;
          instruction.imm = Util::toDouble(token.content);
        } else {
          this->unexpected(token);
          throw 1;
        }
        memory.instructions.push_back(instruction);
        state = 0;
        break;
      case 3:
        /* SET IDENTIFIER _ */
        if (token.type == Scanner::T_EQUALS) {
          state = 4;
        } else {
          this->unexpected(token);
          throw 1;
        }
        break;
      case 4:
        /* SET IDENTIFIER = _ */
        if (token.type == Scanner::T_IDENTIFIER) {
          instruction.op = VM::SET;
          instruction.src = resolve_symbol(token.content);
        } else if (token.type == Scanner::T_VALUE) {
          instruction.op = VM::SETI;
          instruction.imm = Util::toDouble(token.content);
        } else {
          this->unexpected(token);
          throw 1;
        }
        memory.instructions.push_back(instruction);
        state = 0;
        break;
      default:
        throw "FATAL: Parser malfunction: Impossible state!";
        break;
    }
  }
  if (state != 0) {
    this->print_error("syntax error, unexpected end of file");
    throw 1;
  } else {
    memory.variables.resize(symbol_table.size());
  }
}

VM::Instruction::Instruction() {
  op = SET;
  dst = -1;
  imm = 0;
}

VM::Instruction::Instruction(const VM::Instruction &other) {
  op = other.op;
  dst = other.dst;
  data = other.data;
}

void VM::run() {
  /** Objective complete: Practice using iterators */
  for (std::vector<VM::Instruction>::iterator it = memory.instructions.begin(); it != memory.instructions.end(); ++it) {
    VM::Instruction &instruction = *it;
    switch (instruction.op) {
      case SET:
        memory.variables[instruction.dst] = memory.variables[instruction.src];
        break;
      case SETI:
        memory.variables[instruction.dst] = instruction.imm;
        break;
      case PRINT:
        std::cout << memory.variables[instruction.src] << "\n";
        break;
      case PRINTI:
        std::cout << instruction.imm << "\n";
        break;
      default:
        break;
    }
  }
}

/**
 * Main
 * File -> Scanner -|tokens|-> Parser -|opcodes|-> VM -> Execute
 */
int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << argv[0] << ": No input file specified. Usage: " << argv[0] << " <FILE>\n";
    return 1;
  }
  try {
    Scanner scanner(argv[1]);
    VM vm;
    Parser parser(scanner, vm.memory);
    parser.parse();
    vm.run();
    /* The VM and parser can be run in seperate threads with appropriate mutexing for a sort of "just in time" compile behavior,
       but is out of the scope of this assignment, and is unlikely to provide measurable performance gains with such a simple language */
  }
  catch (const char *s) {
    std::cerr << s << "\n";
    return 1;
  }
  catch (...) {
    return 1;
  }
  return 0;
}

