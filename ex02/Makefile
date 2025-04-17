NAME				:= PmergeMe

SRCS_DIR			:= src
OBJS_DIR			:= obj
DEPS_DIR			:= dep
ROOT_DIR			:= $(shell pwd)
TEST_DIR			:= test
BUILD_DIR			:= $(TEST_DIR)/build

CXX				:= c++
CXXFLAGS			:= -std=c++98 -Wall -Wextra -Werror -Wpedantic -Wconversion -Wcast-align -Wshadow -Wcast-qual -Wold-style-cast -Wunused
DEPFLAGS			:= -MMD -MP -MF $(DEPS_DIR)/$*.d
RM				:= rm -rf

SRC_EXT := cpp
VPATH := $(SRCS_DIR)

SRCS := $(wildcard $(addsuffix /*.$(SRC_EXT), $(VPATH)))
DEPS := $(patsubst $(SRCS_DIR)/%.cpp,$(DEPS_DIR)/%.d,$(SRCS))
OBJS := $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
INCLUDES := -I$(SRCS_DIR)

all: $(DEPS_DIR) $(OBJS_DIR) $(NAME)

$(DEPS_DIR):
	@mkdir -p $@

$(OBJS_DIR):
	@mkdir -p $(dir $@)

$(NAME): $(OBJS)
	@mkdir -p logs/
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DEPS_DIR)/%.d: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS_DIR) $(DEPS_DIR) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test:
	echo test

debug: CXXFLAGS += -fsanitize=address -g 
debug: all

-include $(DEPS)

.PHONY: all clean fclean re test debug
