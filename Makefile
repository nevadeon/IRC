NAME := ircserv

INC_PATH := include
SRC_PATH := src
OBJ_PATH := obj

# CC := c++
# CFLAGS = -Wall -Wextra -Werror -I$(INC_PATH) -std=c++98 -g
# CFLAGS += $(foreach dir, $(shell find $(INC_PATH) -type d), -I$(PATH))
# GDB_FLAGS := --quiet --args
CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g
CPPFLAGS := -I$(INC_PATH)
# LDFLAGS  :=
# LDLIBS   :=
SRC := $(shell find $(SRC_PATH) -type f -name "*.cpp")
OBJ := $(patsubst $(SRC_PATH)/%.cpp, $(OBJ_PATH)/%.o, $(SRC))

# ============================================================================ #
#        Main rules                                                            #
# ============================================================================ #

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $^
# 	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
# 	@$(CC) $(CFLAGS) -c $< -o $@

re: fclean all

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

# ============================================================================ #
#        Test rules                                                            #
# ============================================================================ #

valgrind: all
	valgrind ./$(NAME)

gdb: all
	gdb $(GDB_FLAGS) ./$(NAME)

.PHONY: all re clean fclean valgrind gdb
